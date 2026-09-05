#include <esp_now.h>
#include <WiFi.h>
#include <esp_wifi.h>

// Address Of Our Reciever
uint8_t receiverAddress[] = {0x30, 0x76, 0xF5, 0xA5, 0xEA, 0xE4};

const int ledPin = 2;

// Structure For Data (Must Match Sender Structure)
typedef struct struct_message {
  char text[32];
  int count;
  float temp;
} struct_message;

struct_message myData;
esp_now_peer_info_t peerInfo;

// Callback When Data Is Sent
void OnDataSent(const wifi_tx_info_t *info, esp_now_send_status_t status) {
  // Blink On Data Sent
  blink();
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
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

  Serial.println("Registered Sender!");
  
  // Set Device As A Wifi Station
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

  // Register The Callback
  esp_now_register_send_cb(OnDataSent);
  
  // Register Our Reciever
  memcpy(peerInfo.peer_addr, receiverAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  
  // Add Our Reciever        
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }
}
 
void loop() {
  // Format Data To Send
  strcpy(myData.text, "Hello from Sender!");
  myData.count++;
  myData.temp = 24.5;
  
  // Send Message
  esp_err_t result = esp_now_send(receiverAddress, (uint8_t *) &myData, sizeof(myData));
   
  if (result == ESP_OK) {
    Serial.println("Sent with success");
  } else {
    Serial.println("Error sending the data");
  }
  
  // Wait Two Seconds
  delay(2000);
}