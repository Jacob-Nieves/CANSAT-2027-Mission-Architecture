#include <telemetry.h>

const int ledPin = 2;

telemetry tel = telemetry();

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
  blink();
}

void setup() {
  pinMode(ledPin, OUTPUT);

  Serial.begin(115200);

  // Delay For Serial
  delay(100);

  // Setup As Receiver And Attach Blink To Callback
  tel.recieverSetup(handleMessage);

}

void loop() {
  // put your main code here, to run repeatedly:

}
