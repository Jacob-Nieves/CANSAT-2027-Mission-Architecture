// Test A Board As A Reciever
#include <esp_now.h>
#include <WiFi.h>
#include <esp_wifi.h>

const int ledPin = 2;

// Structure For Data (Must Match Sender Structure)
typedef struct struct_message {
  char text[32];
  int count;
  float temp;
} struct_message;

struct_message incomingData;

// Callback Function On Data Recieved
void OnDataRecv(const esp_now_recv_info *recvInfo, const uint8_t *incomingDataPtr, int len) {
  memcpy(&incomingData, incomingDataPtr, sizeof(incomingData));
  
  // Blink On Data Recieved
  blink();

  Serial.print("Bytes received: ");
  Serial.println(len);
  Serial.print("Text: ");
  Serial.println(incomingData.text);
  Serial.print("Count: ");
  Serial.println(incomingData.count);
  Serial.print("Temperature: ");
  Serial.println(incomingData.temp);
  Serial.println();
}

// Blink The Onboard LED 
void blink()
{
  digitalWrite(ledPin, HIGH);
  delay(50);
  digitalWrite(ledPin, LOW);
}

void setup() {
  pinMode(ledPin, OUTPUT);

  Serial.begin(115200);

  Serial.println("Registered Reciever!");
  
  // Set Device As Wifi Station
  WiFi.mode(WIFI_STA);

  // Enable Long Range Protocol
  esp_err_t protocol_err = esp_wifi_set_protocol(WIFI_IF_STA, WIFI_PROTOCOL_LR);
  if (protocol_err == ESP_OK) {
    Serial.println("Long Range (LR) Mode Enabled Successfully");
  }

  // Lower The Transmission Rate To 250Kbps
  esp_err_t rate_err = esp_wifi_config_espnow_rate(WIFI_IF_STA, WIFI_PHY_RATE_LORA_250K);
  if (rate_err == ESP_OK) {
    Serial.println("PHY Rate locked to 250Kbps");
  }

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  // Register Callback For Incoming Data
  esp_now_register_recv_cb(OnDataRecv);
}
 
void loop() {}
