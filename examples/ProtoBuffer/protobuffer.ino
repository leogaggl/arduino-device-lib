#include <TheThingsNetwork.h>
#include <TheThingsMessage.h>

// Set your AppEUI and AppKey
const byte appEui[8] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
const byte appKey[16] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
, 0x00 };

#define loraSerial Serial1
#define debugSerial Serial

TheThingsNetwork ttn(loraSerial, debugSerial, /* TTN_FP_EU868 or TTN_FP_US915 */);

sensorData data = api_Measurement_init_default;

void setup()
{
  pinMode(2, INPUT);

  loraSerial.begin(57600);
  debugSerial.begin(9600);

  // Wait a maximum of 10s for Serial Monitor
  while (!debugSerial && millis() < 10000);

  debugSerial.println("-- STATUS");
  ttn.showStatus();

  debugSerial.println("-- JOIN");
  ttn.join(appEui, appKey);

  //Chose message you want to send (send the message by writing true)
  data.has_motion = true;
  data.has_water = false;
  data.has_temperature_celcius = true;
  data.has_temperature_fahrenheit = true;
  data.has_humidity = true;
}

void loop() {
  debugSerial.println("-- LOOP");

  float humidity = 232;
  bool motion = digitalRead(2) == HIGH;
  uint32_t water = 682 - analogRead(A0);
  float temperature_celcius = 345 - analogRead(A0);
  float temperature_fahrenheit = 345 - analogRead(A0);
  byte *buffer;
  size_t size;

  debugSerial.print("Motion : ");
  debugSerial.print(motion);
  debugSerial.print("; Humidity : ");
  debugSerial.print(humidity);
  debugSerial.print("; Water : ");
  debugSerial.print(water);
  debugSerial.print("; Temperature celcius : ");
  debugSerial.print(temperature_celcius);
  debugSerial.print("; Temperature farhenheit : ");
  debugSerial.println(temperature_fahrenheit);

  data.motion = motion;
  data.water = water;
  data.temperature_celcius = temperature_celcius;
  data.temperature_fahrenheit = temperature_fahrenheit;
  data.humidity = humidity;

  //Encode the message
  TheThingsMessage::encodeSensorData(&data, &buffer, &size);

  //Send the message
  ttn.sendBytes(buffer, size);

  delay(10000);
}