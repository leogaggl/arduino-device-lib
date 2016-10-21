# The Things Network Arduino Library

This is an [Arduino Library](https://www.arduino.cc/en/Guide/Libraries) for Arduino-compatible devices like [The Things Uno](https://shop.thethingsnetwork.com/index.php/product/the-things-uno/) and [Node](https://shop.thethingsnetwork.com/index.php/product/the-things-node/) to communicate via [The Things Network](https://www.thethingsnetwork.org).

> At the moment this library requires devices to feature a [Microchip RN2xx3 module](http://www.microchip.com/design-centers/wireless-connectivity/embedded-wireless/lora-technology).

## Installation

* Install the library by [Using the Library Manager](https://www.arduino.cc/en/Guide/Libraries#toc3)
* **OR** by [Importing the .zip library](https://www.arduino.cc/en/Guide/Libraries#toc4) using either the [master](https://github.com/TheThingsNetwork/arduino-device-lib/archive/master.zip) or one of the [releases](https://github.com/TheThingsNetwork/arduino-device-lib/releases) ZIP files.

To use build in messages, thanks to [Protobufs](https://en.wikipedia.org/wiki/Protocol_Buffers), in the Arduino library, you might want to install the Nanopb compiler by [downloading](http://koti.kapsi.fi/%7Ejpa/nanopb/download/) it.
After that, in your shell, just define `$NANOPB` pointing to the Nanopb compiler's path e.g.:

`export NANOPB="~/dev/bin/nanopb-0.3.6-macosx-x86"`

## Documentation

* [The Things Network Documentation / Arduino](https://www.thethingsnetwork.org/docs/arduino/)
* [API Reference](API.md)

## Examples

The library comes with [examples](examples). After installing the library you need to restart the Arduino IDE before they can be found under **File > Examples > TheThingsNetwork**.
