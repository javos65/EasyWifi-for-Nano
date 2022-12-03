# EasyWifi_Nano
EasyWifi setup for Arduino Nano modules with uBLox Nina (ESP32) wifi, but NO RGB leds (Like MKR series), No AP interface
Fork from EAsyWifi libaries

EasyWifi means Wifi setup without storing your credentials (SSID/Password) in your code.
Based on Wifi-library for NINA uBlox.

This supports the class EasyWiFi
EasyWiFi.start(): starts wifi setup, reading credentials (if available) from flash-disk on the Module and start Wifi login.

SO .... no Credentials in your code :)

Setup your credentials: use the example wifi_storage.ino: set your seed, write your SSID/Pass to Flash
Use the same seed in your code to let EasyWifi read the credentials and login.

Credentials are Cyphered stored for  back-reading protection (simplistic rolling cypher code, but ok)
use .SEED setting to change the Cyphering.


Feel free to add or modify for other Wifi module support, as lonmg as you have a Flash-file system.

V1.1.0 - JayFox 2022
