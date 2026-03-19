/*
 * Copyright (c) 2025 Blynk Technologies Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <string.h>
#include "blynk_edgent.h"

#define MQTT_QOS_DEFAULT (0)
#define EDGENT_TOPIC_BUF_LEN (32 + 128) // max prefix + max DS name
#define EDGENT_DATA_BUF_LEN (32) // max string data for double and integer conversions

static char topic[EDGENT_TOPIC_BUF_LEN];
static char data[EDGENT_DATA_BUF_LEN];

static SemaphoreHandle_t edgent_api_mutex;
#define EDGENT_LOCK()   xSemaphoreTake(edgent_api_mutex, portMAX_DELAY)
#define EDGENT_UNLOCK() xSemaphoreGive(edgent_api_mutex)

edgent_err edgent_publish_ds_str(const char* ds, const char* data_in) {
   EDGENT_LOCK();

   snprintf(topic, sizeof(topic), "ds/%s", ds);
   edgent_err rc = edgent_mqtt_publish(topic, data_in, strlen(data_in), MQTT_QOS_DEFAULT);

   EDGENT_UNLOCK();
   return rc;
}

edgent_err edgent_publish_ds_int(const char* ds, int64_t value) {
   EDGENT_LOCK();

   int topic_len = snprintf(topic, sizeof(topic), "ds/%s", ds);
   if (topic_len < 0 || topic_len >= (int)sizeof(topic)) {
      EDGENT_UNLOCK();
      return EDGENT_ERR_INVALID_ARG;
   }

   int data_len = snprintf(data, sizeof(data), "%" PRId64, value);
   if (data_len < 0 || data_len >= (int)sizeof(data)) {
      EDGENT_UNLOCK();
      return EDGENT_ERR_NO_MEM;
   }

   edgent_err rc = edgent_mqtt_publish(topic, data, data_len, MQTT_QOS_DEFAULT);

   EDGENT_UNLOCK();
   return rc;
}

edgent_err edgent_publish_ds_float(const char* ds, double value, uint8_t precision) {
   EDGENT_LOCK();

   int topic_len = snprintf(topic, sizeof(topic), "ds/%s", ds);
   if (topic_len < 0 || topic_len >= (int)sizeof(topic)) {
      EDGENT_UNLOCK();
      return EDGENT_ERR_INVALID_ARG;
   }

   if (precision > 10){
      precision = 10;
   }

   int data_len = snprintf(data, sizeof(data), "%.*f", precision, value);
   if (data_len < 0 || data_len >= (int)sizeof(data)) {
      EDGENT_UNLOCK();
      return EDGENT_ERR_NO_MEM;
   }

   edgent_err rc = edgent_mqtt_publish(topic, data, data_len, MQTT_QOS_DEFAULT);

   EDGENT_UNLOCK();
   return rc;
}

edgent_err edgent_get_metadata(const char* meta) { 
   return edgent_mqtt_publish("get/meta", meta, strlen(meta), MQTT_QOS_DEFAULT);
}

edgent_err edgent_set_metadata(const char* meta, const char* value)
{
    if (!meta || !value) {
        return EDGENT_ERR_INVALID_ARG;
    }

    EDGENT_LOCK();

    int topic_len = snprintf(topic, sizeof(topic), "meta/%s", meta);
    if (topic_len < 0 || topic_len >= (int)sizeof(topic)) {
        EDGENT_UNLOCK();
        return EDGENT_ERR_NO_MEM;
    }

    edgent_err rc = edgent_mqtt_publish(topic, value, strlen(value), MQTT_QOS_DEFAULT);

    EDGENT_UNLOCK();
    return rc;
}

edgent_err edgent_get_utc(){
   return edgent_mqtt_publish("get/utc/all/json", NULL, 0, MQTT_QOS_DEFAULT);
}

edgent_err edgent_get_location(){
   return edgent_mqtt_publish("get/loc/all", NULL, 0, MQTT_QOS_DEFAULT);
}

edgent_err edgent_get_ds(const char* ds) {
   return edgent_mqtt_publish("get/ds", ds, strlen(ds), MQTT_QOS_DEFAULT);
}

edgent_err edgent_get_ds_all() { 
   return edgent_mqtt_publish("get/ds/all", NULL, 0, MQTT_QOS_DEFAULT); 
}

edgent_err edgent_set_property(const char* ds, const char* prop, const char* value) {
   if (!ds || !prop) {
      return EDGENT_ERR_INVALID_ARG;
   }

   EDGENT_LOCK();

   int topic_len = snprintf(topic, sizeof(topic), "ds/%s/prop/%s", ds, prop);

   if (topic_len < 0 || topic_len >= (int)sizeof(topic)) {
      EDGENT_UNLOCK();
      return EDGENT_ERR_NO_MEM;
   }

   edgent_err rc = edgent_mqtt_publish(topic, value, strlen(value), MQTT_QOS_DEFAULT);

   EDGENT_UNLOCK();
   return rc;
}

edgent_err edgent_clear_property(const char* ds, const char* prop) {
   if (!ds || !prop) {
      return EDGENT_ERR_INVALID_ARG;
   }

   EDGENT_LOCK();

   int topic_len = snprintf(topic, sizeof(topic), "ds/%s/prop/%s/erase", ds, prop);

   if (topic_len < 0 || topic_len >= (int)sizeof(topic)) {
      EDGENT_UNLOCK();
      return EDGENT_ERR_NO_MEM;
   }

   edgent_err rc = edgent_mqtt_publish(topic, NULL, 0, MQTT_QOS_DEFAULT);

   EDGENT_UNLOCK();
   return rc;
}

edgent_err edgent_log_event(const char* event, const char* description) {
   if (!event) {
      return EDGENT_ERR_INVALID_ARG;
   }

   EDGENT_LOCK();

   int topic_len = snprintf(topic, sizeof(topic), "event/%s", event);
   if (topic_len < 0 || topic_len >= (int)sizeof(topic)) {
      EDGENT_UNLOCK();
      return EDGENT_ERR_NO_MEM;
   }

   edgent_err rc = description ? edgent_mqtt_publish(topic, description, strlen(description), MQTT_QOS_DEFAULT)
                               : edgent_mqtt_publish(topic, NULL, 0, MQTT_QOS_DEFAULT);

   EDGENT_UNLOCK();
   return rc;
}

// Small trick for glue weak implementations during linking
void edgent_internal_init(const edgent_config_t* config);
void fwinfo_init();

void edgent_init(const edgent_config_t* config) {
   edgent_api_mutex = xSemaphoreCreateMutex();
   fwinfo_init(); // dummy call to embed fwinfo
   edgent_internal_init(config);
}
