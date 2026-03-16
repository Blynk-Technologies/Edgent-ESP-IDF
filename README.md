# Blynk.Edgent for ESP-IDF

**Blynk.Edgent** is a packaged solution that allows **ESP-IDF** developers to easily connect their devices to the Blynk IoT platform
and take advantage of all its advanced features without the need for extensive coding. 

![image](https://github.com/blynkkk/blynkkk.github.io/raw/master/images/GithubBanner.jpg?raw=1)

## Features

- **Blynk.Inject**: connect your devices easily using [**Blynk IoT App**][blynk-apps] (iOS and Android)
  - `BLE`-assisted device provisioning for the best end-user experience
  - `WiFiAP`-based provisioning for devices without BLE support - *(soon)*
- **Network Manager**: Advanced network connection management and troubleshooting
  - `WiFi`: Maintains connection to the most reliable WiFi network (up to 16 configured networks)
  - `Ethernet`: Supports `Static IP` or `DHCP` network configuration
  <!--
  - `Cellular`: Provides connectivity through `2G GSM`, `EDGE`, `3G`, `4G LTE`, `Cat M1`, or `5G` networks using `PPP`
  -->
- Secure **Blynk.Cloud** MQTT connection that provides simple API for:
  - Data transfer with `DataStreams`, reporting `Events`, and accessing `Metadata`
- **Blynk.Air** - automatic, managed Over The Air firmware updates using Web Dashboard
  - Direct firmware upgrade using iOS/Android App before device activation - *(soon)*

Supported targets:
```
✅ ESP32, ESP32-S3, ESP32-C2, ESP32-C3, ESP32-C5, ESP32-C6
⏳ ESP32-S2 - *(soon)*
❌ ESP32-P4, ESP32-H4
```

## Getting Started

- Sign up/Log in to your [Blynk Account](https://blynk.cloud)
- Install **Blynk IoT App** for [iOS][blynk-ios] or [Android][blynk-android]
- Create a new Product Template. This will provide you with `Template ID` and `Template Name`.

## 1. Install

Configure the target chip, eg `idf.py set-target esp32`.

Configure component using `idf.py menuconfig`:
```
(Top) → Component config → Blynk.Edgent
```

- Set `Template ID`
- Set `Template Name`
- Save configuration and exit
- Build and flash firmware


## 2. Connect your device to Blynk.Cloud

1. Open **Blynk IoT App** on your smartphone
2. Click **Add device** -> **Find devices nearby**
3. Select your device and follow the wizard instructions

The device should appear `online` once the above steps complete successfully.

<details><summary><b>Expected device log</b></summary>

See the device log via `idf.py monitor`:

```log
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

</details>

<!--
> [!NOTE]
> If you have already created your device in Blynk,
> you can [connect it manually using REPL](_extra/Cookbook.md#manual-device-connection)
-->

The example application also handles the on-board button:

- **Double click** – start provisioning without resetting the previous configuration, allowing reconfiguration.  
- **Long press** – perform a full configuration reset and start provisioning.

## Adding to existing project

```sh
idf.py add-dependency "blynk/edgent^1.0.0"
```

If your project uses **Minimal Build**, consider disabling it, or add build dependencies explicitly:
```
esp_event esp_netif esp_wifi esp_https_ota
esp_partition efuse nvs_flash mqtt json bt
esp_eth
```

## OTA configuration

Minimum flash size requirement for OTA updates is `4MB`.

You should select a dual-bank partition table, for example:
```
(Top) → Partition Table → Partition Table → Two large size OTA partitions
```

## Ethernet configuration

To enable Ethernet:

1. Go to: **Top → Component config → Blynk.Edgent**  
   and enable the **"Use Ethernet"** option.

2. Then go to: **Top → Example Ethernet Configuration**  
   and configure the adapter according to type, options, and pins.

> Example: `sdkconfig.T-Internet-POE` file contains a sample configuration for the [LilyGO T-Internet-POE](https://lilygo.cc/products/t-internet-poe) board.

# Further reading

<!--
- [Cookbook](_extra/Cookbook.md)
-->
- [Blynk.Edgent](https://docs.blynk.io/en/blynk.edgent/overview)
- [Blynk IoT Apps][blynk-apps]
- [Deploying Products With Dynamic AuthTokens](https://docs.blynk.io/en/commercial-use/deploying-products-with-dynamic-authtokens)
- [Blynk MQTT API](https://docs.blynk.io/en/blynk.cloud-mqtt-api/device-mqtt-api)
- [Blynk Network Co-Processor (NCP)](https://docs.blynk.io/en/blynk.ncp/overview)


[blynk-apps]: https://docs.blynk.io/en/downloads/blynk-apps-for-ios-and-android
[blynk-android]: https://play.google.com/store/apps/details?id=cloud.blynk
[blynk-ios]: https://apps.apple.com/us/app/blynk-iot/id1559317868
[blynk_sales]: https://blynk.io/en/contact-us-business
