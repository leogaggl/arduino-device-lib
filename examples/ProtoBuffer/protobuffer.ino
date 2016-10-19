#include <TheThingsNetwork.h>

const byte appEui[8] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
const byte appKey[16] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
, 0x00 };

#define loraSerial Serial1
#define debugSerial Serial

TheThingsNetwork ttn(loraSerial, debugSerial, TTN_FP_EU868);
TheThingsMessage msg(debugSerial);

api_Measurement measurement = api_Measurement_init_default;

void setup()
{
  pinMode(2, INPUT);

  loraSerial.begin(57600);
  debugSerial.begin(9600);

  // Wait a maximum of 10s for Serial Monitor
  while (!debugSerial && millis() < 10000);

  //Set callback for incoming message
  msg.onMessage(message);

  debugSerial.println("-- STATUS");
  ttn.showStatus();

  debugSerial.println("-- JOIN");
  ttn.join(appEui, appKey);

  //Chose message you want to send
  msg.ChoseMessage(&measurement, all /* water, humidity, temperature, motion*/);
}

void loop()
{
  float humidity = 232;
  uint32_t motion = digitalRead(2) == HIGH;
  uint32_t water = 682 - analogRead(A0);
  uint32_t temperature = 345 - analogRead(A0);

  measurement.motion = motion;
  measurement.water = water;
  measurement.temperature = temperature;
  measurement.humidity = humidity;

  msg.showValues(measurement);

  msg.encodeMessage(measurement);

  msg.sendMessage();

  delay(10000);
}

void message(const byte* payload, int length, int port, bool confirm) {
  if (ttn.sendBytes(payload, length, port, confirm) == 2) {
    Serial.println("-- MESSAGE");
    Serial.print("Received " + String(length) + " bytes on port " + String(port) + ":");

    for (int i = 0; i < length; i++) {
      Serial.print(" " + String(payload[i]));
    }
    Serial.println();
    msg.ProcessMessage(payload, length, port);
    Serial.println();
  }
}