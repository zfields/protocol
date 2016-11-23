/* Created and copyrighted by Zachary J. Fields. Offered as open source under the MIT License (MIT). */

#ifndef DEVICE_CONTRACT_H
#define DEVICE_CONTRACT_H

#include <cstddef>
#include <cstdint>

namespace remote_wiring {
namespace protocol {

typedef uint_fast32_t pin_config_t;

extern const pin_config_t ANALOG_READ;
extern const pin_config_t ANALOG_WRITE;
extern const pin_config_t DIGITAL_READ;
extern const pin_config_t DIGITAL_READ_WITH_PULLUP;
extern const pin_config_t DIGITAL_WRITE;

/*!
 * \brief Describes the capabilities and configuration of a pin
 *
 * PinConfig is static data describing the capabilities of each
 * pin. This information describes the modes of operation supported
 * on each pin as well as the read and write resolution of analog
 * operations for that pin if they exist.
 */
typedef struct PinConfig {
    pin_config_t supported_modes : 8;
    pin_config_t analog_read_resolution_bits : 8;
    pin_config_t analog_write_resolution_bits : 8;
    pin_config_t reserved : 8;
} PinConfig;

/*!
 * \brief Encodes and decodes the configuration parameters
 *
 * To encode set the `config` member to a `PinConfig` class whose fields
 * have been properly set, and read the `data` member. Conversely, to decode,
 * set the `data` member with encoded data and read the `config` member.
 */
union ConfigCodec {
    pin_config_t data;
    PinConfig config;
};

/*!
 * \brief A DeviceContract describes the capabilities of the remote device
 *
 * A device contract abstracts the capabilties of the remote device from the
 * protocol used to invoke those capabilities. This provides a generic way to
 * test the capabilities of the board, regardless of the underlying protocol.
 */
struct DeviceContract {
    virtual
    ~DeviceContract (
        void
    ) {

    }

    /*!
     * \brief Describes whether a pin has an analog read capability
     *
     * \param [in] pin_ The number of the pin to test
     *
     * \return A `bool` that indicates `true` when the capability is
     *         available and `false` when the capabilty is unavailable
     */
    virtual
    bool
    analogReadAvailableOnPin (
        const size_t pin_
    ) const = 0;

    /*!
     * \brief Describes the bits of resolution associated with analog read
     *
     * \param [in] pin_ The number of the pin to check
     *
     * \return An integer that indicates the number of bits used to
     *         describe the precision of the analog read value.
     *
     * \note The Arduino Uno has a 10-bit analog to digital converter (ADC),
     *       which allows for a reading range of [0, 1023].
     */
    virtual
    size_t
    analogReadBitsOfResolutionForPin (
        const size_t pin_
    ) const = 0;

    /*!
     * \brief Describes whether a pin has an analog write capability
     *
     * \param [in] pin_ The number of the pin to test
     *
     * \return A `bool` that indicates `true` when the capability is
     *         available and `false` when the capabilty is unavailable
     */
    virtual
    bool
    analogWriteAvailableOnPin (
        const size_t pin_
    ) const = 0;

    /*!
     * \brief Describes the bits of resolution associated with analog write
     *
     * \param [in] pin_ The number of the pin to check
     *
     * \return An integer that indicates the number of bits used to
     *         describe the precision of the analog write value.
     *
     * \note The Arduino Uno uses 8-bit resolution (default) when invoking
     *       pins with the pulse-width modulation (PWM) capability. 8 bits
     *       allows for a writing range of [0, 255].
     */
    virtual
    size_t
    analogWriteBitsOfResolutionForPin (
        const size_t pin_
    ) const = 0;

    /*!
     * \brief Describes whether a pin has a digital read capability
     *
     * \param [in] pin_ The number of the pin to test
     *
     * \return A `bool` that indicates `true` when the capability is
     *         available and `false` when the capabilty is unavailable
     */
    virtual
    bool
    digitalReadAvailableOnPin (
        const size_t pin_
    ) const = 0;

    /*!
     * \brief Describes whether a pin has a built-in pullup resistor
     *
     * \param [in] pin_ The number of the pin to test
     *
     * \return A `bool` that indicates `true` when a pullup resistor is
     *         available and `false` when a pullup resistor is unavailable
     *
     * \note A pullup resistor is used to insulate digital reads from
     *       environmental noise. When a pullup resistor is employed
     *       the digital pin will be tested with for LOW.
     */
    virtual
    bool
    digitalReadPullupAvailableOnPin (
        const size_t pin_
    ) const = 0;

    /*!
     * \brief Describes whether a pin has a digital write capability
     *
     * \param [in] pin_ The number of the pin to test
     *
     * \return A `bool` that indicates `true` when the capability is
     *         available and `false` when the capabilty is unavailable
     */
    virtual
    bool
    digitalWriteAvailableOnPin (
        const size_t pin_
    ) const = 0;
};

} // protocol
} // remote_wiring

#endif // DEVICE_CONTRACT_H

/* Created and copyrighted by Zachary J. Fields. Offered as open source under the MIT License (MIT). */
