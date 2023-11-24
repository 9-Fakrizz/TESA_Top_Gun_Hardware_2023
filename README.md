# TESA_Top_Gun_Hardware_2023
TESA Top Gun Rally #17 Hardware 2023 ESP32_S3 TSIM Firmware Develop

Hello, This is my repository of TESA TOP GUN RALLY #17 2023
There are 3 missions of the hardware department.
This year we working on Developing ESP32-S3 TSIM firmware.

**using: PlatformIO, Edge Impulse, C-type Data wire

1. Say Ahh Mqtt 
->> Record your voice and continuously publish to MQTT
2. Obj Detection
->> Detectect 2 of logo use TinyML Edge Impulse
3. (FINAL) Staff Gauge Vision Detection 
->> Use ESP32's camera and Edge Impulse to detect water level

About the Problem I found in ESP32-S3:
1. memory leak (It can't handle a big scale of edge impulse TinyML)
