# code to find Mac Add of ESP32
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Getting the ESP32 MAC_Address
/*
 * - If you want to create a project like in the video, besides following this code, I highly recommend that you also follow the steps as in the video, 
 *   so that you understand the steps better.
 * - To make this project, make sure that your Arduino IDE has an ESP32 board installed in the "Board Manager", if you haven't already,
 *   see here for how to install it : https://www.youtube.com/watch?v=To3TIOFX6cA
 * - If between ESP32 has the same MAC Address or you want to change the MAC Address of ESP32,
 *   see here: https://randomnerdtutorials.com/get-change-esp32-esp8266-mac-address-arduino/
 */

// ======================================== Load libraries
#include "WiFi.h"
// ======================================== 

// ________________________________________________________________________________ VOID SETUP()
void setup() {
  // put your setup code here, to run once:
  
  Serial.begin(115200);
  WiFi.mode(WIFI_MODE_STA);
  Serial.println();
  Serial.print("Mac Address : ");
  Serial.println(WiFi.macAddress());
}
// ________________________________________________________________________________ 

// ________________________________________________________________________________ VOID LOOP()
void loop() {
  // put your main code here, to run repeatedly:
}
// ________________________________________________________________________________ 
//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
