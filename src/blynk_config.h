/*
 * Copyright (c) 2025 Blynk Technologies Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef BLYNK_CONFIG_H
#define BLYNK_CONFIG_H

#define BLYNK_FIRMWARE_VERSION CONFIG_BLYNK_FIRMWARE_VERSION
#define BLYNK_TEMPLATE_ID      CONFIG_BLYNK_TEMPLATE_ID
#define BLYNK_TEMPLATE_NAME    CONFIG_BLYNK_TEMPLATE_NAME
#define BLYNK_DEVICE_PREFIX    CONFIG_BLYNK_DEVICE_PREFIX
#define BLYNK_DEFAULT_SERVER   CONFIG_BLYNK_DEFAULT_SERVER

_Static_assert(sizeof(BLYNK_FIRMWARE_VERSION) > 1, "BLYNK_FIRMWARE_VERSION must not be empty!");
_Static_assert(sizeof(BLYNK_TEMPLATE_ID) > 1, "BLYNK_TEMPLATE_ID must not be empty!");
_Static_assert(sizeof(BLYNK_TEMPLATE_NAME) > 1, "BLYNK_TEMPLATE_NAME must not be empty!");
_Static_assert(sizeof(BLYNK_DEVICE_PREFIX) > 1, "BLYNK_DEVICE_PREFIX must not be empty!");

#if CONFIG_BLYNK_FIRMWARE_TYPE_SET
#   define BLYNK_FIRMWARE_TYPE CONFIG_BLYNK_FIRMWARE_TYPE
#else
#   define BLYNK_FIRMWARE_TYPE BLYNK_TEMPLATE_ID
#endif

#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)

#if !CONFIG_BT_ENABLED
#error "Bluetooth must be enabled: set BT_ENABLED=y in menuconfig"
#endif

#if !CONFIG_BT_NIMBLE_ENABLED
#error "NimBLE must be enabled: set BT_NIMBLE_ENABLED=y in menuconfig"
#endif

#if CONFIG_ESP_SYSTEM_EVENT_TASK_STACK_SIZE < 2560
#error "ESP_SYSTEM_EVENT_TASK_STACK_SIZE should be >= 2560 (current: " STR(CONFIG_ESP_SYSTEM_EVENT_TASK_STACK_SIZE) ")"
#endif

#endif //BLYNK_CONFIG_H