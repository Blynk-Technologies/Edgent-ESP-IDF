/*
 * Copyright (c) 2025 Blynk Technologies Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <esp_log.h>
#include <esp_system.h>
#include <esp_random.h>
#include <nvs_flash.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <iot_button.h>
#include <button_gpio.h>
#include "blynk_edgent.h"

static const char* TAG = "example";

static EventGroupHandle_t evt_group;
#define EVT_CONNECTED BIT0

#if CONFIG_IDF_TARGET_ESP32 || CONFIG_IDF_TARGET_ESP32S3
#   define BOOT_BUTTON_NUM 0
#elif CONFIG_IDF_TARGET_ESP32C3 || CONFIG_IDF_TARGET_ESP32C6 || CONFIG_IDF_TARGET_ESP32C2 || CONFIG_IDF_TARGET_ESP32C5
#   define BOOT_BUTTON_NUM 9
#endif
#define BUTTON_ACTIVE_LEVEL 0

static void button_event_cb(void* arg, void* data) {
   iot_button_print_event((button_handle_t)arg);
   button_event_t event = iot_button_get_event((button_handle_t)arg);
   if (BUTTON_LONG_PRESS_START == event) {
      edgent_config_reset();
   } else if (BUTTON_DOUBLE_CLICK == event) {
      edgent_config_start();
   }
}

static void button_init(uint32_t button_num) {
   button_config_t btn_cfg = {.long_press_time = 5000, .short_press_time = 200};
   button_gpio_config_t gpio_cfg = {.gpio_num = (int32_t)button_num,
                                    .active_level = BUTTON_ACTIVE_LEVEL,
                                    .enable_power_save = true,
                                    .disable_pull = true};

   button_handle_t btn;
   esp_err_t ret = iot_button_new_gpio_device(&btn_cfg, &gpio_cfg, &btn);
   assert(ret == ESP_OK);

   ret |= iot_button_register_cb(btn, BUTTON_SINGLE_CLICK, NULL, button_event_cb, NULL);
   ret |= iot_button_register_cb(btn, BUTTON_DOUBLE_CLICK, NULL, button_event_cb, NULL);
   ret |= iot_button_register_cb(btn, BUTTON_LONG_PRESS_START, NULL, button_event_cb, NULL);

   ESP_ERROR_CHECK(ret);
}

static void on_downlink_datastream_callback(const char* topic, int topic_len, const char* data, int data_len) {
   ESP_LOGI(TAG, "Datastream received — topic: %s, data: %s", topic, data);
}

static void on_downlink_callback(const char* topic, int topic_len, const char* data, int data_len) {
   ESP_LOGI(TAG, "Downlink received — topic: %s, data: %s", topic, data);
}

static void on_state_change(void) {
   ESP_LOGI(TAG, "State change event received");
}

static void on_initial_connection(void) {
   ESP_LOGI(TAG, "Initial connection established");
   edgent_get_ds_all();
   edgent_get_utc();
   edgent_get_location();
}

static void on_config_change(void) {
   ESP_LOGI(TAG, "Configuration change detected");
}

void on_reboot_request(void) {
   ESP_LOGI(TAG, "Blynk.Edgent initiated reboot");
   esp_restart();
}

// Event handler
static void blynk_edgent_event_handler(void* handler_arg, esp_event_base_t base, int32_t id, void* event_data) {
   if (id == EDGENT_EVENT_STATE_CHANGED && event_data) {
      edgent_state_evt_t* evt = (edgent_state_evt_t*)event_data;
      ESP_LOGD(TAG, "Blynk.Edgent state changed: %d -> %d", evt->prev, evt->curr);
      switch (evt->curr) {
      case EDGENT_STATE_CONNECTED:
         xEventGroupSetBits(evt_group, EVT_CONNECTED);
         break;
      case EDGENT_STATE_ERROR:
         ESP_LOGW(TAG, "Device entered ERROR state, take action!");
         break;
      default:
         break;
      }
   }
}

void other_task(void* arg) {
   EventBits_t bits = xEventGroupWaitBits(evt_group, EVT_CONNECTED, pdFALSE, pdFALSE, portMAX_DELAY);
   if (bits & EVT_CONNECTED) {
      ESP_LOGI(TAG, "[%s] Cloud connected event received! Continuing...", pcTaskGetName(NULL));
   }

   while (true) {
      double rnd = 10.0 + (esp_random() % 2501) / 100.0;
      edgent_publish_ds_float("Set Temperature", rnd, 2);
      edgent_publish_ds_int("Status", rand() % 5);
      vTaskDelay(pdMS_TO_TICKS(10000));
   }
}

extern "C" void app_main(void) {
   ESP_LOGI(TAG, "app_main() entered");

   /* Suppress log pollution */
   esp_log_level_set("phy_init", ESP_LOG_WARN);
   esp_log_level_set("wifi", ESP_LOG_ERROR);
   esp_log_level_set("wifi_init", ESP_LOG_WARN);
   esp_log_level_set("pp", ESP_LOG_WARN);
   esp_log_level_set("net80211", ESP_LOG_WARN);
   esp_log_level_set("esp_netif_handlers", ESP_LOG_WARN);
   esp_log_level_set("NimBLE", ESP_LOG_WARN);
   esp_log_level_set("mqtt_client", ESP_LOG_WARN);
   esp_log_level_set("esp_image", ESP_LOG_WARN);
   esp_log_level_set("esp_https_ota", ESP_LOG_WARN);

   /* Initialize NVS — it is used to store PHY calibration data */
   esp_err_t ret = nvs_flash_init();
   if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
      ESP_ERROR_CHECK(nvs_flash_erase());
      ret = nvs_flash_init();
   }
   ESP_ERROR_CHECK(ret);

   ESP_ERROR_CHECK(esp_event_loop_create_default());
   ESP_ERROR_CHECK(esp_event_handler_instance_register(
      EDGENT_EVENT_BASE, EDGENT_EVENT_STATE_CHANGED, blynk_edgent_event_handler, NULL, NULL));

   // Create the event group
   evt_group = xEventGroupCreate();
   assert(evt_group);

   // initialize button
   button_init(BOOT_BUTTON_NUM);

   // initialize Edgent
   edgent_config_t config = {};
   config.downlink_ds_callback = on_downlink_datastream_callback;
   config.downlink_callback = on_downlink_callback;
   config.state_change_callback = on_state_change;
   config.config_change_callback = on_config_change;
   config.initial_connection_callback = on_initial_connection;
   config.reboot_request_callback = on_reboot_request;
   edgent_init(&config);

   // start Edgent
   edgent_start();

   // create parallel task
   xTaskCreate(other_task,
               "other_task",
               2048,
               NULL,
               tskIDLE_PRIORITY + 1, // default normal priority
               NULL);

   ESP_LOGI(TAG, "Awaiting Cloud connection...");
   EventBits_t bits = xEventGroupWaitBits(evt_group, EVT_CONNECTED, pdFALSE, pdFALSE, portMAX_DELAY);
   if (bits & EVT_CONNECTED) {
      ESP_LOGI(TAG, "[%s] Cloud connected event received! Continuing...", pcTaskGetName(NULL));
   }

   static bool bON = false;
   while (true) {
      // Publish Power state
      edgent_publish_ds_int("Power", (int64_t)bON);

      // Publish random Current Temperature
      double rnd = 10.0 + (esp_random() % 2501) / 100.0;
      edgent_publish_ds_float("Current Temperature", rnd, 2);

      // Publish uptime
      uint32_t uptime_sec = xTaskGetTickCount();
      edgent_publish_ds_int("uptime", uptime_sec);

      // Request metadata
      edgent_get_metadata("Device Name");

      // Toggle widget state
      edgent_set_property("uptime", "isDisabled", bON ? "false" : "true");

      bON = !bON;
      vTaskDelay(pdMS_TO_TICKS(15000));
   }
}
