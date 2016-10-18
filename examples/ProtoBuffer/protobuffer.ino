#include <TheThingsNetwork.h>

TheThingsNetwork ttn(Serial1, Serial, TTN_FP_EU868);
TheThingsMessage msg(Serial);

const byte appEui[8] = { 0x70, 0xB3, 0xD5, 0x7E, 0xD0, 0x00, 0x11, 0x86 };
const byte appKey[16] = { 0xEF, 0x4C, 0xAA, 0x85, 0xE1, 0xB2, 0x26, 0x53, 0x98, 0xE2, 0x3C, 0xD1, 0x31, 0x50, 0xCA, 0xDF };

api_Measurement measurement = api_Measurement_init_default;

void setup()
{
  pinMode(2, INPUT);

  Serial1.begin(57600);
  Serial.begin(9600);

  while (!Serial);

  ttn.join(appEui, appKey);

  msg.ChoseMessage(&measurement, all);

  msg.onMessage(message);
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
