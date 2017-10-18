/*
* Copyright (C) Andrew Yeung - All Rights Reserved
* Unauthorized copying of this file, via any medium is strictly prohibited
* Proprietary and confidential
* Written by Andrew Yeung <azy.development@gmail.com>, May 2017
*/

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
