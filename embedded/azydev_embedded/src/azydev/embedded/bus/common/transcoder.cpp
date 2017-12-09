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

#pragma once

#include <azydev/embedded/bus/common/transcoder.h>

#include <azydev/embedded/bus/spi/common/bus.h>

/* PUBLIC */

template<typename BUS_TYPE, typename DEVICE_ID_TYPE>
CBusTranscoder<BUS_TYPE, DEVICE_ID_TYPE>::~CBusTranscoder() {
}

// NVI

template<typename BUS_TYPE, typename DEVICE_ID_TYPE>
void CBusTranscoder<BUS_TYPE, DEVICE_ID_TYPE>::AttachToBus(
    BUS_TYPE& bus,
    const DEVICE_ID_TYPE deviceId) {
    m_bus       = &bus;
    m_device_id = deviceId;
}

/* PROTECTED */

template<typename BUS_TYPE, typename DEVICE_ID_TYPE>
CBusTranscoder<BUS_TYPE, DEVICE_ID_TYPE>::CBusTranscoder() {
}

template<typename BUS_TYPE, typename DEVICE_ID_TYPE>
BUS_TYPE& CBusTranscoder<BUS_TYPE, DEVICE_ID_TYPE>::GetBus() const {
    return *m_bus;
}

template<typename BUS_TYPE, typename DEVICE_ID_TYPE>
const DEVICE_ID_TYPE CBusTranscoder<BUS_TYPE, DEVICE_ID_TYPE>::GetDeviceId() const {
    return m_device_id;
}

/* FORWARD DECLARED TEMPLATES */
template class CBusTranscoder<CSPIBus<uint8_t>, uint8_t>;
template class CBusTranscoder<CSPIBus<uint16_t>, uint8_t>;
