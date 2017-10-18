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