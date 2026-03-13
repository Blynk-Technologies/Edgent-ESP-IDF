# Blynk.Edgent Component for ESP-IDF

## Overview

Blynk.Edgent is an ESP-IDF component that connects ESP32 firmware to the Blynk IoT platform. It handles WiFi/Ethernet connectivity, BLE-assisted provisioning, over-the-air firmware updates, and secure MQTT communication. Your firmware gets a cloud backend, and your users get native mobile apps, dashboards, and device management.

## Features

- Secure MQTT connectivity to **Blynk.Cloud** over Wi-Fi
- Handling incoming messages and publishing data to the cloud
- Firmware over-the-air (OTA) updates
- Device ping support
- Handling cloud-initiated device reboot requests
- Device BLE-assisted provisioning

What's next:
- TBD

## How to use

Configure blynk_edgent component using 'idf.py menuconfig':
(Top) → Component config → Blynk.Edgent configuration → Application configuration

- Fill up 'Template ID'
- Fill up 'Template Name'
- Save configuration and exit
- Configure build for target chip, eg 'idf.py set-target esp32'
- Build and flash firmware
- Perfrom device initial provisioning using Blynk mobile app (provisioning service started automatically in the firmware)
- Observe device log via 'idf.py monitor', normally, when device successfully connected you may see the following log:

```
I (326) example: app_main() started
I (326) gpio: GPIO[9]| InputEn: 1| OutputEn: 0| OpenDrain: 0| Pullup: 1| Pulldown: 0| Intr:0 
I (326) button: IoT Button Version: 4.1.3
I (346) nm_eth.c: Ethernet initialized with 0 port(s)
I (346) edgent.c: Blynk.Edgent initialized (device: Blynk Blinking LED-N945)
I (356) edgent.c: State transition: IDLE => CONNECTING_NET
I (356) example: Received on_state_change
I (1506) edgent.c: State transition: CONNECTING_NET => CONNECTING_CLOUD
I (1506) example: Received on_state_change
I (2626) edgent.c: State transition: CONNECTING_CLOUD => RUNNING
I (2626) example: Received on_state_change
I (2626) example: Received on_initial_connection
```

- The device should appear online once all the above steps complete successfully
- For troubleshooting see the device log

The example application also uses the default board button, configured with the following functions:

- **Double click** – start provisioning without resetting the previous configuration, allowing reconfiguration.  
- **Long press** – perform a full configuration reset and start provisioning.

### Ethernet Configuration

To enable Ethernet:

1. Go to: **Top → Component config → Blynk.Edgent configuration**  
   and enable the **"Use Ethernet"** option.

2. Then go to: **Top → Example Ethernet Configuration**  
   and configure the adapter according to type, options, and pins.

> Example: `sdkconfig.T-Internet-POE` file contains a sample configuration for the [LilyGO T-Internet-POE](https://lilygo.cc/products/t-internet-poe) board.

## Documentation

This example consists of `blynk_edgent` component and user's application utilizing this component

- Please refer to `components/blynk_edgent/include/blynk_edgent.h` for API interface reference
- Please refer to `main/main.c` for use case example