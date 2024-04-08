//code to find Mac Add of ESP32
// Youtube video link: https://youtu.be/6O0rVC3zD_I?si=jYY9C4F1B1dIAsAO
#include "WiFi.h"
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  WiFi.mode(WIFI_MODE_STA);
  Serial.println();
  Serial.print("Mac Address : ");
  Serial.println(WiFi.macAddress());
}
void loop() {
  // put your main code here, to run repeatedly:
}

