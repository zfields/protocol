protocol
==============

## Library details:

The `protocol` library marshals calls from a [remote device object](https://github.com/remote-wiring/wiring) over a [serial connection](https://github.com/remote-wiring/transport). To accomplish this, the library utilizes the [Firmata protocol](https://github.com/firmata/protocol/blob/master/protocol.md), to encode and decode the bytes on both the originator and target (typically embedded) device.

Technical documentation, requirements and diagrams will be stored in the `docs/` folder. Sample programs are located in the `samples/` folder. Google Mock unit-tests are located in the `tests/` folder.

## Software License:
This project uses the ***MIT license***, please consult the license file for details.

## References:
[Firmata Protocol](https://github.com/firmata/protocol/blob/master/protocol.md)

## Example code:

#### Query the system capabilities

```c++
#include <FirmataQuery.h>
#include <UartSerial.h>

using namespace remote_wiring::protocol;
using namespace remote_wiring::transport;

int main (int argc, char * argv[]) {
    Stream * stream = new UartSerial("/dev/ttyO0");
    DeviceQuery * query = new FirmataQuery();

    // Connect to the remote device
    stream->begin();

    // Query the device capabilities
    query->queryAsync(&stream, nullptr, nullptr);

    stream->end();
    delete stream;
    return 0;
}
```
