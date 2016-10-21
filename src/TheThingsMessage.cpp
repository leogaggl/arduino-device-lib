#include <TheThingsMessage.h>

void TheThingsMessage::encodeSensorData(sensorData *data, byte **buffer, size_t *size) {
  byte message[51];
  pb_ostream_t sendStream = pb_ostream_from_buffer(message, sizeof(message));
  pb_encode(&sendStream, api_Measurement_fields, data);
  *buffer = message;
  *size = sendStream.bytes_written;
}
