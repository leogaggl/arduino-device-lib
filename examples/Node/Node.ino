#include <TheThingsNetwork.h>
#include <TheThingsNode.h>

const byte appEui[8] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
const byte appKey[16] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

#define loraSerial Serial1
#define debugSerial Serial

TheThingsNetwork ttn(loraSerial, debugSerial, TTN_FP_EU868);
TheThingsNode *node;

enum port : byte
{
  PORT_INTERVAL = 1,
  PORT_TEMPERATURE,
  PORT_MOTION,
  PORT_BUTTON
};

void setup() {
  loraSerial.begin(57600);
  debugSerial.begin(9600);

  // Wait a maximum of 10s for Serial Monitor
  while (!debugSerial && millis() < 10000);

  ttn.onMessage(onMessage);

  debugSerial.println("-- TTN: STATUS");
  ttn.showStatus();

  debugSerial.println("-- TTN: JOIN");
  ttn.join(appEui, appKey);

  node = TheThingsNode::setup();
  node->setColor(TTN_GREEN);

//  node->configUSB(true);
  node->configLight(true);
  node->configInterval(true, 20000);

  node->onWake(onWake);
  node->onInterval(interval);
  node->onSleep(onSleep);
  node->onTemperature(onTemperature);
  node->onMotionStart(onMotionStart);
  node->onMotionStop(onMotionStop);
  node->onButtonPress(onButtonPress);
  node->onButtonRelease(onButtonRelease);

  debugSerial.println("-- NODE: STATUS");
  node->showStatus();
}

void loop() {
  node->loop();
}

void interval() {
  node->setColor(TTN_YELLOW);
  
  debugSerial.println("-- INTERVAL");
  node->showStatus();

  byte* bytes;
  byte payload[9];

  payload[0] = PORT_INTERVAL;

  uint16_t battery = node->getBattery();
  bytes = (byte*) &battery;
  payload[1] = bytes[1];
  payload[2] = bytes[0];

  uint16_t light = node->getLight();
  bytes = (byte*) &light;
  payload[3] = bytes[1];
  payload[4] = bytes[0];

  int16_t temperature = round(node->getTemperatureAsFloat() * 100);
  bytes = (byte*) &temperature;
  payload[5] = bytes[1];
  payload[6] = bytes[0];

  payload[7] = node->getColor();

  byte flags = B00000000;
  
  if (node->isMoving()) {
    flags = flags | 1;
  }

  if (node->isButtonPressed()) {
    flags = flags | 2;
  }

  if (node->isUSBConnected()) {
    flags = flags | 4;
  }

  payload[8] = flags;

  ttn.sendBytes(payload, sizeof(payload), PORT_INTERVAL);
}

void onWake() {
  node->setColor(TTN_GREEN);
}

void onSleep() {
  node->setColor(TTN_BLACK);
}

void onTemperature() {
  float temperature = node->getTemperatureAsFloat();
  
  node->setColor(TTN_RED);
  debugSerial.println("-- TEMPERATURE: " + String(temperature));

  byte* bytes;
  byte payload[3];

  // until payload functions receive port
  payload[0] = PORT_TEMPERATURE;

  int16_t rounded = round(temperature * 100);
  bytes = (byte*) &rounded;
  payload[1] = bytes[1];
  payload[2] = bytes[0];

  ttn.sendBytes(payload, sizeof(payload), PORT_TEMPERATURE);
}

void onMotionStart() {
  node->setColor(TTN_RED);
  debugSerial.println("-- MOTION START");
}

void onMotionStop(unsigned long duration) {
  node->setColor(TTN_RED);
  debugSerial.print("-- MOTION STOP: ");
  debugSerial.println(duration);

  byte* bytes;
  byte payload[5];

  // until payload functions receive port
  payload[0] = PORT_MOTION;

  bytes = (byte*) &duration;
  payload[1] = bytes[3];
  payload[2] = bytes[2];
  payload[3] = bytes[1];
  payload[4] = bytes[0];

  ttn.sendBytes(payload, sizeof(payload), PORT_MOTION);
}

void onButtonPress() {
  node->setColor(TTN_RED);
  debugSerial.println("-- BUTTON PRESS");
}

void onButtonRelease(unsigned long duration) {
  node->setColor(TTN_RED);
  debugSerial.print("-- BUTTON RELEASE: ");
  debugSerial.println(duration);

  byte* bytes;
  byte payload[5];

  // until payload functions receive port
  payload[0] = PORT_BUTTON;

  bytes = (byte*) &duration;
  payload[1] = bytes[3];
  payload[2] = bytes[2];
  payload[3] = bytes[1];
  payload[4] = bytes[0];

  ttn.sendBytes(payload, sizeof(payload), PORT_BUTTON);
}

void onMessage(const byte* payload, int length, int port) {
  debugSerial.println("-- ON MESSAGE");

  uint32_t interval;

  switch (port) {
    case 2:
      interval = ((payload[1] << 8) + payload[2]) * 1000;
      node->configInterval(true, interval);
      debugSerial.println("Loop delay changed: " + String(interval));
      loop();
      break;
    case 3:
      bool enabled = (payload[0] == 1);
      uint8_t lower = (payload[1] << 8) + payload[2];
      uint8_t upper = (payload[3] << 8) + payload[4];
      uint8_t critical = (payload[5] << 8) + payload[6];
      //      node.configTemperature(enabled, lower, upper, critical);
      debugSerial.print("Temperature alert changed, on: ");
      debugSerial.print(enabled ? "yes" : "no");
      debugSerial.println(", lower: " + String(lower) + ", upper: " + String(upper) + ", critical: " + String(critical));
      break;
  }
}
