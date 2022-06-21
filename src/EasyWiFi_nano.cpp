 /*
 * EasyWiFi Nano 
 * Created by John V. - 2021 V 1.1.0
 * 
 *  RGB LED INDICATOR on uBlox nina Module
 *  GREEN: Connected
 *  
 *  BLUE: (Stored) Credentials found, connecting        
 *  RED: No Stored Credentials found, exit error    
 * 
 *  
 * 
 * Released into the public domain on github: https://github.com/javos65/EasyWifi-for-Nano
 */
 
#include "EasyWiFi_nano.h"

#define DBGON   1  // Debug option  -serial print

char G_SSIDList[MAXSSID][SSIDBUFFERSIZE];                     // Store of available SSID's
int G_ssidCounter = 0;                //Gloabl counter for number of foubnd SSID's
char G_ssid[32] = SECRET_SSID;        // optional init: your network SSID (name) 
char G_pass[32] = SECRET_PASS;        // optional init: your network password 
int SEED=4;
boolean G_ledon=1; // leds on or of

// ***************************************


EasyWiFi::EasyWiFi()
{
}

// Login to local network  //
byte EasyWiFi::start(){
int  noconnect=0;
WiFi.disconnect();delay(2000);
int G_Wifistatus = WiFi.status();
if ( ( G_Wifistatus != WL_CONNECTED) || (WiFi.RSSI() <= -90) ||(WiFi.RSSI() ==0) ) { // check if connected
// Read SSId File
  if (Read_Credentials(G_ssid,G_pass)==0) {  // read credentials, if not possible, re-use the old-already loaded credentials
#ifdef DBGON
      Serial.println("* Using old credentials");
#endif
  } 
  noconnect=0;
  while ( (G_Wifistatus != WL_CONNECTED) || (WiFi.RSSI() <= -90) || (WiFi.RSSI() ==0) ) {   // attempt to connect to WiFi network 3 times
#ifdef DBGON
          Serial.print("* Attempt#");Serial.print(noconnect);Serial.print(" to connect to Network: ");Serial.println(G_ssid);                // print the network name (SSID);
#endif
           G_Wifistatus = WiFi.begin(G_ssid, G_pass);     // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
           delay(2000);                                   // wait 2 seconds for connection:
           noconnect++;                                   // try-counter
      if ( G_Wifistatus == WL_CONNECTED ) {    
#ifdef DBGON
      printWiFiStatus();                        // you're connected now, so print out the status anmd break while loop
#endif
      break;
      }
      else 
      {
        if (noconnect >= MAXCONNECT ){
#ifdef DBGON
      Serial.println("* Connection not possible after several retries, potential networks :");                   
#endif
        listNetworks();                         // load avaialble networks in a list
#ifdef DBGON
      Serial.println("* Error: check Credential storage, use Wifi_storage.ino,\n* exiting now...\n");                   
#endif        
        WiFi.end(); 
        WiFi.disconnect();
        break;
      }
    }
  } // end if not connected
  if ( G_Wifistatus == WL_CONNECTED ) return(1);
    else return(0);
 }
else {
#ifdef DBGON
    Serial.println("* Already connected.");                     // you're already connected
    printWiFiStatus(); 
#endif
    return(1);
    }
}

// SERIALPRINT Wifi Status - only for debug
void EasyWiFi::printWiFiStatus() {
#ifdef DBGON
    // print the SSID of the network you're attached to:
    Serial.print("* SSID: "); Serial.print(WiFi.SSID());
    // print your WiFi shield's IP address:
    IPAddress ip = WiFi.localIP(); Serial.print(" - IP Address: "); Serial.print(ip);
    // print your WiFi gateway:
    IPAddress ip2 = WiFi.gatewayIP(); Serial.print(" - IP Gateway: ");Serial.print(ip2);    
    // print the received signal strength:
    long rssi = WiFi.RSSI(); Serial.print("- Rssi: "); Serial.print(rssi); Serial.println(" dBm");
#endif
}


// Set Seed of the Cypher, should be positive
void EasyWiFi::seed(int value) {
  if (value >= 0) SEED=value;
}

// Scan for available Wifi Networks and place is Glovbal SSIDList
void EasyWiFi::listNetworks() {
int t;
String tmp;
    // scan for nearby networks:
    int numSsid = WiFi.scanNetworks();
    if (numSsid == -1)
    {
#ifdef DBGON        
      Serial.println("* Couldn't get a Wifi List");
#endif
    }
    else {
#ifdef DBGON    
    Serial.print("* Found total ");Serial.print(numSsid);Serial.println(" Networks.");
#endif      
    G_ssidCounter = 0;
    // print the network number and name for each network found:
    for (int thisNet = 0; thisNet < numSsid; thisNet++) {
        tmp =  WiFi.SSID(thisNet);
        if(G_ssidCounter < MAXSSID ) { // store only maximum of <SSIDMAX> SSDI's with high dB > -80        && WiFi.RSSI(thisNet)>-81
          for(t=0;t<tmp.length();++t)  G_SSIDList[G_ssidCounter][t] = tmp[t];
          G_SSIDList[G_ssidCounter][t]=0;
#ifdef DBGON           
          Serial.print(G_ssidCounter);
          Serial.print(". ");
          Serial.print(G_SSIDList[G_ssidCounter]);
          Serial.print("\t\tSignal: ");
          Serial.print(WiFi.RSSI(thisNet));
          Serial.println(" dBm");
          Serial.flush();
#endif        
          G_ssidCounter = G_ssidCounter+1; 
          }
        } // end for list loop
  }
}





/********* File Routines **************/

/* Read credentials ID,pass to Flash file , Comma separated style*/
byte EasyWiFi::Read_Credentials(char * buf1,char * buf2)
{
  int u,t,c=0;
  char buf[128],comma=30, zero=0;
  char bufc[128];
  WiFiStorageFile file = WiFiStorage.open(CREDENTIALFILE);
  if (file) {
    file.seek(0);
    if (file.available()) {  // read file buffer into memory, max size is 64 bytes for 2 char-strings
      c= file.read(buf, 128);  //Serial.write(buf, c);
    }
    if (c!=0)
    {
      t=0;u=0;
      while(buf[t] != comma) {  // read ID till comma
        bufc[u++]=buf[t++];
        if (u>31) break;
        }
        bufc[u]=0;
        SimpleDecypher(bufc,buf1);
        u=0;t++;                // move to second part: pass
      while(buf[t] != zero) {   // read till zero
        bufc[u++]=buf[t++];
        if (u>31)  break;
        }
        bufc[u]=0;
        SimpleDecypher(bufc,buf2);
    }
#ifdef DBGON
   Serial.print("* Read Credentials : ");Serial.println(c);
#endif    
   file.close(); return(c);
 }
 else {
#ifdef DBGON
   Serial.println("* Cant read Credentials :");
#endif    
  file.close();return(0);
 }
}


/* Check credentials file */
byte EasyWiFi::Check_Credentials()
{
  WiFiStorageFile file = WiFiStorage.open(CREDENTIALFILE);
  if (file) {
#ifdef DBGON
 Serial.println("* Found Credentialsfile : ");
#endif  
  file.close(); return(1);
 }
 else {
  #ifdef DBGON
 Serial.println("* Could not find Credentialsfile : ");
#endif  
  file.close(); return(0);
 }
}


/* Simple Cyphering the text code */
void EasyWiFi::SimpleCypher(char * textin, char * textout)
{
int c,t=0;
while(textin[t]!=0) {
   textout[t]=textin[t]+SEED%17-t%7;
   t++;
  }
  textout[t]=0;
#ifdef DBGON
// Serial.print("* Cyphered ");Serial.print(t);Serial.print(" - ");Serial.println(textout);
#endif
}

/* Simple DeCyphering the text code */
void EasyWiFi::SimpleDecypher(char * textin, char * textout)
{
int c,t=0;
while(textin[t]!=0) {
   textout[t]=textin[t]-SEED%17+t%7;
   t++;
  }
  textout[t]=0;
#ifdef DBGON
// Serial.print("* Decyphered ");Serial.print(t);Serial.print(" - ");Serial.println(textout);
#endif
}




