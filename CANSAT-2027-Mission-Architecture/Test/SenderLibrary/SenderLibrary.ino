#include <telemetry.h>

const int ledPin = 2;

// Address Of Our Reciever
uint8_t receiverAddress[] = {0x30, 0x76, 0xF5, 0xA5, 0xEA, 0xE4};

// Telemetry Object
telemetry tel = telemetry();

// Blink The Onboard LED 
void blink()
{
  digitalWrite(ledPin, HIGH);
  delay(50);
  digitalWrite(ledPin, LOW);
}

// What To Do When Sending A Message?
void messageCallback(const esp_now_send_info_t *tx_info, esp_now_send_status_t status)
{
  blink();
}

void setup() {
  pinMode(ledPin, OUTPUT);

  Serial.begin(115200);

  // Delay For Serial
  delay(100);

  // Disable Debug Messages
  tel.debug_messages = false;

  // Setup As Receiver And Attach Blink To Callback
  tel.transmitterSetup(receiverAddress, messageCallback);

}

void loop() {
  // Send A Message Every 2 Seconds
  delay(2000);

  // Make Sure Out Message Is Cast As A <uint8_t*>
  tel.transmitMessage((uint8_t*)"Test");

}
