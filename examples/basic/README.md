# Blynk Edgent ESP-IDF Example

This example demonstrates how to integrate **Blynk Edgent** with **ESP-IDF**, quickly provision and connect device to **Blynk Cloud**.

The application shows how to:

- Initialize **Blynk Edgent**
- Handle device **state change events**
- Receive **downlink messages**
- Send **datastream updates**
- Use a **boot button** for configuration and reset

---

# Supported Targets

| Target | Boot Button GPIO |
|------|------|
| ESP32 | GPIO0 |
| ESP32-S3 | GPIO0 |
| ESP32-C2 | GPIO9 |
| ESP32-C3 | GPIO9 |
| ESP32-C5 | GPIO9 |
| ESP32-C6 | GPIO9 |

---

# Button Actions

The boot button is used to control device provisioning.

| Action | Result |
|------|------|
| Double click | Start configuration mode |
| Long press (5 seconds) | Reset device configuration |

---

# Requirements

- **ESP-IDF ≥ 5.1**
- Blynk Cloud account
- Device Template created in Blynk Console

ESP-IDF installation guide:

https://docs.espressif.com/projects/esp-idf

---

## Configure Blynk Template

Set your Blynk template information using **menuconfig** under **Component config → Blynk.Edgent**.