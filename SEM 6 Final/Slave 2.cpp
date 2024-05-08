/*
 * Reference :
 * - ESP32: ESP-NOW Web Server Sensor Dashboard (ESP-NOW + Wi-Fi) : https://randomnerdtutorials.com/esp32-esp-now-wi-fi-web-server/
 * - ESP-NOW with ESP32: Send Data to Multiple Boards (one-to-many) : https://randomnerdtutorials.com/esp-now-one-to-many-esp32-esp8266/
 * - ESP32 Async Web Server – Control Outputs with Arduino IDE (ESPAsyncWebServer library) : https://randomnerdtutorials.com/esp32-async-web-server-espasyncwebserver-library/
 * - and from several other sources.
 */

// ======================================== Including the libraries.
#include <esp_now.h>
#include <esp_wifi.h>
#include <WiFi.h>
// ========================================

// Defines the Wi-Fi channel.
// Configured channel range from 1~13 channels (by default).
// "ESP32 Master" and "ESP32 Slaves" must use the same Wi-Fi channel.
#define CHANNEL 1 

// Defines the Digital Pin of the "On Board LED".
#define ON_Board_LED 2 

// Defines GPIO 13 as LED_1.
#define LED_1 5 
#define LED_2 18 
#define LED_3 19 

// Defines GPIO 12 as LED_2.
#define LED_7 12 

// ======================================== Variables for PWM settings.
const int PWM_freq = 5000;
const int PWM_Channel = 0;
const int PWM_resolution = 8;
// ========================================

// ======================================== Structure example to receive data.
// Must match the sender structure
typedef struct struct_message_receive {
  int receive_GPIO_num;
  int receive_Val;
} struct_message_receive;

// Create a struct_message to receive data.
struct_message_receive receive_Data;
// ========================================

// ________________________________________________________________________________ Callback when data is received.
void OnDataRecv(const uint8_t * mac_addr, const uint8_t *incomingData, int len) {
  digitalWrite(ON_Board_LED, HIGH); //--> Turn on ON_Board_LED.

  // ---------------------------------------- Get the MAC ADDRESS of the sender / slave.
  char macStr[18];
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x", mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  // ---------------------------------------- 

  memcpy(&receive_Data, incomingData, sizeof(receive_Data)); //--> Copy the information in the "incomingData" variable into the "receive_Data" structure variable.

  // ---------------------------------------- Prints the MAC ADDRESS of sender / Master and Bytes received.
  Serial.println();
  Serial.println("<<<<< Receive Data:");
  Serial.print("Packet received from: ");
  Serial.println(macStr);
  Serial.print("Bytes received: ");
  Serial.println(len);
  // ---------------------------------------- 

  // ---------------------------------------- Conditions for controlling the LEDs.
  if (receive_Data.receive_GPIO_num == 5) digitalWrite(LED_1, receive_Data.receive_Val);
  if (receive_Data.receive_GPIO_num == 18) digitalWrite(LED_2, receive_Data.receive_Val);
  if (receive_Data.receive_GPIO_num == 19) digitalWrite(LED_3, receive_Data.receive_Val);
  if (receive_Data.receive_GPIO_num == 12) {
    int pwm = map(receive_Data.receive_Val, 0, 10, 0, 255);
    ledcWrite(PWM_Channel, pwm);  
  }
  // ---------------------------------------- 
  
  // ---------------------------------------- Prints all data received from the sender / Master.
  Serial.println("Receive Data: ");
  Serial.println(receive_Data.receive_GPIO_num);
  Serial.println(receive_Data.receive_Val);
  Serial.println("<<<<<");
  // ---------------------------------------- 

  digitalWrite(ON_Board_LED, LOW); //--> Turn off ON_Board_LED.
}
// ________________________________________________________________________________

// ________________________________________________________________________________ VOID SETUP()
void setup() {
  // put your setup code here, to run once:
  
  Serial.begin(115200);
  Serial.println();

  pinMode(ON_Board_LED,OUTPUT); //--> On Board LED port Direction output
  digitalWrite(ON_Board_LED, LOW); //--> Turn off Led On Board

  pinMode(LED_1,OUTPUT); //--> LED_1 port Direction output
  pinMode(LED_2,OUTPUT); //--> LED_2 port Direction output
  pinMode(LED_3,OUTPUT); //--> LED_3 port Direction output
  pinMode(LED_7,OUTPUT); //--> LED_7 port Direction output

  // ---------------------------------------- PWM settings.
  ledcSetup(PWM_Channel, PWM_freq, PWM_resolution);
  ledcAttachPin(LED_7, PWM_Channel);
  // ----------------------------------------

  // ---------------------------------------- Set Wi-Fi as Wifi Station Mode.
  WiFi.mode(WIFI_STA);
  // ----------------------------------------

  // ---------------------------------------- Set the Wi-Fi channel.
  // "ESP32 Master" and "ESP32 Slaves" must use the same Wi-Fi channel.
  
  int cur_WIFIchannel = WiFi.channel();

  if (cur_WIFIchannel != CHANNEL) {
    //WiFi.printDiag(Serial); // Uncomment to verify channel number before
    esp_wifi_set_promiscuous(true);
    esp_wifi_set_channel(CHANNEL, WIFI_SECOND_CHAN_NONE);
    esp_wifi_set_promiscuous(false);
    //WiFi.printDiag(Serial); // Uncomment to verify channel change after
  }

  Serial.println("-------------");
  Serial.print("Wi-Fi channel : ");
  Serial.println(WiFi.channel());
  Serial.println("-------------");
  // ---------------------------------------- 

  // ---------------------------------------- Init ESP-NOW
  Serial.println("-------------");
  Serial.println("Start initializing ESP-NOW...");
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    Serial.println("Restart ESP32...");
    Serial.println("-------------");
    delay(1000);
    ESP.restart();
  }
  Serial.println("Initializing ESP-NOW was successful.");
  Serial.println("-------------");
  // ---------------------------------------- 

  // ---------------------------------------- Register for a callback function that will be called when data is received
  Serial.println();
  Serial.println("Register for a callback function that will be called when data is received");
  esp_now_register_recv_cb(OnDataRecv);
  // ----------------------------------------
}
// ________________________________________________________________________________

// ________________________________________________________________________________ VOID LOOP()
void loop() {
  // put your main code here, to run repeatedly:
  
}
// ________________________________________________________________________________
//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
