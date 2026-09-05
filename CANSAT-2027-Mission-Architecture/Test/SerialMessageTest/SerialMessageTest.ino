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


void setup() {
  pinMode(ledPin, OUTPUT);

  Serial.begin(115200);
}

// In The Implementation The Message Would Be Passed
// From The Ground Station To The Container And PocketQube
void handleMessage(String msg)
{
  blink();
  msg.toUpperCase();
  Serial.print("Received: ");
  Serial.println(msg);

  // Handle Messages
  if (msg == "SIM")
  {
    Serial.println("Setting Mode To SIM...");
  }
  else if (msg == "SIMP")
  {
    Serial.println("Sending Simulated Pressure Data...");
  }
  else if (msg == "CAL")
  {
    Serial.println("Calibrating Sensors...");
  }
  else if (msg == "ARM")
  {
    Serial.println("Arming Launch Detection...");
  }
  else if (msg == "MEC")
  {
    Serial.println("Controlling Mechanism...");
  }

}

void loop() {
  if (Serial.available() > 0) {
    handleMessage(Serial.readStringUntil('\n'));
    
  }
}
