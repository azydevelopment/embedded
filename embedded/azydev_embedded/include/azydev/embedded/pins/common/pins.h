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

#include <stdint.h>

template<typename PIN_TYPE>
class CPins
{
public:
    enum class DIRECTION : uint8_t
    {
        UNDEFINED = 255,
        IN        = 0,
        OUT       = 1
    };

    enum class PULL : uint8_t
    {
        NONE = 0,
        UP   = 1,
        DOWN = 2
    };

    enum class DIGITAL_STATE : uint8_t
    {
        UNDEFINED    = 255,
        DIGITAL_LOW  = 0,
        DIGITAL_HIGH = 1
    };

    enum class INTERRUPT_TRIGGER : uint8_t
    {
        UNDEFINED       = 255,
        TRIGGER_CHANGE  = 1,
        TRIGGER_FALLING = 2,
        TRIGGER_RISING  = 3
    };

    struct CONFIG_DESC
    {
        DIRECTION direction = DIRECTION::UNDEFINED;
        PULL pull           = PULL::NONE;
    };

    // destructor
    virtual ~CPins();

    // NVI
    void SetPinConfig(const PIN_TYPE pinId, const CONFIG_DESC&);
    void PinWriteDigital(const PIN_TYPE pinId, const DIGITAL_STATE);
    uint8_t PinReadDigital(const PIN_TYPE pinId) const;
    void EnableInterrupt(const PIN_TYPE pinId, const INTERRUPT_TRIGGER, const bool ignorePending);
    void DisableInterrupt(const PIN_TYPE pinId);

protected:
    // constructor
    CPins();

private:
    // rule of three
    CPins(const CPins&);
    CPins& operator=(const CPins&);

    // abstract
    virtual void SetPinConfig_impl(const PIN_TYPE pinId, const CONFIG_DESC&)     = 0;
    virtual void PinWriteDigital_impl(const PIN_TYPE pinId, const DIGITAL_STATE) = 0;
    virtual uint8_t PinReadDigital_impl(const PIN_TYPE pinId) const = 0;
    virtual void EnableInterrupt_impl(
        const PIN_TYPE pinId,
        const INTERRUPT_TRIGGER,
        const bool ignorePending)                            = 0;
    virtual void DisableInterrupt_impl(const PIN_TYPE pinId) = 0;
};