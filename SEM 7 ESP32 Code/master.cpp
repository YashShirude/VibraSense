#include <esp_now.h>
#include <WiFi.h>
#include <WebSocketsClient.h>
#include <ArduinoJson.h>
#include <esp_wifi.h>

// WiFi credentials
const char* ssid = "Yash";
const char* password = "Easports28";

// WebSocket server details
const char* websocket_server = "192.168.19.238";
const int websocket_port = 8080;
const char* websocket_path = "/ws";

WebSocketsClient webSocket;
int myInts[6]; 
int myPins[] = {5, 18, 19}; 
// Define the Braille alphabet structure
typedef struct {
    char letter;
    int dots[6];
} BrailleChar;

// Braille alphabet mapping (remains the same)
BrailleChar brailleAlphabet[] = {
    {'A', {1, 0, 0, 0, 0, 0}},
    {'B', {1, 1, 0, 0, 0, 0}},
    {'C', {1, 0, 0, 1, 0, 0}},
    {'D', {1, 0, 0, 1, 1, 0}},
    {'E', {1, 0, 0, 0, 1, 0}},
    {'F', {1, 1, 0, 1, 0, 0}},
    {'G', {1, 1, 0, 1, 1, 0}},
    {'H', {1, 1, 0, 0, 1, 0}},
    {'I', {0, 1, 0, 1, 0, 0}},
    {'J', {0, 1, 0, 1, 1, 0}},
    {'K', {1, 0, 1, 0, 0, 0}},
    {'L', {1, 1, 1, 0, 0, 0}},
    {'M', {1, 0, 1, 1, 0, 0}},
    {'N', {1, 0, 1, 1, 1, 0}},
    {'O', {1, 0, 1, 0, 1, 0}},
    {'P', {1, 1, 1, 1, 0, 0}},
    {'Q', {1, 1, 1, 1, 1, 0}},
    {'R', {1, 1, 1, 0, 1, 0}},
    {'S', {0, 1, 1, 1, 0, 0}},
    {'T', {0, 1, 1, 1, 1, 0}},
    {'U', {1, 0, 1, 0, 0, 1}},
    {'V', {1, 1, 1, 0, 0, 1}},
    {'W', {0, 1, 0, 1, 1, 1}},
    {'X', {1, 0, 1, 1, 0, 1}},
    {'Y', {1, 0, 1, 1, 1, 1}},
    {'Z', {1, 0, 1, 0, 1, 1}},
    {'.', {0, 0, 1, 1, 0, 0}},
    {' ', {0, 0, 0, 0, 0, 0}}
};

// Structure for ESP-NOW communication
typedef struct {
    int receive_GPIO_num;
    int receive_Val;
} struct_message_send;

// MAC addresses of slave devices
uint8_t slave1_mac[] = {0x80, 0x7D, 0x3A, 0xEF, 0xA4, 0xD8};
uint8_t slave2_mac[] = {0x30, 0xAE, 0xA4, 0xEF, 0xC8, 0xD8};

bool isWiFiConnected = false;
bool isWebSocketConnected = false;
unsigned long lastReconnectAttempt = 0;
const unsigned long RECONNECT_INTERVAL = 5000;

// ESP-NOW callback function (remains the same)
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
    char macStr[18];
    snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
             mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
    Serial.printf("Last Packet Sent to: %s, Status: %s\n", 
                 macStr, 
                 status == ESP_NOW_SEND_SUCCESS ? "Success" : "Failed");
}

// Send data to slave with retry mechanism
bool send_data_to_slave(uint8_t *slave_mac, int gpio_num, int value, int maxRetries = 3) {
    struct_message_send message;
    message.receive_GPIO_num = gpio_num;
    message.receive_Val = value;
    
    for (int i = 0; i < maxRetries; i++) {
        esp_err_t result = esp_now_send(slave_mac, (uint8_t *)&message, sizeof(message));
        if (result == ESP_OK) {
            Serial.printf("Data sent successfully to GPIO %d with value %d\n", gpio_num, value);
            return true;
        }
        Serial.printf("Retry %d: Failed to send data\n", i + 1);
        delay(50);
    }
    Serial.println("Failed to send data after all retries");
    return false;
}

void sendBrailleCharacter(char c) {
    Serial.printf("Processing character: %c\n", c);
    
    int brailleDots[6] = {0};
    bool found = false;
    
    for (BrailleChar b : brailleAlphabet) {
        if (b.letter == toupper(c)) {
            memcpy(brailleDots, b.dots, sizeof(brailleDots));
            found = true;
            break;
        }
    }
    
    if (!found) {
        Serial.printf("Warning: Character '%c' not found in Braille alphabet\n", c);
        return;
    }

    // Send to Slave 1 (dots 1-3)
    bool slave1Success = true;
    for (int i = 0; i < 3; i++) {
        if (!send_data_to_slave(slave1_mac, myPins[i], brailleDots[i])) {
            slave1Success = false;
            break;
        }
        delay(50);
    }

    // Send to Slave 2 (dots 4-6)
    bool slave2Success = true;
    for (int i = 3; i < 6; i++) {
        if (!send_data_to_slave(slave2_mac, myPins[i-3], brailleDots[i])) {
            slave2Success = false;
            break;
        }
        delay(50);
    }

    if (slave1Success && slave2Success) {
        Serial.printf("Successfully sent Braille pattern for '%c'\n", c);
    } else {
        Serial.printf("Error sending Braille pattern for '%c'\n", c);
    }
}

void webSocketEvent(WStype_t type, uint8_t * payload, size_t length) {
    switch(type) {
        case WStype_DISCONNECTED:
            Serial.println("WebSocket Disconnected!");
            isWebSocketConnected = false;
            break;
            
        case WStype_CONNECTED:
            Serial.printf("WebSocket Connected to: %s\n", websocket_server);
            isWebSocketConnected = true;
            webSocket.sendTXT("{\"type\":\"identification\",\"device\":\"esp32\"}");
            break;
            
        case WStype_TEXT: {
            Serial.printf("Received text: %s\n", payload);
            
            StaticJsonDocument<512> doc;
            DeserializationError error = deserializeJson(doc, payload);
            
            if (error) {
                Serial.printf("JSON Parse Error: %s\n", error.c_str());
                return;
            }

            if (doc.containsKey("char") && doc.containsKey("index")) {
                const char* charStr = doc["char"];
                int index = doc["index"];
                
                if (charStr && strlen(charStr) > 0) {
                    Serial.printf("Processing character '%c' at index %d\n", charStr[0], index);
                    sendBrailleCharacter(charStr[0]);
                    
                    // Create response JSON
                    StaticJsonDocument<200> response;
                    response["type"] = "confirmation";
                    response["status"] = "processed";
                    response["index"] = index;
                    
                    // Store the character as a string instead of a char
                    char charBuffer[2] = {charStr[0], '\0'};
                    response["char"] = charBuffer;
                    
                    String responseStr;
                    serializeJson(response, responseStr);
                    webSocket.sendTXT(responseStr);
                }
            }
            break;
        }
        
        case WStype_ERROR:
            Serial.printf("WebSocket Error: %u\n", payload[0]);
            break;
    }
}

// Setup WebSocket function (remains the same)
void setupWebSocket() {
    // Add custom headers including User-Agent
    webSocket.setExtraHeaders("User-Agent: ESP32");
    
    // Configure WebSocket
    webSocket.begin(websocket_server, websocket_port, websocket_path);
    webSocket.onEvent(webSocketEvent);
    webSocket.setReconnectInterval(5000);
    
    // Enable ping/pong for connection monitoring
    webSocket.enableHeartbeat(15000, 3000, 2);
}

void setup() {
    Serial.begin(115200);
    Serial.println("\nInitializing ESP32 Master...");

    // Connect to WiFi
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    
    Serial.print("Connecting to WiFi");
    int attempts = 0;
    while (WiFi.status() != WL_CONNECTED && attempts < 20) {
        delay(500);
        Serial.print(".");
        attempts++;
    }
    
    if (WiFi.status() == WL_CONNECTED) {
        Serial.printf("\nWiFi connected! IP: %s\n", WiFi.localIP().toString().c_str());
        isWiFiConnected = true;
    } else {
        Serial.println("\nWiFi connection failed!");
        ESP.restart();
    }

    // Initialize ESP-NOW
    uint8_t current_channel;
    esp_wifi_get_channel(&current_channel, NULL);
    Serial.printf("Current WiFi Channel: %d\n", current_channel);

    esp_err_t result = esp_wifi_set_channel(1, WIFI_SECOND_CHAN_NONE);
    Serial.printf("Current Wi-Fi channel::: %d\n", WiFi.channel());
    if (esp_now_init() != ESP_OK) {
        Serial.println("ESP-NOW initialization failed!");
        ESP.restart();
    }
    Serial.println("ESP-NOW initialized successfully");

    // Register ESP-NOW callback
    esp_now_register_send_cb(OnDataSent);

    // Register slaves
    esp_now_peer_info_t peerInfo;
    memset(&peerInfo, 0, sizeof(esp_now_peer_info_t));
    peerInfo.channel = 0;  // Use the same channel as your WiFi
    peerInfo.encrypt = false;

    // Register Slave 1
    memcpy(peerInfo.peer_addr, slave1_mac, 6);
    esp_err_t addResult1 = esp_now_add_peer(&peerInfo);
    if (addResult1 != ESP_OK) {
        Serial.printf("Failed to add Slave 1. Error: %d\n", addResult1);
    }

    // Register Slave 2
    memcpy(peerInfo.peer_addr, slave2_mac, 6);
    esp_err_t addResult2 = esp_now_add_peer(&peerInfo);
    if (addResult2 != ESP_OK) {
        Serial.printf("Failed to add Slave 2. Error: %d\n", addResult2);
    }

    // Initialize WebSocket after WiFi connection
    setupWebSocket();
    
    Serial.println("Setup completed");
}

void loop() {
    if (isWiFiConnected) {
        webSocket.loop();
        
        unsigned long currentMillis = millis();
        
        // Handle WiFi disconnection
        if (WiFi.status() != WL_CONNECTED) {
            Serial.println("WiFi disconnected! Reconnecting...");
            isWiFiConnected = false;
            WiFi.reconnect();
        }
        
        // Handle WebSocket reconnection
        if (!isWebSocketConnected && 
            (currentMillis - lastReconnectAttempt >= RECONNECT_INTERVAL)) {
            Serial.println("Attempting WebSocket reconnection...");
            setupWebSocket();
            lastReconnectAttempt = currentMillis;
        }
    }
    
    // Add some delay to prevent tight looping
    delay(10);
}
