#include <WiFi.h>

const char* ssid     = "Yash";
const char* password = "Easports28";

WiFiServer server(80);

int vibMotor1 = 5;
int vibMotor2 = 18;
int vibMotor3 = 19;

struct BrailleMap {
    char alphabet;
    int left1, left2, left3;
};

void initializeBrailleMap(struct BrailleMap map[]) {
    struct BrailleMap tempMap;

    // Initialize the Braille mappings for each alphabet for the left hand
    tempMap.alphabet = 'a';
    tempMap.left1 = 1;
    tempMap.left2 = 0;
    tempMap.left3 = 0;
    map[0] = tempMap;

    tempMap.alphabet = 'b';
    tempMap.left1 = 1;
    tempMap.left2 = 1;
    tempMap.left3 = 0;
    map[1] = tempMap;

    tempMap.alphabet = 'c';
    tempMap.left1 = 1;
    tempMap.left2 = 0;
    tempMap.left3 = 0;
    map[2] = tempMap;

    tempMap.alphabet = 'd';
    tempMap.left1 = 1;
    tempMap.left2 = 0;
    tempMap.left3 = 0;
    map[3] = tempMap;

    tempMap.alphabet = 'e';
    tempMap.left1 = 1;
    tempMap.left2 = 0;
    tempMap.left3 = 0;
    map[4] = tempMap;
   
    tempMap.alphabet = 'f';
    tempMap.left1 = 1;
    tempMap.left2 = 1;
    tempMap.left3 = 0;
    map[5] = tempMap;
   
    tempMap.alphabet = 'g';
    tempMap.left1 = 1;
    tempMap.left2 = 1;
    tempMap.left3 = 0;
    map[6] = tempMap;
   
    tempMap.alphabet = 'h';
    tempMap.left1 = 1;
    tempMap.left2 = 1;
    tempMap.left3 = 0;
    map[7] = tempMap;
   
    tempMap.alphabet = 'i';
    tempMap.left1 = 0;
    tempMap.left2 = 1;
    tempMap.left3 = 0;
    map[8] = tempMap;
   
    tempMap.alphabet = 'j';
    tempMap.left1 = 0;
    tempMap.left2 = 1;
    tempMap.left3 = 0;
    map[9] = tempMap;
   
    tempMap.alphabet = 'k';
    tempMap.left1 = 1;
    tempMap.left2 = 0;
    tempMap.left3 = 1;
    map[10] = tempMap;
   
    tempMap.alphabet = 'l';
    tempMap.left1 = 1;
    tempMap.left2 = 1;
    tempMap.left3 = 1;
    map[11] = tempMap;
   
    tempMap.alphabet = 'm';
    tempMap.left1 = 1;
    tempMap.left2 = 0;
    tempMap.left3 = 1;
    map[12] = tempMap;
   
    tempMap.alphabet = 'n';
    tempMap.left1 = 1;
    tempMap.left2 = 0;
    tempMap.left3 = 1;
    map[13] = tempMap;
   
    tempMap.alphabet = 'o';
    tempMap.left1 = 1;
    tempMap.left2 = 0;
    tempMap.left3 = 1;
    map[14] = tempMap;
   
    tempMap.alphabet = 'p';
    tempMap.left1 = 1;
    tempMap.left2 = 1;
    tempMap.left3 = 1;
    map[15] = tempMap;
   
    tempMap.alphabet = 'q';
    tempMap.left1 = 1;
    tempMap.left2 = 1;
    tempMap.left3 = 1;
    map[16] = tempMap;
   
    tempMap.alphabet = 'r';
    tempMap.left1 = 1;
    tempMap.left2 = 1;
    tempMap.left3 = 1;
    map[17] = tempMap;
   
    tempMap.alphabet = 's';
    tempMap.left1 = 0;
    tempMap.left2 = 1;
    tempMap.left3 = 1;
    map[18] = tempMap;
   
    tempMap.alphabet = 't';
    tempMap.left1 = 0;
    tempMap.left2 = 1;
    tempMap.left3 = 1;
    map[19] = tempMap;
   
    tempMap.alphabet = 'u';
    tempMap.left1 = 1;
    tempMap.left2 = 0;
    tempMap.left3 = 1;
    map[20] = tempMap;
   
    tempMap.alphabet = 'v';
    tempMap.left1 = 1;
    tempMap.left2 = 1;
    tempMap.left3 = 1;
    map[21] = tempMap;
   
    tempMap.alphabet = 'w';
    tempMap.left1 = 0;
    tempMap.left2 = 1;
    tempMap.left3 = 0;
    map[22] = tempMap;
   
    tempMap.alphabet = 'x';
    tempMap.left1 = 1;
    tempMap.left2 = 0;
    tempMap.left3 = 1;
    map[23] = tempMap;
   
    tempMap.alphabet = 'y';
    tempMap.left1 = 1;
    tempMap.left2 = 0;
    tempMap.left3 = 1;
    map[24] = tempMap;
   
    tempMap.alphabet = 'z';
    tempMap.left1 = 1;
    tempMap.left2 = 0;
    tempMap.left3 = 1;
    map[25] = tempMap;
}


void setup() {
    Serial.begin(115200);
    pinMode(vibMotor1, OUTPUT);
    pinMode(vibMotor2, OUTPUT);
    pinMode(vibMotor3, OUTPUT);

    delay(10);

    // Connect to WiFi network
    Serial.println();
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);

    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi connected.");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
   
    server.begin();
}

void loop() {
    WiFiClient client = server.available();
    if (client) {
        Serial.println("New Client.");
        String currentLine = "";

        while (client.connected()) {
            if (client.available()) {
                char c = client.read();
                Serial.write(c);
                if (c == '\n') {
                    if (currentLine.length() == 0) {
                        client.println("HTTP/1.1 200 OK");
                        client.println("Content-type:text/html");
                        client.println();
                        client.println("<html><head><title>Vibraille</title></head><body><h1>Vibraille</h1>");
                        //
                        client.println("<form action=\"/H\" method=\"get\">");
                        client.println("<style>");
                        client.println("form {");
                        client.println("  font-family: Arial, sans-serif;");
                        client.println("  width: 300px;");
                        client.println("  margin: 0 auto;");
                        client.println("  text-align: center;");
                        client.println("}");
                        client.println("input[type=text] {");
                        client.println("  padding: 10px;");
                        client.println("  font-size: 16px;");
                        client.println("  border: 1px solid #ccc;");
                        client.println("  border-radius: 4px;");
                        client.println("  box-shadow: 0px 0px 5px rgba(0,0,0,0.1);");
                        client.println("  width: 70%;");
                        client.println("}");
                        client.println("input[type=submit] {");
                        client.println("  padding: 10px 20px;");
                        client.println("  font-size: 16px;");
                        client.println("  background-color: #4CAF50;");
                        client.println("  color: white;");
                        client.println("  border: none;");
                        client.println("  border-radius: 4px;");
                        client.println("  box-shadow: 0px 0px 5px rgba(0,0,0,0.1);");
                        client.println("  cursor: pointer;");
                        client.println("}");
                        client.println("</style>");
                        client.println("Enter a word: <input type=\"text\" name=\"word\"><br>");
                        client.println("<input type=\"submit\" value=\"Submit\">");
                        client.println("</form>");
                        client.println("<br>");
                        break;
                    }else {
                        currentLine = "";
                    }
                } else if (c != '\r') {
                    currentLine += c;
                }

                // Check client request and control motors accordingly
                if (currentLine.startsWith("GET /H")) {
                    // Activate Braille translation and control motors
                    BrailleMap brailleMap[26];
                    initializeBrailleMap(brailleMap);
                    // Extract the word from the GET request
                    int wordIndex = currentLine.indexOf("word=");

                    if (wordIndex != -1) {
                        String word = currentLine.substring(wordIndex+5); // Assuming 'word=' is 5 characters
                        // Process each character in the word
                        for (int i = 0; i < word.length(); i++) {
                            char letter = word.charAt(i);
                            if (letter >= 'a' && letter <= 'z') {
                                int brailleIndex = letter - 'a';
                                digitalWrite(vibMotor1, brailleMap[brailleIndex].left1);
                                digitalWrite(vibMotor2, brailleMap[brailleIndex].left2);
                                digitalWrite(vibMotor3, brailleMap[brailleIndex].left3); // Uncomment for third motor
                                delay(100);
                                // Delay after turning off motors
                            }
                        }
                      digitalWrite(vibMotor1, LOW);
                      digitalWrite(vibMotor2, LOW);
                      digitalWrite(vibMotor3, LOW);
                    }
                } else if (currentLine.endsWith("GET /L")) {
                    digitalWrite(vibMotor1, LOW);
                    digitalWrite(vibMotor2, LOW);
                    digitalWrite(vibMotor3, LOW); // Uncomment this line if you want to add a third motor
                }
            }
        }

        // Close the connection
        client.stop();

        Serial.println("Client Disconnected.");
    }
}
