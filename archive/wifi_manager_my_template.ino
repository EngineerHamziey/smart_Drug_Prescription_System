//beebee jump s1 battery = 180k

#if 0

#include <WiFiManager.h> // https://github.com/tzapu/WiFiManager


void setup() {
  
     WiFi.mode(WIFI_STA); // explicitly set mode, esp defaults to STA+AP
    // it is a good practice to make sure your code sets wifi mode how you want it.

    beginTheSerialMonitor();
    
    //WiFiManager, Local intialization. Once its business is done, there is no need to keep it around
    WiFiManager myWifiManager;//creating object to represent our wifi manager

    // reset settings - wipe stored credentials for testing
    // these are stored by the esp library
     myWifiManager.resetSettings();//this will prevent it from automatically reconnecting to the previous wifi
     // you can use a button to make this fun.

    // Automatically connect using saved credentials,
    // if connection fails, it starts an access point with the specified name ( "AutoConnectAP"),
    // if empty will auto generate SSID, if password is blank it will be anonymous AP (wm.autoConnect())
    // then goes into a blocking loop awaiting configuration and will return success result

    bool res;
    // res = wm.autoConnect(); // auto generated AP name from chipid
    // res = wm.autoConnect("AutoConnectAP"); // anonymous ap
    res = myWifiManager.autoConnect("Smart Power Meter","password"); // password protected app

    if(!res) {
        Serial.println("Failed to connect");
        // ESP.restart();
    } 
    else {
        //if you get here you have connected to the WiFi  
        Serial.println("connected...yeey :)");
    }

}

void loop() {
    // put your main code here, to run repeatedly:   
}

#endif
