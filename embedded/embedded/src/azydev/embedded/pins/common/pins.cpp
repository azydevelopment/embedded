/*
* Copyright (C) Andrew Yeung - All Rights Reserved
* Unauthorized copying of this file, via any medium is strictly prohibited
* Proprietary and confidential
* Written by Andrew Yeung <azy.development@gmail.com>, May 2017
*/

#pragma once

#include <azydev/embedded/pins/common/pins.h>

/* PUBLIC */

template<typename PIN_TYPE>
CPins<PIN_TYPE>::~CPins() {
}

// NVI

template<typename PIN_TYPE>
void CPins<PIN_TYPE>::SetPinConfig(const PIN_TYPE pinId, const CONFIG_DESC& config) {
    SetPinConfig_impl(pinId, config);
}

template<typename PIN_TYPE>
void CPins<PIN_TYPE>::PinWriteDigital(const PIN_TYPE pinId, const DIGITAL_STATE state) {
    PinWriteDigital_impl(pinId, state);
}

template<typename PIN_TYPE>
uint8_t CPins<PIN_TYPE>::PinReadDigital(const PIN_TYPE pinId) const {
    return PinReadDigital_impl(pinId);
}

template<typename PIN_TYPE>
void CPins<PIN_TYPE>::EnableInterrupt(
    const PIN_TYPE pinId,
    const INTERRUPT_TRIGGER trigger,
    const bool ignorePending) {
    EnableInterrupt_impl(pinId, trigger, ignorePending);
}

template<typename PIN_TYPE>
void CPins<PIN_TYPE>::DisableInterrupt(const PIN_TYPE pinId) {
    DisableInterrupt_impl(pinId);
}

/* PROTECTED */

template<typename PIN_TYPE>
CPins<PIN_TYPE>::CPins() {
}

/* FORWARD DECLARE TEMPLATES */
template class CPins<uint32_t>;