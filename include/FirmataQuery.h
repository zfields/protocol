/* Created and copyrighted by Zachary J. Fields. Offered as open source under the MIT License (MIT). */

#ifndef FIRMATA_QUERY_H
#define FIRMATA_QUERY_H

#include <atomic>
#include <chrono>
#include <future>

#include <FirmataMarshaller.h>
#include <FirmataParser.h>

#include "DeviceContract.h"
#include "DeviceQuery.h"
#include "Stream.h"

namespace remote_wiring {
namespace protocol {

class FirmataQuery : public DeviceQuery {
  public:
    FirmataQuery (
    );

    int
    queryContractAsync (
        Stream * stream_,
        contractReady contractReadyCallback_,
        void * contract_ready_callback_context_
    ) override;

    DeviceContract *
    detachDeviceContract (
        void
    ) override;

    Stream *
    getStream (
        void
    ) const override;

  private:
    FirmataMarshaller _marshaller;
    FirmataParser _parser;
    uint8_t * _parser_buffer;
    size_t _parser_buffer_size;
    Stream * _stream;

    void
    processFirmataStream (
        void
    );

    static std::promise<void> callback_signal;
    static std::future<void> callback_gate;
    static void * contract_ready_callback_context;
    static std::atomic_bool contract_ready;
    static contractReady contractReadyCallback;
    static std::atomic_bool firmata_ready;
    static pin_config_t * pin;
    static size_t pin_count;

    static
    void
    extendBuffer (
        void * context_
    );

    static
    void
    firmataReadyCallback (
        void
    );

    static
    void
    queryResponseCallback (
        uint8_t command,
        uint8_t argc,
        uint8_t * argv
    );

    static
    void
    serialEventCallback (
        void * context_
    );
};

} // protocol
} // remote_wiring

#endif // FIRMATA_QUERY_H

/* Created and copyrighted by Zachary J. Fields. Offered as open source under the MIT License (MIT). */
