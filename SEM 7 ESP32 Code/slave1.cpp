#include <esp_now.h>
#include <esp_wifi.h>
#include <WiFi.h>
#include <driver/ledc.h>  

#define CHANNEL 7
#define ON_Board_LED 2
#define LED_1 5
#define LED_2 18
#define LED_3 19
#define LED_7 12

// PWM Channel configuration
#define PWM_CHANNEL 0

// Master's MAC Address - update with your master's actual MAC
uint8_t masterAddress[] = {0x78, 0xE3, 0x6D, 0x09, 0xBC, 0x5C}; // Replace with actual master MAC

const int PWM_freq = 5000;
const int PWM_resolution = 8;
const int max_duty = (1 << PWM_resolution) - 1;

typedef struct struct_message_receive {
    int receive_GPIO_num;
    int receive_Val;
} struct_message_receive;

struct_message_receive receive_Data;

void OnDataRecv(const esp_now_recv_info_t *esp_now_info, const uint8_t *incomingData, int len) {
    digitalWrite(ON_Board_LED, HIGH);
    
    if (len == sizeof(struct_message_receive)) {
        memcpy(&receive_Data, incomingData, sizeof(receive_Data));
        
        char macStr[18];
        snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
                esp_now_info->src_addr[0], esp_now_info->src_addr[1],
                esp_now_info->src_addr[2], esp_now_info->src_addr[3],
                esp_now_info->src_addr[4], esp_now_info->src_addr[5]);
        
        Serial.printf("\nReceived from: %s\n", macStr);
        Serial.printf("GPIO: %d, Value: %d\n", 
                     receive_Data.receive_GPIO_num, 
                     receive_Data.receive_Val);

        // Control outputs using standard Arduino functions
        switch(receive_Data.receive_GPIO_num) {
            case 5:
                digitalWrite(LED_1, receive_Data.receive_Val);
                break;
            case 18:
                digitalWrite(LED_2, receive_Data.receive_Val);
                break;
            case 19:
                digitalWrite(LED_3, receive_Data.receive_Val);
                break;
            case 12:
                int pwm = map(receive_Data.receive_Val, 0, 1, 0, 255);
                ledcWrite(PWM_CHANNEL, pwm);
                break;
        }
      }
    
    digitalWrite(ON_Board_LED, LOW);
}

void setup() {
    Serial.begin(115200);
    delay(500); // Small delay to ensure serial is ready
    
    // Configure pins
    pinMode(ON_Board_LED, OUTPUT);
    pinMode(LED_1, OUTPUT);
    pinMode(LED_2, OUTPUT);
    pinMode(LED_3, OUTPUT);
    digitalWrite(ON_Board_LED, LOW);
    
    // Configure PWM using ledcSetup and ledcAttachPin
    ledcAttach(LED_7, PWM_freq, PWM_resolution);
    
    // Force WiFi mode and MAC
    WiFi.mode(WIFI_STA);
    
    // Set WiFi channel
    esp_wifi_set_channel(CHANNEL, WIFI_SECOND_CHAN_NONE);
    Serial.printf("Current Wi-Fi channel: %d\n", WiFi.channel());
    
    // Print MAC address
    Serial.print("MAC Address: ");
    Serial.println(WiFi.macAddress());
    
    // Initialize ESP-NOW
    if (esp_now_init() != ESP_OK) {
        Serial.println("ESP-NOW init failed");
        ESP.restart();
    }
    
    // Register peer (master)
    esp_now_peer_info_t peerInfo = {};
    memcpy(peerInfo.peer_addr, masterAddress, 6);
    peerInfo.channel = CHANNEL;
    peerInfo.encrypt = false;
    
    if (esp_now_add_peer(&peerInfo) != ESP_OK) {
        Serial.println("Failed to add peer");
    } else {
        Serial.println("Master peer added successfully");
    }
    
    // Register callback
    esp_now_register_recv_cb(OnDataRecv);
    
    Serial.println("Slave initialized successfully");
}

void loop() {
    // Empty loop
}
