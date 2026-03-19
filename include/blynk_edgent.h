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

/*
 * Edgent API Overview
 * -------------------
 * - Datastream helpers follow: edgent_ds_publish_* naming.
 * - Configuration is provided via `edgent_config_t` and passed to `edgent_init()`.
 *
 * MQTT topic conventions: https://docs.blynk.io/en/blynk.cloud-mqtt-api/device-mqtt-api
 */

#include "esp_event.h"

enum edgent_err_code {
   EDGENT_OK = 0,
   EDGENT_FAIL = -3000,
   EDGENT_ERR_INVALID_ARG = -3001,
   EDGENT_ERR_NO_MEM = -3002,
};

typedef int edgent_err;

typedef void (*edgent_ds_cb_t)(const char* topic, int topic_len,
                              const char* data, int data_len);
typedef void (*edgent_cb_t)(void);

/* =========================
 * Edgent State Management
 * ========================= */

typedef enum {
   EDGENT_STATE_UNKNOWN = -1,   // Uninitialized
   EDGENT_STATE_IDLE,           // Idle
   EDGENT_STATE_CONNECTING_NET, // Connecting to network
   EDGENT_STATE_CONNECTING_CLOUD,
   EDGENT_STATE_CONNECTED,      // Fully operational
   EDGENT_STATE_WAIT_CONFIG,    // Provisioning mode
   EDGENT_STATE_OTA_UPGRADE,
   EDGENT_STATE_ERROR,
} edgent_state_t;

typedef struct {
   edgent_state_t prev;
   edgent_state_t curr;
} edgent_state_evt_t;

ESP_EVENT_DECLARE_BASE(EDGENT_EVENT_BASE);
#define EDGENT_EVENT_STATE_CHANGED 1

/* =========================
 * Configuration
 * ========================= */

typedef struct {
   edgent_ds_cb_t downlink_ds_callback;
   edgent_ds_cb_t downlink_callback;
   edgent_cb_t    state_change_callback;
   edgent_cb_t    config_change_callback;
   edgent_cb_t    initial_connection_callback;
   edgent_cb_t    reboot_request_callback;
   uint32_t       config_timeout_seconds;
   uint32_t       config_skip_limit;
} edgent_config_t;

/* =========================
 * Lifecycle API
 * ========================= */

/**
 * @brief Initialize Edgent.
 *
 * Must be called before any other API.
 */
void edgent_init(const edgent_config_t *config);

/**
 * @brief Start Edgent runtime (network + cloud).
 */
void edgent_start(void);

/**
 * @brief Stop Edgent runtime.
 */
void edgent_stop(void);

/**
 * @brief Reset stored configuration (factory reset).
 */
void edgent_config_reset(void);

/**
 * @brief Enter provisioning mode.
 */
void edgent_config_start(void);

/**
 * @brief Stop provisioning mode.
 */
void edgent_config_stop(void);

/**
 * @brief Check if device is configured.
 */
bool edgent_is_configured(void);

/* =========================
 * MQTT Publish API
 * ========================= */

/**
 * @brief Publish raw MQTT message.
 *
 * Low-level API used to send MQTT message as is
 */
edgent_err edgent_mqtt_publish(const char* topic,
                              const char* data,
                              int len,
                              int qos);

/* =========================
 * Datastream API
 * ========================= */

/**
 * @brief Publish string value to datastream.
 */
edgent_err edgent_publish_ds_str(const char* ds, const char* data);

/**
 * @brief Publish integer value to datastream.
 */
edgent_err edgent_publish_ds_int(const char* ds, int64_t value);

/**
 * @brief Publish floating-point value to datastream.
 */
edgent_err edgent_publish_ds_float(const char* ds,
                                    double value,
                                    uint8_t precision);

/**
 * @brief Request datastream value(s).
 */
edgent_err edgent_get_ds(const char* ds);

/**
 * @brief Request all datastreams value(s).
 */
edgent_err edgent_get_ds_all();

/* =========================
 * Device Info API
 * ========================= */

/**
 * @brief Request metadata (comma-separated list).
 */
edgent_err edgent_get_metadata(const char* meta);

/**
 * @brief Set metadata value.
 */
edgent_err edgent_set_metadata(const char* meta, const char* value);

/**
 * @brief Request UTC time.
 */
edgent_err edgent_get_utc(void);

/**
 * @brief Request device location.
 */
edgent_err edgent_get_location(void);

/* =========================
 * Properties & Events
 * ========================= */

/**
 * @brief Set or modify datastream property.
 *
 * @param ds Datastream name
 * @param prop Property name
 * @param value Property value
 */
edgent_err edgent_set_property(const char* ds,
                           const char* prop,
                           const char* value);

/**
 * @brief Clear datasteram property.
 *
 * @param ds Datastream name
 * @param prop Property name
 */
edgent_err edgent_clear_property(const char* ds,
                           const char* prop);

/**
 * @brief Send event to cloud.
 *
 * @param event Event name
 * @param description Optional message (NULL = no payload)
 */
edgent_err edgent_log_event(const char* event,
                            const char* description);

#ifdef __cplusplus
}
#endif

#endif // BLYNK_EDGENT_H