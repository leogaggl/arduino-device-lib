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
  public:
    static void encodeSensorData(api_Measurement *measurement, byte **buffer, size_t *size);
};

#endif
