//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> ESP32 ESP-NOW Web Server Master Sender AP Mode
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
#include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>

// ========================================

// Include the contents of the User Interface Web page, stored in the same folder as the .ino file
#include "PageIndex.h" 

// Defines the Wi-Fi channel.
// Configured channel range from 1~13 channels (by default).
// "ESP32 Master" and "ESP32 Slaves" must use the same Wi-Fi channel.
#define CHANNEL 1 

// Defines the Digital Pin of the "On Board LED".
#define ON_Board_LED 2  

// ======================================== Access Point Declaration and Configuration.
const char* ssid = "A";  //--> access point name
const char* password = "aditheking"; //--> access point password

IPAddress local_ip(192,168,251,233);
IPAddress gateway(192,168,251,233);
IPAddress subnet(255,255,255,0);
// ======================================== 

// ======================================== REPLACE WITH THE MAC ADDRESS OF YOUR SLAVES / RECEIVERS / ESP32 RECEIVERS.
uint8_t broadcastAddressESP32Slave1[] = {0x80, 0x7D, 0x3A, 0xEF, 0xA4, 0xD8}; // Slave 1 MAC Address
uint8_t broadcastAddressESP32Slave2[] = {0x30, 0xAE, 0xA4, 0xEF, 0xC8, 0xD8}; // Slave 2 MAC Address
// ========================================

// ======================================== The variables used to check the parameters passed in the URL.
// Look in the "PageIndex.h" file.
// "set_LED?board="+board+"&gpio_output="+gpio+"&val="+value
// For example :
// set_LED?board=ESP32Slave1&gpio_output=13&val=1
// PARAM_INPUT_1 = ESP32Slave1
// PARAM_INPUT_2 = 13
// PARAM_INPUT_3 = 1
const char* PARAM_INPUT_1 = "board";
const char* PARAM_INPUT_2 = "gpio_output";
const char* PARAM_INPUT_3 = "val";
// ======================================== 

// ======================================== Structure example to send data
// Must match the receiver structure
typedef struct struct_message_send {
  int send_GPIO_num;
  int send_Val;
} struct_message_send;

struct_message_send send_Data; //--> Create a struct_message to send data.
// ======================================== 

// Create a variable of type "esp_now_peer_info_t" to store information about the peer.
esp_now_peer_info_t peerInfo;

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

// ________________________________________________________________________________ Callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  digitalWrite(ON_Board_LED, HIGH); //--> Turn on ON_Board_LED.
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
  Serial.println(">>>>>");
  digitalWrite(ON_Board_LED, LOW); //--> Turn off ON_Board_LED.
}
// ________________________________________________________________________________

// ________________________________________________________________________________ Subroutine to prepare data and send it to the Slaves.
void send_data_to_slave(int slave_number, int gpio_number, int value) {
  Serial.println();
  Serial.print(">>>>> ");
  Serial.println("Send data");

  send_Data.send_GPIO_num = gpio_number;
  send_Data.send_Val = value;

  esp_err_t result;

  // ::::::::::::::::: Sending data to Slave 1
  if (slave_number == 1) {
    result = esp_now_send(broadcastAddressESP32Slave1, (uint8_t *) &send_Data, sizeof(send_Data));
  }
  // :::::::::::::::::

  // ::::::::::::::::: Sending data to Slave 2
  if (slave_number == 2) {
    result = esp_now_send(broadcastAddressESP32Slave2, (uint8_t *) &send_Data, sizeof(send_Data));
  }
  // :::::::::::::::::
  
  if (result == ESP_OK) {
    Serial.println("Sent with success");
  }
  else {
    Serial.println("Error sending the data");
  }
}
// ________________________________________________________________________________

// ________________________________________________________________________________ VOID SETUP()
void setup() {
  // put your setup code here, to run once:
  
  Serial.begin(115200);
  Serial.println();

  pinMode(ON_Board_LED,OUTPUT); //--> On Board LED port Direction output
  digitalWrite(ON_Board_LED, LOW); //--> Turn off Led On Board

  // ---------------------------------------- Set Wi-Fi as Access Point and Wifi Station.
  WiFi.mode(WIFI_AP_STA);
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
  // ---------------------------------------- 

  // ---------------------------------------- Setting up ESP32 to be an Access Point.
  Serial.println();
  Serial.println("-------------");
  Serial.println("Setting up ESP32 to be an Access Point.");
  WiFi.softAP(ssid, password); //--> Creating Access Points
  delay(1000);
  Serial.println("Setting up ESP32 softAPConfig.");
  WiFi.softAPConfig(local_ip, gateway, subnet);
  Serial.println("-------------");
  //----------------------------------------

  // ---------------------------------------- Init ESP-NOW
  Serial.println();
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

  // ---------------------------------------- Once ESPNow is successfully Init, we will register for Send CB to get the status of Trasnmitted packet
  Serial.println();
  Serial.println("get the status of Trasnmitted packet");
  esp_now_register_send_cb(OnDataSent);
  // ---------------------------------------- 

  // ---------------------------------------- Register Peer
  Serial.println();
  Serial.println("-------------");
  Serial.println("Register peer");
  peerInfo.encrypt = false;
  // ::::::::::::::::: register first peer
  Serial.println("Register first peer (ESP32 Slave 1)");
  memcpy(peerInfo.peer_addr, broadcastAddressESP32Slave1, 6);
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add first peer");
    return;
  }
  // :::::::::::::::::
  // ::::::::::::::::: register second peer
  Serial.println("Register second peer (ESP32 Slave 2)");
  memcpy(peerInfo.peer_addr, broadcastAddressESP32Slave2, 6);
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add second peer");
    return;
  }
  // :::::::::::::::::
  Serial.println("-------------");
  // ---------------------------------------- 

  // ---------------------------------------- Handle Web Server
  Serial.println();
  Serial.println("Setting Up the Main Page on the Server.");
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", MAIN_page);
  });
  // ----------------------------------------

  // ---------------------------------------- Send a GET request to <ESP_IP>/set_LED?board=<inputMessage1>&gpio_output=<inputMessage2>&val=<inputMessage3>
  server.on("/set_LED", HTTP_GET, [] (AsyncWebServerRequest *request) {
    // :::::::::::::::::
    // GET input value on <ESP_IP>/set_LED?board=<inputMessage1>&gpio_output=<inputMessage2>&val=<inputMessage3>
    // PARAM_INPUT_1 = inputMessage1
    // PARAM_INPUT_2 = inputMessage2
    // PARAM_INPUT_3 = inputMessage3
    // Board = PARAM_INPUT_1
    // LED_gpio_num = PARAM_INPUT_2
    // LED_val = PARAM_INPUT_3
    // :::::::::::::::::

    String Board;
    String LED_gpio_num;
    String LED_val;
    
    if (request->hasParam(PARAM_INPUT_1) && request->hasParam(PARAM_INPUT_2) && request->hasParam(PARAM_INPUT_3)) {
      Board = request->getParam(PARAM_INPUT_1)->value();
      LED_gpio_num = request->getParam(PARAM_INPUT_2)->value();
      LED_val = request->getParam(PARAM_INPUT_3)->value();

      String Rslt = "Board : " + Board + " || GPIO : " + LED_gpio_num + " || Set to :" + LED_val;
      Serial.println();
      Serial.println(Rslt);
      // Conditions for sending data to Slave 1.
      if (Board == "ESP32Slave1") send_data_to_slave(1, LED_gpio_num.toInt(), LED_val.toInt());
      // Conditions for sending data to Slave 2.
      if (Board == "ESP32Slave2") send_data_to_slave(2, LED_gpio_num.toInt(), LED_val.toInt());
    }
    else {
      Board = "No message sent";
      LED_gpio_num = "No message sent";
      LED_val = "No message sent";
    }
    request->send(200, "text/plain", "OK");
  });
  // ---------------------------------------- 
  
  Serial.println();
  Serial.println("Starting the Server.");
  server.begin();

  Serial.println();
  Serial.println("------------");
  Serial.print("SSID name : ");
  Serial.println(ssid);
  Serial.print("IP address : ");
  Serial.println(WiFi.softAPIP());
  Serial.print("Wi-Fi channel : ");
  Serial.println(WiFi.channel());
  Serial.println();
  Serial.println("Connect your computer or mobile Wifi to the SSID above.");
  Serial.println("Visit the IP Address above in your browser to open the main page.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println("------------");
  Serial.println();
}
// ________________________________________________________________________________

// ________________________________________________________________________________ VOID LOOP()
void loop() {
  // put your main code here, to run repeatedly:

}
// ________________________________________________________________________________
//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
