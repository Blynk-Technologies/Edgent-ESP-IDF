#include <stddef.h>
#include <stdbool.h>
#include "sdkconfig.h"
#include "blynk_config.h"

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

const char* blynk_get_device_prefix(void)
{
    return BLYNK_DEVICE_PREFIX;
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

// Small trick for glue weak implementations during linking
void edgent_internal_init(void);
void edgent_init(void){
    edgent_internal_init();
}