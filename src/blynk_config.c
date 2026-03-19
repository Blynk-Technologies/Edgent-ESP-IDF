/*
 * Copyright (c) 2025 Blynk Technologies Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stddef.h>
#include <stdbool.h>
#include "sdkconfig.h"
#include "blynk_edgent.h"

#define BLYNK_FIRMWARE_VERSION CONFIG_BLYNK_FIRMWARE_VERSION
#define BLYNK_TEMPLATE_ID      CONFIG_BLYNK_TEMPLATE_ID
#define BLYNK_TEMPLATE_NAME    CONFIG_BLYNK_TEMPLATE_NAME
#define BLYNK_VENDOR_PREFIX    CONFIG_BLYNK_VENDOR_PREFIX
#define BLYNK_DEFAULT_SERVER   CONFIG_BLYNK_DEFAULT_SERVER

_Static_assert(sizeof(BLYNK_FIRMWARE_VERSION) > 1, "BLYNK_FIRMWARE_VERSION must not be empty!");
_Static_assert(sizeof(BLYNK_TEMPLATE_ID) > 1, "BLYNK_TEMPLATE_ID must not be empty!");
_Static_assert(sizeof(BLYNK_TEMPLATE_NAME) > 1, "BLYNK_TEMPLATE_NAME must not be empty!");
_Static_assert(sizeof(BLYNK_VENDOR_PREFIX) > 1, "BLYNK_VENDOR_PREFIX must not be empty!");

#if CONFIG_BLYNK_FIRMWARE_TYPE_SET
#   define BLYNK_FIRMWARE_TYPE CONFIG_BLYNK_FIRMWARE_TYPE
#else
#   define BLYNK_FIRMWARE_TYPE BLYNK_TEMPLATE_ID
#endif

_Static_assert(CONFIG_BT_ENABLED == 1, "Bluetooth must be enabled: set BT_ENABLED=y in menuconfig");
_Static_assert(CONFIG_BT_NIMBLE_ENABLED == 1, "NimBLE must be enabled: set BT_NIMBLE_ENABLED=y in menuconfig");
_Static_assert(
   CONFIG_ESP_SYSTEM_EVENT_TASK_STACK_SIZE >= 2560,
   "ESP_SYSTEM_EVENT_TASK_STACK_SIZE should be >= 2560: set ESP_SYSTEM_EVENT_TASK_STACK_SIZE>=2560 in menuconfig");

SemaphoreHandle_t edgent_api_mutex;

/* --------------------------------------------------------------------------
 * String getters
 * -------------------------------------------------------------------------- */

const char* blynk_get_firmware_version(void)
{
    return BLYNK_FIRMWARE_VERSION;
}

const char* blynk_get_template_id(void)
{
    return BLYNK_TEMPLATE_ID;
}

const char* blynk_get_template_name(void)
{
    return BLYNK_TEMPLATE_NAME;
}

const char* blynk_get_vendor_prefix(void)
{
    return BLYNK_VENDOR_PREFIX;
}

const char* blynk_get_firmware_type(void)
{
    return BLYNK_FIRMWARE_TYPE;
}

const char* blynk_get_default_server(void)
{
    return BLYNK_DEFAULT_SERVER;
}

/* --------------------------------------------------------------------------
 * Debug level
 * -------------------------------------------------------------------------- */

int blynk_get_debug_level(void)
{
    return CONFIG_BLYNK_DEBUG_LEVEL;
}

/* --------------------------------------------------------------------------
 * Feature flags
 * -------------------------------------------------------------------------- */

bool blynk_use_ethernet(void)
{
#if CONFIG_BLYNK_USE_ETHERNET
    return true;
#else
    return false;
#endif
}

bool blynk_use_cellular(void)
{
#if CONFIG_BLYNK_USE_CELLULAR
    return true;
#else
    return false;
#endif
}

#define BLYNK_PARAM_KV(k, v) k "\0" v "\0"

void fwinfo_init() {
   volatile const char firmwareTag[] =
      "blnkinf\0" BLYNK_PARAM_KV("mcu", BLYNK_FIRMWARE_VERSION) // Primary MCU: firmware version
      BLYNK_PARAM_KV("fw-type", BLYNK_FIRMWARE_TYPE) // Firmware type (usually same as Template ID)
      BLYNK_PARAM_KV("build", __DATE__ " " __TIME__) // Firmware build date and time
      "\0";
   (void)firmwareTag;
}