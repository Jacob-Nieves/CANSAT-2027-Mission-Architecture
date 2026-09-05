// Print The MAC Address Of Our Board TO Serial Monitor
#include "WiFi.h"

void setup() {
  Serial.begin(115200);
  Serial.println("Retrieving MAC Address...");
  delay(100);
  WiFi.mode(WIFI_MODE_STA);
  delay(100);
  Serial.print("Receiver MAC Address: ");
  Serial.println(WiFi.macAddress());
}

// Loop Function Is Required Even If Empty
void loop()
{

}