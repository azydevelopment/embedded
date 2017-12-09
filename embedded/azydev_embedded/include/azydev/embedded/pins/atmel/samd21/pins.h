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

#include <stdint.h>

class CPinsAtmelSAMD21 final : public CPins<uint32_t>
{
public:
    enum class MUX_TYPE : uint8_t
    {
        UNDEFINED  = 255,
        GPIO       = 1 << 7,
        PERIPHERAL = 0
    };

    struct CONFIG_DESC : CPins::CONFIG_DESC
    { MUX_TYPE mux_type = MUX_TYPE::UNDEFINED; };

    // constructor
    CPinsAtmelSAMD21();

    // destructor
    virtual ~CPinsAtmelSAMD21() override final;

    // copy constructor
    CPinsAtmelSAMD21(const CPinsAtmelSAMD21&);

private:
    // rule of three
    CPinsAtmelSAMD21& operator=(const CPinsAtmelSAMD21&);

    // CPins
    virtual void SetPinConfig_impl(const uint32_t pinId, const CPins::CONFIG_DESC&) override final;
    virtual void PinWriteDigital_impl(const uint32_t pinId, const DIGITAL_STATE) override final;
    virtual uint8_t PinReadDigital_impl(const uint32_t pinId) const override final;
    virtual void EnableInterrupt_impl(
        const uint32_t pinId,
        const INTERRUPT_TRIGGER,
        const bool ignorePending) override final;
    virtual void DisableInterrupt_impl(const uint32_t pinId) override final;
};