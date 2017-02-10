/* Created and copyrighted by Zachary J. Fields. Offered as open source under the MIT License (MIT). */

#include "FirmataQuery.h"

#include <cstdio>
#include <iostream>

#include "FirmataConstants.h"
#include "FirmataContract.h"

using namespace remote_wiring::protocol;


std::promise<void> FirmataQuery::callback_signal;
std::future<void> FirmataQuery::callback_gate = callback_signal.get_future();
void * FirmataQuery::contract_ready_callback_context = nullptr;
std::atomic_bool FirmataQuery::contract_ready(false);
FirmataQuery::contractReady FirmataQuery::contractReadyCallback = nullptr;
std::atomic_bool FirmataQuery::firmata_ready(false);
pin_config_t * FirmataQuery::pin = nullptr;
size_t FirmataQuery::pin_count = 0;

FirmataQuery::FirmataQuery (
    void
) :
    _parser_buffer(nullptr),
    _parser_buffer_size(0),
    _stream(nullptr)
{
}

FirmataQuery::~FirmataQuery (
    void
) {
    delete[](_parser_buffer);
}

DeviceContract *
FirmataQuery::detachDeviceContract (
    void
) {
    if ( !contract_ready || !pin ) { return nullptr; }

    return (new FirmataContract(pin, pin_count));
}

void
FirmataQuery::firmataReadyCallback (
    void * context_
) {
    callback_signal.set_value();
}

Stream *
FirmataQuery::getStream (
    void
) const {
    return _stream;
}

int
FirmataQuery::queryContractAsync (
    Stream * stream_,
    contractReady contractReadyCallback_,
    void * contract_ready_callback_context_
) {
    int error;

    // Allocate the parser buffer
    if ( NULL == (_parser_buffer = new uint8_t[MAX_DATA_BYTES]/*reinterpret_cast<uint8_t *>(malloc(MAX_DATA_BYTES))*/) ) {
        error = __LINE__;
    } else if ( 0 != _parser.setDataBufferOfSize(_parser_buffer, MAX_DATA_BYTES) ) {
        error = __LINE__;
    } else {
        // Store user supplied variables
        _stream = stream_;
        contractReadyCallback = contractReadyCallback_;
        contract_ready_callback_context = contract_ready_callback_context_;

        // Update state
        _parser_buffer_size = MAX_DATA_BYTES;

        // Register callbacks
        _stream->registerSerialEventCallback(FirmataQuery::serialEventCallback, this);
        _parser.attach(FirmataQuery::extendBuffer, this);
        _parser.attach(REPORT_VERSION, FirmataQuery::firmataReadyCallback, this);
        _parser.attach(START_SYSEX, FirmataQuery::queryResponseCallback, this);

        // Invoke the marshaller
        _marshaller.begin(*_stream);
        if ( std::future_status::ready != callback_gate.wait_for(std::chrono::milliseconds(10000)) ) {
            error = __LINE__;
        } else {
            _marshaller.sendCapabilityQuery();
            error = 0;
        }
    }

    return error;
}

void
FirmataQuery::queryResponseCallback (
    void * context_,
    uint8_t command_,
    size_t argc_,
    uint8_t * argv_
) {
    ConfigCodec codec;
    bool analog_resolution = false;
    bool mode_byte = true;
    bool pwm_resolution = false;
    FirmataQuery * this_query = (FirmataQuery *)context_;

    switch (command_) {
      case CAPABILITY_RESPONSE:
        std::cout << std::endl;
        std::cout << std::endl;
        pin_count = 0;
        codec.data = 0;

        // Parse capability response into device contract struct
        for (size_t i = 0 ; i < argc_ ; ++i, mode_byte = !mode_byte) {
            //TODO: Remove print functionality from library and `#include`s
            printf("0x%02x ", argv_[i]);
            if ( mode_byte ) {
                switch (argv_[i]) {
                  case PIN_MODE_ANALOG:
                    codec.config.supported_modes |= ANALOG_READ;
                    analog_resolution = true;
                    break;
                  case PIN_MODE_IGNORE:
                    printf("\nPinConfig %u:\n\tsupported modes: 0x%02x\n\tanalog read resolution bits: %u\n\tanalog write resolution bits: %u\n\n", static_cast<unsigned int>(pin_count), static_cast<uint8_t>(codec.config.supported_modes), static_cast<uint8_t>(codec.config.analog_read_resolution_bits), static_cast<uint8_t>(codec.config.analog_write_resolution_bits));
                    ++pin_count;
                    pin = (pin_config_t *)realloc(pin, (sizeof(pin_config_t) * pin_count));
                    pin[(pin_count - 1)] = codec.data;
                    codec.data = 0;
                    mode_byte = !mode_byte;
                    break;
                  case PIN_MODE_INPUT:
                    codec.config.supported_modes |= DIGITAL_READ;
                    break;
                  case PIN_MODE_OUTPUT:
                    codec.config.supported_modes |= DIGITAL_WRITE;
                    break;
                  case PIN_MODE_PULLUP:
                    codec.config.supported_modes |= DIGITAL_READ_WITH_PULLUP;
                    break;
                  case PIN_MODE_PWM:
                    codec.config.supported_modes |= ANALOG_WRITE;
                    pwm_resolution = true;
                    break;
                }
            } else if ( analog_resolution ) {
                codec.config.analog_read_resolution_bits = argv_[i];
                analog_resolution = false;
            } else if ( pwm_resolution ) {
                codec.config.analog_write_resolution_bits = argv_[i];
                pwm_resolution = false;
            }
        }

        // Query analog pin mapping
        this_query->_marshaller.sendAnalogMappingQuery();
        break;
      case ANALOG_MAPPING_RESPONSE:
        std::cout << std::endl;
        std::cout << std::endl;
        codec.data = 0;

        // Parse analog mapping response into device contract struct
        for (size_t i = 0 ; i < argc_ ; ++i) {
            //TODO: Remove print functionality from library and `#include`s
            printf("0x%02x ", argv_[i]);
            codec.data = pin[i];
            codec.config.reserved = argv_[i];
            pin[i] = codec.data;
        }
        std::cout << std::endl;

        contract_ready = true;
        if ( NULL != contractReadyCallback ) { contractReadyCallback(contract_ready_callback_context); }
        break;
      default: break;
    }
}

void
FirmataQuery::extendBuffer (
    void * context_
) {
    FirmataQuery * query = reinterpret_cast<FirmataQuery *>(context_);

    std::cout << std::endl << query->_parser_buffer_size << "-byte buffer exhausted!" << std::endl;
    uint8_t * temp_buffer;
    size_t temp_buffer_size = (query->_parser_buffer_size * 2);

    // Double parser buffer allocation
    temp_buffer = reinterpret_cast<uint8_t *>(realloc(query->_parser_buffer, temp_buffer_size));
    if ( NULL != temp_buffer ) {
        query->_parser_buffer = temp_buffer;
        query->_parser_buffer_size = temp_buffer_size;
        (void)query->_parser.setDataBufferOfSize(query->_parser_buffer, query->_parser_buffer_size);
        std::cout << "Buffer increased to " << query->_parser_buffer_size << "-byte buffer." << std::endl;
    }
}

void
FirmataQuery::processFirmataStream (
    void
) {
    if ( nullptr == _stream ) { return; }
    uint8_t incoming_byte = '\0';
    for (; _stream->available() ; _parser.parse(incoming_byte)) {
        incoming_byte = _stream->read();
        printf("0x%02x ", incoming_byte);
    }
}

void
FirmataQuery::serialEventCallback (
    void * context_
) {
    reinterpret_cast<FirmataQuery *>(context_)->processFirmataStream();
}

/* Created and copyrighted by Zachary J. Fields. Offered as open source under the MIT License (MIT). */
