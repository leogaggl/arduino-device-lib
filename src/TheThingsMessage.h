#ifndef _THETHINGSMESSAGE_H_
#define _THETHINGSMESSAGE_H_

#include <TheThingsNetwork.h>
#include <pb.h>
#include <pb_encode.h>
#include <pb_decode.h>
#include "measurement.pb.h"

class TheThingsMessage
{
  private:
    TheThingsNetwork* ttn;
    Stream* debugStream;
    Stream* modemStream;
    void (* messageCallback)(const byte* payload, int length, int port);

  public:
    TheThingsMessage(Stream& modemStream, Stream& debugStream, TheThingsNetwork& ttn);
    void processMessage(const byte *buffer, int size, int port);
    void onMessage(void (*cb)(const byte* payload, int length, int port));
    int sendMessage(api_Measurement measurement, int port = 1, bool confirm = false);
    void showValues(api_Measurement measurement);
};


#endif
