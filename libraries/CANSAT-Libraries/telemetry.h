/* 
    Define Telemetry Basic Startup As Well As Data Structure
    And Keep Telemetry Settings And Reference Across 
    Several Devices
*/
#include <esp_now.h>
#include <WiFi.h>
#include <esp_wifi.h>
#include <stdio.h>
#include <stdint.h>

/* 
TRANSMITTER USAGE

Initialize As Transmitter With Mac Address

Transmit Bytes


RECEIVER USAGE

Initialize As Receiver With A Register Callback With Recieved Data

*/ 


struct telemetry
{
    private:
    // Can This Telemetry Object Transmit Data?
    bool is_transmitter = false;
    // Can This Telemetry Object Receive Data?
    bool is_receiver = false;
    // Reciever Address
    uint8_t receiverAddress[6];
    // Info About Our Peer
    esp_now_peer_info_t peerInfo;

    public:

    // Should We Print Debug Messages?
    bool debug_messages = true;

    // Send A Message
    void transmitMessage(uint8_t *message)
    {
        if (!is_transmitter)
        {
            Serial.println("Telemetry: This device is not yet a transmitter!");
            return;
        }

        // Send Message
        esp_err_t result = esp_now_send(receiverAddress, message, sizeof(message));

        if (result == ESP_OK && debug_messages) {
            Serial.println("Sent with success");
        } 
        else if (debug_messages) {
            Serial.println("Error sending the data");
        }
    }

    // Allow This Device To Transmit And Register The Provided Callback
    void transmitterSetup(uint8_t _receiverAddress[6], esp_now_send_cb_t cb)
    {
        Serial.println("Telemetry Setup: Setting up device as a transmitter...");

        if (is_transmitter)
        {
            Serial.println("Telemetry Setup: Device is already a transmitter!");
            return;
        }

        // Copy Bytes Into Receiver Address
        memcpy(receiverAddress, _receiverAddress, 6);

        // Set Device As A Wifi Station
        WiFi.mode(WIFI_STA);

        // Enable Long Range Protocol
        esp_err_t protocol_err = esp_wifi_set_protocol(WIFI_IF_STA, WIFI_PROTOCOL_LR);
        if (protocol_err != ESP_OK) {
          Serial.println("Telemetry Setup: Unable to enable long range mode!");
          Serial.println("Telemetry Setup: Setup cancelled!");
          return;
        }
    
        // Lower The Transmission Rate To 250Kbps
        esp_err_t rate_err = esp_wifi_config_espnow_rate(WIFI_IF_STA, WIFI_PHY_RATE_LORA_250K);
        if (rate_err != ESP_OK) {
          Serial.println("Telemetry Setup: Unable to set PHY rate to 250Kbps!");
          Serial.println("Telemetry Setup: Setup cancelled!");
          return;
        }

        // Increase Tx Power To Max
        esp_wifi_set_max_tx_power(84);

        // Init ESP-NOW
        if (esp_now_init() != ESP_OK) {
            Serial.println("Telemetry Setup: Error initializing ESP-NOW");
            Serial.println("Telemetry Setup: Setup cancelled!");
            return;
        }
        
        // Register The Callback
        esp_now_register_send_cb(cb);

        // Register Our Reciever
        memcpy(peerInfo.peer_addr, receiverAddress, 6);
        peerInfo.channel = 0;  
        peerInfo.encrypt = false;

        // Add Our Reciever        
        if (esp_now_add_peer(&peerInfo) != ESP_OK){
            Serial.println("Telemetry Setup: Failed to add peer");
            Serial.println("Telemetry Setup: Setup cancelled!");
            return;
        }

        // Set At The End
        is_transmitter = true;

        Serial.println("Telemetry Setup: Succesfully setup device as a transmitter!");

        // Print Transmission Address
        Serial.print("Telemetry Setup: Transmitting to address ");
        Serial.printf("%02X:%02X:%02X:%02X:%02X:%02X\n", receiverAddress[0], receiverAddress[1], receiverAddress[2], receiverAddress[3], receiverAddress[4], receiverAddress[5]);
    }

    

    // Allow This Device To Recieve Transmission And Register The Provided Callback
    void recieverSetup(esp_now_recv_cb_t cb)
    {
        Serial.println("Telemetry Setup: Setting up device as a receiver...");

        if (is_receiver)
        {
            Serial.println("Telemetry Setup: Device is already a receiver!");
            return;
        }

        // Set Device As Wifi Station
        WiFi.mode(WIFI_STA);

        // Enable Long Range Protocol
        esp_err_t protocol_err = esp_wifi_set_protocol(WIFI_IF_STA, WIFI_PROTOCOL_LR);
        if (protocol_err != ESP_OK) {
          Serial.println("Telemetry Setup: Unable to enable long range mode!");
          Serial.println("Telemetry Setup: Setup cancelled!");
          return;
        }
    
        // Lower The Transmission Rate To 250Kbps
        esp_err_t rate_err = esp_wifi_config_espnow_rate(WIFI_IF_STA, WIFI_PHY_RATE_LORA_250K);
        if (rate_err != ESP_OK) {
          Serial.println("Telemetry Setup: Unable to set PHY rate to 250Kbps!");
          Serial.println("Telemetry Setup: Setup cancelled!");
          return;
        }

        // Init ESP-NOW
        if (esp_now_init() != ESP_OK) {
            Serial.println("Telemetry Setup: Error initializing ESP-NOW");
            Serial.println("Telemetry Setup: Setup cancelled!");
            return;
        }

        // Register Callback For Incoming Data
        esp_now_register_recv_cb(cb);

        // Set At The End
        is_receiver = true;

        Serial.println("Telemetry Setup: Succesfully setup device as a receiver!");

        // Print MAC Address
        Serial.print("Telemetry Setup: Receiving on address ");
        Serial.println(WiFi.macAddress());
    }
};


