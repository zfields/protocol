/* Created and copyrighted by Zachary J. Fields. Offered as open source under the MIT License (MIT). */

#ifndef FIRMATA_CONTRACT_H
#define FIRMATA_CONTRACT_H

#include <cstddef>
#include <cstdint>

#include "DeviceContract.h"

namespace remote_wiring {
namespace protocol {

class FirmataQuery;

class FirmataContract : public DeviceContract {
  friend FirmataQuery;
  public:
    bool
    analogReadAvailableOnPin (
        const size_t pin_
    ) const override;

    size_t
    analogReadBitsOfResolutionForPin (
        const size_t pin_
    ) const override;

    bool
    analogWriteAvailableOnPin (
        const size_t pin_
    ) const override;

    size_t
    analogWriteBitsOfResolutionForPin (
        const size_t pin_
    ) const override;

    bool
    digitalReadAvailableOnPin (
        const size_t pin_
    ) const override;

    bool
    digitalReadPullupAvailableOnPin (
        const size_t pin_
    ) const override;

    bool
    digitalWriteAvailableOnPin (
        const size_t pin_
    ) const override;

    size_t
    pinCount (
       void
    ) const override;

  private:
    FirmataContract (
        const pin_config_t * const pin_data_,
        const size_t pin_count_
    );

    inline
    bool
    capabilityAvailableOnPin (
        const size_t capability_,
        const size_t pin_
    ) const {
        return ((pin_ > _pin_count) && (0x00 != (decodePinConfigFromData(_pin_data[(pin_ - 1)]).supported_modes & capability_)));
    }

    const pin_config_t * const _pin_data;
    const size_t _pin_count;

    static inline
    PinConfig
    decodePinConfigFromData (
        pin_config_t data_
    ) {
        ConfigCodec codec = { data_ };
        return codec.config;
    }

    static inline
    pin_config_t
    encodeDataFromPinConfig (
        PinConfig config_
    ) {
        ConfigCodec codec = { *reinterpret_cast<pin_config_t *>(&config_) };
        return codec.data;
    }
};

} // protocol
} // remote_wiring

#endif // FIRMATA_CONTRACT_H

/* Created and copyrighted by Zachary J. Fields. Offered as open source under the MIT License (MIT). */
