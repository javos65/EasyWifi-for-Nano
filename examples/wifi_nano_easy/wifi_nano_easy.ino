/*
Easy WiFi - build for Nano series / WIFININA using the uBlox NINA-W102 module Library

THIS PROGERAM SETUP WIFI BY USING 
- Load pre-stored Credentials (SSID/Password) stored on module (Cyphered)
- Store the pass info by the other script : wifi_storage.ino
- setup multiple attemps to setup your wifi connection
- if fails, Returns fault and RGB led will be red


(C) jAY fOX 2021 / 
*************************************************************/
#include <WiFiNINA.h>
#include <SPI.h>
#include <EasyWiFi_nano.h>

/*********** Global Settings  **********/
EasyWiFi MyEasyWiFi;


//
// Setup / initialisation 
//
void setup()
{ 
/*********** Serial SETUP  **********/
int t=10;  //Initialize serial and wait for port to open, max 10 second waiting
Serial.begin(115200);
while (!Serial) {
    delay(1000);
    if ( (t--)== 0 ) break; // no serial, but continue program
    }
  
/*********** Check WifiShield  **********/
if (WiFi.status() == WL_NO_SHIELD) {   // check for the presence of the shield:
    Serial.println("WiFi shield not present");
    while (true);     // don't continue if no shield
    }
MyEasyWiFi.seed(0);  // Seed for Credentials storage
} // endSetup



//
// Main SUPER Loop
//
void loop()
{
  if (WiFi.status()==WL_CONNECTED)
  {
    printWiFiStatus();
    delay(5000);
  }
  else
  {
    Serial.println("* Not Connected, starting EasyWiFi");
    if (!MyEasyWiFi.start()) while(1){};     // Start Wifi login 
  }

} // end Main loop



// SERIALPRINT Wifi Status 
void printWiFiStatus() {

    // print the SSID of the network you're attached to:
    Serial.print("\nStatus: SSID: "); Serial.print(WiFi.SSID());
    // print your WiFi shield's IP address:
    IPAddress ip = WiFi.localIP(); Serial.print(" - IPAddress: "); Serial.print(ip);
    // print the received signal strength:
    long rssi = WiFi.RSSI(); Serial.print("- Rssi: "); Serial.print(rssi); Serial.println("dBm");

}
