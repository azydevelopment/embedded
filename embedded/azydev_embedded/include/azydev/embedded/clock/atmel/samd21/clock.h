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

#include <azydev/embedded/clock/common/clock.h>

#include <stdint.h>

class CClockAtmelSAMD21 final : public CClock
{
public:
    enum class BUS : uint8_t
    {
        UNDEFINED,
        AHB,
        APBA,
        APBB,
        APBC
    };

    struct DESC : CClock::DESC
    {
        // TODO HACK: Move PM index elsewhere?
        BUS bus           = BUS::UNDEFINED;
        uint32_t pm_index = 0;
    };

    // TODO IMPLEMENT: Clock division control
    struct CONFIG_DESC : CClock::CONFIG_DESC
    { uint8_t clock_source_generator = 0; };

    // constructor
    CClockAtmelSAMD21(const DESC&);

    // destructor
    virtual ~CClockAtmelSAMD21() override final;

private:
    // rule of three
    CClockAtmelSAMD21(const CClockAtmelSAMD21&);
    CClockAtmelSAMD21& operator=(const CClockAtmelSAMD21&);

    // member variables
    BUS m_bus;
    uint32_t m_pm_index;
    CONFIG_DESC m_config;

    // static functions
    static void SetPmMask(const BUS, const uint32_t pmIndex, const bool enabled);

    // CClock
    virtual void SetConfig_impl(const CClock::CONFIG_DESC&) override final;
    virtual void SetEnabled_impl(const bool) override final;
};