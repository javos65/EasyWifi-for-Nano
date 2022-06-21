 /*
 * EasyWiFi - for Nano Series
 * Created by John V. - 2022 V 1.2.0
 * 
 * 
 * 
 * 
 * Released into the public domain on github: https://github.com/javos65/EasyWifi-for-Nano
 */
#ifndef EASYWIFI_H
#define EASYWIFI_H

#include "Arduino.h"
#include <WiFiNINA.h>

// Define AP Wifi-Client parameters
#define MAXSSID 10                         // MAX number of SSID's listed after search
#define SSIDBUFFERSIZE 32                  // SSID name BUFFER size
#define APCHANNEL  5                       // AP wifi channel
#define SECRET_SSID "UnKnownWireless"		   // Backup SSID - not required
#define SECRET_PASS "NoPassword"	         // Backup Pass - not required
#define CREDENTIALFILE "/fs/credfile"
#define MAXCONNECT 4                       // Max number of wifi logon connects before opening AP




class EasyWiFi
{
  public:
    EasyWiFi();
    byte start();
    void seed(int value);
    void led(boolean value);
    
  private:                      
  void SimpleDecypher(char * textin, char * textout);
  void SimpleCypher(char * textin, char * textout);
  byte Check_Credentials();
  byte Read_Credentials(char * buf1,char * buf2);
  void listNetworks();
  void printWiFiStatus();

  
};

#endif
