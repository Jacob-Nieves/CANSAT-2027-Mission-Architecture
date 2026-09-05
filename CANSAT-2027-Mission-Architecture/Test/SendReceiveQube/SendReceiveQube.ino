#include <telemetry.h>

const int ledPin = 2;

telemetry tel = telemetry();

// Address Of Our Reciever
uint8_t receiverAddress[] = {0x30, 0x76, 0xF5, 0xA5, 0xEA, 0xE4};

// Blink The Onboard LED 
void blink()
{
  digitalWrite(ledPin, HIGH);
  delay(50);
  digitalWrite(ledPin, LOW);
}

// What To Do When Getting A Message?
void handleMessage(const esp_now_recv_info_t *recv_info, const uint8_t *incomingData, int len)
{
  // Blink When Receiving A Message
  blink();
}

// What To Do When Sending A Message?
void messageCallback(const esp_now_send_info_t *tx_info, esp_now_send_status_t status)
{
  // Do Nothing
}

void setup() {
  pinMode(ledPin, OUTPUT);

  Serial.begin(115200);

  // Delay For Serial
  delay(100);

  // Setup As Receiver And Attach Blink To Callback
  tel.recieverSetup(handleMessage);

  // Setup As Transmitter And Assign Empty Function To Callback
  tel.transmitterSetup(receiverAddress, messageCallback);
}

void loop() {
  // Send A Message Every 2 Seconds
  delay(2000);

  // Make Sure Out Message Is Cast As A <uint8_t*>
  tel.transmitMessage((uint8_t*)"Test");

}
