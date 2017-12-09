/* The MIT License (MIT)
*
* Copyright (c) 2017 Andrew Yeung <azy.development@gmail.com>
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE. */

#include <azydev/embedded/bus/spi/common/bus.h>

#include <azydev/embedded/bus/spi/common/device.h>

/* PUBLIC */

// destructor

template<typename TRANSFER_PRIMITIVE>
CSPIBus<TRANSFER_PRIMITIVE>::~CSPIBus() {
}

// NVI

template<typename TRANSFER_PRIMITIVE>
void CSPIBus<TRANSFER_PRIMITIVE>::SetConfig(const CONFIG_DESC& config) {
    SetConfig_impl(config);
}

template<typename TRANSFER_PRIMITIVE>
CSPIEntity::STATUS
CSPIBus<TRANSFER_PRIMITIVE>::SetDeviceRole(const uint8_t deviceId, const ROLE role) {
    // TODO HACK: Device ID shouldn't necessarily be the position in an array
    return m_devices[deviceId]->SetRole(role);
}

template<typename TRANSFER_PRIMITIVE>
CSPIEntity::STATUS CSPIBus<TRANSFER_PRIMITIVE>::SetEnabled(const bool enable) {
    // TODO ERROR_HANDLING: Anything to do if we're already in the requested state?
    STATUS status = SetEnabled_impl(enable);
    m_enabled     = enable;
    return status;
}

template<typename TRANSFER_PRIMITIVE>
CSPIEntity::STATUS CSPIBus<TRANSFER_PRIMITIVE>::SetDuplexMode(const DUPLEX_MODE mode) {
    STATUS status = SetDuplexMode_impl(mode);
    m_duplex_mode = mode;
    return status;
}

template<typename TRANSFER_PRIMITIVE>
CSPIEntity::STATUS CSPIBus<TRANSFER_PRIMITIVE>::Start(const uint8_t deviceId) {
    // TODO ERROR_HANDLING: Disallow transaction initiation when in worker mode
    // TODO ERROR_HANDLING: Check for already active device
    // TODO HACK: Device ID is not necessarily the index.  Should find a way to do hashmaps.

    STATUS status      = m_devices[deviceId]->SetActive(true);
    status             = Start_impl(deviceId);
    m_active_device_id = deviceId;
    return status;
}

template<typename TRANSFER_PRIMITIVE>
CSPIEntity::STATUS CSPIBus<TRANSFER_PRIMITIVE>::Write(const TRANSFER_PRIMITIVE data) {
    return Write_impl(data);
}

template<typename TRANSFER_PRIMITIVE>
CSPIEntity::STATUS CSPIBus<TRANSFER_PRIMITIVE>::Read(TRANSFER_PRIMITIVE& outData) {
    return Read_impl(outData);
}

template<typename TRANSFER_PRIMITIVE>
CSPIEntity::STATUS CSPIBus<TRANSFER_PRIMITIVE>::Stop() {
    // TODO ERROR_HANDLING: Disallow transaction initiation when in worker mode (ending cannot happen
    // if starting never did)
    // TODO ERROR_HANDLING: Check for active device
    // TODO HACK: Device ID is not necessarily the index.  Should find a way to do hashmaps.
    STATUS status      = Stop_impl();
    status             = m_devices[m_active_device_id]->SetActive(false);
    m_active_device_id = 255;
    return status;
}

/* PROTECTED */

template<typename TRANSFER_PRIMITIVE>
CSPIBus<TRANSFER_PRIMITIVE>::CSPIBus(const DESC& desc)
    : CSPIEntity(desc)
    , m_num_devices(desc.num_devices)
    , m_devices(desc.devices)
    , m_enabled(false)
    , m_active_device_id(255)
    , m_duplex_mode(DUPLEX_MODE::UNDEFINED) {
}

/* FORWARD DECLARED TEMPLATES */
template class CSPIBus<uint8_t>;
template class CSPIBus<uint16_t>;