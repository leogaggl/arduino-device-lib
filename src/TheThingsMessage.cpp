#include <TheThingsNetwork.h>

#define debugPrintLn(...) { if (printStream) printStream->println(__VA_ARGS__); }
#define debugPrint(...) { if (printStream) printStream->print(__VA_ARGS__); }

void TheThingsMessage::ChoseMessage(api_Measurement *measurement, ttn_message_t message) {
  switch (message) {
    case Water:
      measurement->has_water = 1;
      break;
    case Motion:
      measurement->has_motion = 1;
      break;
    case Humidity:
      measurement->has_humidity = 1;
      break;
    case Temperature:
      measurement->has_temperature = 1;
      break;
    case all:
      measurement->has_water = 1;
      measurement->has_motion = 1;
      measurement->has_humidity = 1;
      measurement->has_temperature = 1;
      break;
    default:
      return;
  }
}

void TheThingsMessage::ProcessMessage(const byte *buffer, int size, int port) {
  api_Measurement message;
  pb_istream_t stream = pb_istream_from_buffer(buffer, size);
  if (!pb_decode(&stream, api_Measurement_fields, &message)) {
    return false;
  }
  if (message.has_water) {
    uint32_t Water = message.water;
    debugPrint(F("water = "));
    debugPrintLn(Water);
  }
  if (message.has_motion) {
    uint32_t Motion = message.motion;
    debugPrint(F("motion = "));
    debugPrintLn(Motion);
  }
  if (message.has_humidity) {
    uint32_t Humidity = message.humidity;
    debugPrint(F("humidity = "));
    debugPrintLn(Humidity);
  }
  if (message.has_temperature) {
    uint32_t Temperature = message.temperature;
    debugPrint(F("temperature = "));
    debugPrintLn(Temperature);
  }
}

void TheThingsMessage::showValues(api_Measurement measurement) {
if (measurement.has_water) {
    uint32_t Water = measurement.water;
    debugPrint(F("water = "));
    debugPrint(Water);
    debugPrint("; ");
  }
  if (measurement.has_motion) {
    uint32_t Motion = measurement.motion;
    debugPrint(F("motion = "));
    debugPrint(Motion);
    debugPrint("; ");
  }
  if (measurement.has_humidity) {
    uint32_t Humidity = measurement.humidity;
    debugPrint(F("humidity = "));
    debugPrint(Humidity);
    debugPrint("; ");
  }
  if (measurement.has_temperature) {
    uint32_t Temperature = measurement.temperature;
    debugPrint(F("temperature = "));
    debugPrint(Temperature);
    debugPrint("; ");
  }
  debugPrintLn();
}

void TheThingsMessage::encodeMessage(api_Measurement measurement) {
  this->sendStream = pb_ostream_from_buffer(this->buffer, sizeof(this->buffer));
  pb_encode(&this->sendStream, api_Measurement_fields, &measurement);
}

void TheThingsMessage::onMessage(void (*cb)(const byte* payload, int length, int port, bool confirm)) {
  this->messageCallback = cb;
}

int TheThingsMessage::sendMessage(int port, bool confirm) {
  if (this->messageCallback) {
    this->messageCallback(this->buffer, this->sendStream.bytes_written, port, confirm);
  }
  return 0;
}

TheThingsMessage::TheThingsMessage(Stream& printStream) {
  this->printStream = &printStream;
}
