#include <TheThingsMessage.h>

#define debugPrintLn(...) { if (debugStream) debugStream->println(__VA_ARGS__); }
#define debugPrint(...) { if (debugStream) debugStream->print(__VA_ARGS__); }

void TheThingsMessage::choseMessage(api_Measurement *measurement, ttn_message_t message) {
  switch (message) {
    case water:
      measurement->has_water = 1;
      break;
    case motion:
      measurement->has_motion = 1;
      break;
    case humidity:
      measurement->has_humidity = 1;
      break;
    case temperature_celcius:
      measurement->has_temperature_celcius = 1;
      break;
    case temperature_fahrenheit:
      measurement->has_temperature_fahrenheit = 1;
      break;
    case all:
      measurement->has_water = 1;
      measurement->has_motion = 1;
      measurement->has_humidity = 1;
      measurement->has_temperature_fahrenheit = 1;
      measurement->has_temperature_celcius = 1;
      break;
    default:
      return;
  }
}

void TheThingsMessage::processMessage(const byte *buffer, int size, int port) {
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
  if (message.has_temperature_celcius) {
    float Temperature_celcius = message.temperature_celcius;
    debugPrint(F("temperature celcius = "));
    debugPrintLn(Temperature_celcius);
  }
  if (message.has_temperature_fahrenheit) {
    float Temperature_fahrenheit = message.temperature_fahrenheit;
    debugPrint(F("temperature fahrenheit = "));
    debugPrintLn(Temperature_fahrenheit);
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
    bool Motion = measurement.motion;
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
  if (measurement.has_temperature_celcius) {
    float Temperature_celcius = measurement.temperature_celcius;
    debugPrint(F("temperature celcius = "));
    debugPrint(Temperature_celcius);
    debugPrint("; ");
  }
  if (measurement.has_temperature_fahrenheit) {
    float Temperature_fahrenheit = measurement.temperature_fahrenheit;
    debugPrint(F("temperature fahrenheit = "));
    debugPrint(Temperature_fahrenheit);
    debugPrint("; ");
  }
  debugPrintLn();
}

void TheThingsMessage::onMessage(void (*cb)(const byte* payload, int length, int port)) {
  this->messageCallback = cb;
}

int TheThingsMessage::sendMessage(api_Measurement measurement, int port, bool confirm) {
  byte buffer[51];
  pb_ostream_t sendStream = pb_ostream_from_buffer(buffer, sizeof(buffer));
  pb_encode(&sendStream, api_Measurement_fields, &measurement);
  return this->ttn->sendBytes(buffer, sendStream.bytes_written, port, confirm);
}

TheThingsMessage::TheThingsMessage(Stream& modemStream, Stream& debugStream, TheThingsNetwork& ttn) {
  this->debugStream = &debugStream;
  this->modemStream = &modemStream;
  this->ttn = &ttn;
}
