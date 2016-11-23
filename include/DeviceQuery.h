/* Created and copyrighted by Zachary J. Fields. Offered as open source under the MIT License (MIT). */

#ifndef DEVICE_QUERY_H
#define DEVICE_QUERY_H

#include "Stream.h"

namespace remote_wiring {
namespace protocol {

class DeviceContract;

/*!
 * \brief An interface for requesting and processing the capabilities of a remote device.
 */
struct DeviceQuery {
    typedef void(*contractReady)(void * context_);

    virtual
    ~DeviceQuery (
        void
    ) {

    }

    /*!
     * \brief Query the device capability contract
     *
     * \param [in] stream_ The underlying serial stream that provides a connection
     *                     to the remote device
     * \param [in] contractReadyCallback_ A callback to indicate the device contract
     *                                    has been acquired from the remote device
     * \param [in] callback_context_ A context supplied to the callback when called
     *
     * \return If an error occurred, then a non-zero value will be returned.
     */
    virtual
    int
    queryContractAsync (
        Stream * stream_,
        contractReady contractReadyCallback_,
        void * callback_context_
    ) = 0;

    /*!
     * \brief Detach the device contract from the query
     *
     * \return A `DeviceContract` interface that describes the capabilities of
     *         each pin or `nullptr` when an error occurs (i.e. `queryAsync`
     *         was not previously called)
     *
     * \warning The caller will own the memory at the DeviceContract pointer
     *          that is returned, and will be responsible for calling `delete()`
     *          once the pointer once the object is no longer required.
     *
     * \sa remote_wiring::protocol::DeviceContract
     */
    virtual
    DeviceContract *
    detachDeviceContract (
        void
    ) = 0;

    /*!
     * \brief Get the pointer to the underlying stream
     *
     * \return A pointer to the stream supplied to the `beginAsync` method
     *
     * \sa DeviceQuery::beginAsync
     */
    virtual
    Stream *
    getStream (
        void
    ) const = 0;
};

} // protocol
} // remote_wiring

#endif // DEVICE_QUERY_H

/* Created and copyrighted by Zachary J. Fields. Offered as open source under the MIT License (MIT). */
