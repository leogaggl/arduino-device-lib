#ifndef _THETHINGSMESSAGE_H_
#define _THETHINGSMESSAGE_H_

#include <TheThingsNetwork.h>
#include <pb.h>
#include <pb_encode.h>
#include <pb_decode.h>
#include "measurement.pb.h"

typedef api_Measurement sensorData;

class TheThingsMessage
{
  private:
    Stream* debugStream;
    Stream* modemStream;

  public:
    TheThingsMessage(Stream& modemStream, Stream& debugStream);
    void processMessage(const byte *buffer, int size, int port);
    void encodeSensorData(sensorData *measurement, byte **buffer, size_t *size);
    void showValues(sensorData measurement);
};

#endif
