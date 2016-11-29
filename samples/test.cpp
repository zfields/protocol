#include <chrono>
#include <future>
#include <iostream>
#include <FirmataQuery.h>
#include <UartSerial.h>

using namespace remote_wiring;
using namespace remote_wiring::protocol;
using namespace remote_wiring::transport;

void onContractReady (void * context) {
    reinterpret_cast<std::promise<void> *>(context)->set_value();
}

int main (int argc, char * argv[]) {
    std::promise<void> p;
    std::future<void> f = p.get_future();

    std::cout << ">>Firmata Capabilities Query<<" << std::endl;

    Stream * s = new UartSerial("/dev/ttyACM0");
    s->begin();
    FirmataQuery query;
    if ( 0 != query.queryContractAsync(s, onContractReady, &p) ) {
        std::cout << "Failed to query contract" << std::endl;
    } else if ( std::future_status::ready == f.wait_for(std::chrono::seconds(1)) ) {
        s->end();
    }

    return 0;
}
