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

#include <azydev/embedded/pins/atmel/samd21/pins.h>
#include <azydev/embedded/util/binary.h>

#include <asf.h>

/* PUBLIC */

CPinsAtmelSAMD21::CPinsAtmelSAMD21()
    : CPins<uint32_t>() {
}

CPinsAtmelSAMD21::~CPinsAtmelSAMD21() {
}

CPinsAtmelSAMD21::CPinsAtmelSAMD21(const CPinsAtmelSAMD21& other) {
}

/* PRIVATE  */

// CPins

void CPinsAtmelSAMD21::SetPinConfig_impl(const uint32_t pinId, const CPins::CONFIG_DESC& config) {
    // TODO HACK: Get rid of all of the ASF stuff

    uint32_t pin = pinId;

    if (pin != PINMUX_UNUSED) {
        const CONFIG_DESC& configDesc      = static_cast<const CONFIG_DESC&>(config);
        CPinsAtmelSAMD21::MUX_TYPE muxType = configDesc.mux_type;

        struct system_pinmux_config pinmuxConfig;
        pinmuxConfig.powersave = false;

        // set mux
        switch (muxType) {
        case CPinsAtmelSAMD21::MUX_TYPE::GPIO:
            pinmuxConfig.mux_position = SYSTEM_PINMUX_GPIO;
            break;
        case CPinsAtmelSAMD21::MUX_TYPE::PERIPHERAL:
            // TODO HACK: Apparently the mux position is in the lower 16 bits and the pin ID in the
            // upper 16 bits
            pinmuxConfig.mux_position = Binary::BM<uint32_t>(pin, 0, 16);
            pin                       = pin >> 16;
            break;
        default:
            // TODO ERROR_HANDLING
            break;
        }

        // set direction
        switch (configDesc.direction) {
        case CPins::DIRECTION::IN:
            pinmuxConfig.direction = SYSTEM_PINMUX_PIN_DIR_INPUT;
            break;
        case CPins::DIRECTION::OUT:
            pinmuxConfig.direction = SYSTEM_PINMUX_PIN_DIR_OUTPUT;
            break;
        default:
            // TODO ERROR_HANDLING
            break;
        }

        // set pull
        switch (configDesc.pull) {
        case CPins::PULL::UP:
            pinmuxConfig.input_pull = SYSTEM_PINMUX_PIN_PULL_UP;
            break;
        case CPins::PULL::DOWN:
            pinmuxConfig.input_pull = SYSTEM_PINMUX_PIN_PULL_DOWN;
            break;
        case CPins::PULL::NONE:
            pinmuxConfig.input_pull = SYSTEM_PINMUX_PIN_PULL_NONE;
            break;
        default:
            // TODO ERROR_HANDLING
            break;
        }

        system_pinmux_pin_set_config(pin, &pinmuxConfig);
    }
}

void CPinsAtmelSAMD21::PinWriteDigital_impl(
    const uint32_t pinId,
    const CPins::DIGITAL_STATE state) {
    switch (state) {
    case CPins::DIGITAL_STATE::DIGITAL_LOW:
        port_pin_set_output_level(pinId, false);
        break;
    case CPins::DIGITAL_STATE::DIGITAL_HIGH:
        port_pin_set_output_level(pinId, true);
        break;
    default:
        // TODO ERROR_HANDLING
        break;
    }
    // TODO IMPLEMENT
}

uint8_t CPinsAtmelSAMD21::PinReadDigital_impl(const uint32_t pinId) const {
    // TODO IMPLEMENT
    return 0;
}

void CPinsAtmelSAMD21::EnableInterrupt_impl(
    const uint32_t pinId,
    const CPins::INTERRUPT_TRIGGER trigger,
    const bool ignorePending) {
    // TODO IMPLEMENT
}

void CPinsAtmelSAMD21::DisableInterrupt_impl(const uint32_t pinId) {
    // TODO IMPLEMENT
}
