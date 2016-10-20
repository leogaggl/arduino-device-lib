#include <TheThingsMessage.h>

#define debugPrintLn(...) { if (debugStream) debugStream->println(__VA_ARGS__); }
#define debugPrint(...) { if (debugStream) debugStream->print(__VA_ARGS__); }

void TheThingsMessage::processMessage(const byte *buffer, int size, int port) {
  sensorData message;
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

void TheThingsMessage::showValues(sensorData measurement) {
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

void TheThingsMessage::encodeSensorData(sensorData *data, byte **buffer, size_t *size) {
  byte message[51];
  pb_ostream_t sendStream = pb_ostream_from_buffer(message, sizeof(message));
  pb_encode(&sendStream, api_Measurement_fields, data);
  *(buffer) = message;
  *size = sendStream.bytes_written;
}

TheThingsMessage::TheThingsMessage(Stream& modemStream, Stream& debugStream) {
  this->debugStream = &debugStream;
  this->modemStream = &modemStream;
}
