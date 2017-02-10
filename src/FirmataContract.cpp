/* Created and copyrighted by Zachary J. Fields. Offered as open source under the MIT License (MIT). */

#include "FirmataContract.h"

using namespace remote_wiring::protocol;

const pin_config_t remote_wiring::protocol::ANALOG_READ = 0x01;
const pin_config_t remote_wiring::protocol::ANALOG_WRITE = 0x02;
const pin_config_t remote_wiring::protocol::DIGITAL_READ = 0x04;
const pin_config_t remote_wiring::protocol::DIGITAL_READ_WITH_PULLUP = 0x08;
const pin_config_t remote_wiring::protocol::DIGITAL_WRITE = 0x10;

ConfigCodec FirmataContract::codec;

FirmataContract::FirmataContract (
    const pin_config_t * const pin_data_,
    const size_t pin_count_
) :
    _pin_data(pin_data_),
    _pin_count(pin_count_)
{
}

bool
FirmataContract::analogReadAvailableOnPin (
    const size_t pin_
) const {
    return capabilityAvailableOnPin(ANALOG_READ, pin_);
}

size_t
FirmataContract::analogReadBitsOfResolutionForPin (
    const size_t pin_
) const {
    return decodePinConfigFromData(_pin_data[pin_]).analog_read_resolution_bits;
}

bool
FirmataContract::analogWriteAvailableOnPin (
    const size_t pin_
) const {
    return capabilityAvailableOnPin(ANALOG_WRITE, pin_);
}

size_t
FirmataContract::analogWriteBitsOfResolutionForPin (
    const size_t pin_
) const {
    return decodePinConfigFromData(_pin_data[pin_]).analog_write_resolution_bits;
}

bool
FirmataContract::digitalReadAvailableOnPin (
    const size_t pin_
) const {
    return capabilityAvailableOnPin(DIGITAL_READ, pin_);
}

bool
FirmataContract::digitalReadPullupAvailableOnPin (
    const size_t pin_
) const {
    return capabilityAvailableOnPin(DIGITAL_READ_WITH_PULLUP, pin_);
}

bool
FirmataContract::digitalWriteAvailableOnPin (
    const size_t pin_
) const {
    return capabilityAvailableOnPin(DIGITAL_WRITE, pin_);
}

size_t
FirmataContract::pinCount (
    void
) const {
    return _pin_count;
}

/* Created and copyrighted by Zachary J. Fields. Offered as open source under the MIT License (MIT). */
