/*
 * Copyright (c) 2025 Blynk Technologies Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef BLYNK_EDGENT_H
#define BLYNK_EDGENT_H

#ifdef __cplusplus
extern "C" {
#endif

#include "esp_event.h"

typedef void (*edgent_ds_cb_t)(const char* topic, int topic_len, const char* data, int data_len);
typedef void (*edgent_cb_t)(void);
typedef int edgent_err;

// Blynk Edgent operational states
typedef enum {
   EDGENT_STATE_UNKNOWN = -1, //Unknown state (unitialized)
   EDGENT_STATE_IDLE, // No active operation
   EDGENT_STATE_CONNECTING_NET, // Connecting to Wi-Fi / Ethernet / PPP
   EDGENT_STATE_CONNECTING_CLOUD, // Connected to network, connecting to Blynk Cloud
   EDGENT_STATE_CONNECTED, // Normal running mode (cloud connected)
   EDGENT_STATE_WAIT_CONFIG, // Waiting for provisioning/config
   EDGENT_STATE_RESET_CONFIG, // Resetting stored configuration
   EDGENT_STATE_OTA_UPGRADE, // OTA update in progress
   EDGENT_STATE_ERROR, // Error or fault detected
} edgent_state_t;

// Payload for the STATE_CHANGED event
typedef struct {
   edgent_state_t prev;
   edgent_state_t curr;
} edgent_state_evt_t;

ESP_EVENT_DECLARE_BASE(EDGENT_EVENT_BASE);
#define EDGENT_EVENT_STATE_CHANGED 1

/**
 * @brief Initialize the Edgent system.
 *
 * This function prepares internal structures, configuration,
 * and resources required for the Edgent system to operate.
 *
 * @return edgent_err Returns 0 on success, or an error code on failure.
 */
void edgent_init(void);

/**
 * @brief Start the Edgent system.
 *
 * This function begins Edgent operations, such as connecting to
 * the Сloud, and other runtime services.
 *
 * @return edgent_err Returns 0 on success, or an error code on failure.
 */
void edgent_start();

/**
 * @brief Reset the Edgent configuration to factory defaults.
 *
 * Clears saved credentials, settings, or any persistent configuration,
 * and prepares the system for provisioning.
 *
 */
void edgent_reset_config(void);

/**
 * @brief Start Edgent configuration (factory reset mode).
 *
 * After calling this function, the device will enter provisioning mode,
 * allowing new configuration (e.g., Wi-Fi credentials or authentication token)
 * to be set up again.
 */
void edgent_start_config(void);

/**
 * @brief (Weak) Callback invoked when a reboot is requested by Edgent.
 *
 * This function can be overridden by user code to handle reboot requests
 * (e.g., from the cloud or internal triggers). Default implementation reboots device immediately.
 */
__attribute__((weak)) void edgent_on_reboot_request(void);

/**
 * @brief Set a callback function for handling downlink/ds/DATASTREAM input events.
 *
 * This function registers a user-defined callback to handle interactions
 * with Blynk datastreams.
 *
 * @param cb User-defined callback function of type `edgent_ds_cb_t`.
 */
void edgent_set_downlink_datastream_callback(edgent_ds_cb_t cb);

/**
 * @brief Set a callback function for handling general downlink/# input events.
 *
 * This function registers a user-defined callback to handle interactions
 * with Blynk datastreams.
 *
 * @param cb User-defined callback function of type `edgent_ds_cb_t`.
 */
void edgent_set_downlink_callback(edgent_ds_cb_t cb);

/**
 * @brief Publish arbitrary data to a specified topic.
 *
 * Sends raw data to the cloud via the specified topic, using the
 * desired QoS level.
 * 
 * For supported topics and format, refer to:
 * https://docs.blynk.io/en/blynk.cloud-mqtt-api/device-mqtt-api/datastreams
 *
 * @param topic Topic name to publish to.
 * @param data Pointer to the data buffer to publish.
 * @param len Length of the data in bytes.
 * @param qos Quality of Service level (0 or 1).
 * 
 * @return edgent_err Returns 0 on success, or an error code on failure.
 */
edgent_err edgent_publish(const char* topic, const char* data, int len, int qos);

/**
 * @brief Set a callback function to handle Edgent state change events.
 *
 * This function registers a user-defined callback that is invoked whenever
 * the Edgent connection state changes — for example, when transitioning
 * between network, cloud, or idle states.
 *
 * @param cb User-defined callback function of type `edgent_cb_t`.
 */
void edgent_set_on_state_change(edgent_cb_t cb);

/**
 * @brief Set a callback function for the initial successful connection.
 *
 * This function registers a user-defined callback that is called once when
 * Edgent establishes its first successful connection to the cloud after boot
 * or configuration. It is typically used to perform initialization actions
 * that require cloud connectivity.
 *
 * @param cb User-defined callback function of type `edgent_cb_t`.
 */
void edgent_set_on_initial_connection(edgent_cb_t cb);

/**
 * @brief Set a callback function for configuration change events.
 *
 * This function registers a user-defined callback that is triggered when
 * the device configuration is modified — for example, after provisioning,
 * token change, or updated settings received from the cloud.
 *
 * @param cb User-defined callback function of type `edgent_cb_t`.
 */
void edgent_set_on_config_change(edgent_cb_t cb);

void edgent_init(void);

#ifdef __cplusplus
}
#endif

#endif // BLYNK_EDGENT_H