#include <chrono>
#include <future>
#include <iostream>
#include <FirmataQuery.h>
#include <UartSerial.h>

void onContractReady (void * context) {
    reinterpret_cast<std::promise<void> *>(context)->set_value();
}

int main (int argc, char * argv[]) {
    std::promise<void> p;
    std::future<void> f = p.get_future();

    std::cout << ">>Firmata Capabilities Query<<" << std::endl;

    remote_wiring::UartSerial usb("/dev/ttyACM0");
    remote_wiring::protocol::FirmataQuery query;

    usb.begin();
    if ( 0 != query.queryContractAsync(&usb, onContractReady, &p) ) {
        std::cout << "Failed to query contract!" << std::endl;
    } else if ( std::future_status::ready != f.wait_for(std::chrono::seconds(1)) ) {
        std::cout << "Query timed out!" << std::endl;
    } else {
        std::cout << std::endl << "Query succeed." << std::endl;
    }

    usb.end();
    return 0;
}
