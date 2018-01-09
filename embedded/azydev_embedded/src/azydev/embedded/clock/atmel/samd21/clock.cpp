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

#include <azydev/embedded/clock/atmel/samd21/clock.h>

#include <asf.h>

/* PUBLIC */

CClockAtmelSAMD21::CClockAtmelSAMD21(const DESC& desc)
    : CClock(desc)
    , m_desc(desc)
    , m_config({}) {
}

CClockAtmelSAMD21::~CClockAtmelSAMD21() {
}

/* PRIVATE */

// static functions

void CClockAtmelSAMD21::EnableClock(const CLOCK_AHB clockIndex, const bool enabled) {
    uint32_t mask = 1 << static_cast<uint8_t>(clockIndex);
    if (enabled) {
        PM->AHBMASK.reg |= mask;
    } else {
        PM->AHBMASK.reg &= ~mask;
    }
}

void CClockAtmelSAMD21::EnableClock(const CLOCK_APBA clockIndex, const bool enabled) {
    uint32_t mask = 1 << static_cast<uint8_t>(clockIndex);
    if (enabled) {
        PM->APBAMASK.reg |= mask;
    } else {
        PM->APBAMASK.reg &= ~mask;
    }
}

void CClockAtmelSAMD21::EnableClock(const CLOCK_APBB clockIndex, const bool enabled) {
    uint32_t mask = 1 << static_cast<uint8_t>(clockIndex);
    if (enabled) {
        PM->APBBMASK.reg |= mask;
    } else {
        PM->APBBMASK.reg &= ~mask;
    }
}

void CClockAtmelSAMD21::EnableClock(const CLOCK_APBC clockIndex, const bool enabled) {
    uint32_t mask = 1 << static_cast<uint8_t>(clockIndex);
    if (enabled) {
        PM->APBCMASK.reg |= mask;
    } else {
        PM->APBCMASK.reg &= ~mask;
    }
}

// CClock

void CClockAtmelSAMD21::SetConfig_impl(const CClock::CONFIG_DESC& config) {
    m_config = static_cast<const CONFIG_DESC&>(config);
}

void CClockAtmelSAMD21::SetEnabled_impl(const bool enabled) {
    // enable/disable PM clocks
    {
        if (m_desc.clock_ahb != CLOCK_AHB::UNDEFINED) {
            EnableClock(m_desc.clock_ahb, enabled);
        }

        if (m_desc.clock_apba != CLOCK_APBA::UNDEFINED) {
            EnableClock(m_desc.clock_apba, enabled);
        }

        if (m_desc.clock_apbb != CLOCK_APBB::UNDEFINED) {
            EnableClock(m_desc.clock_apbb, enabled);
        }

        if (m_desc.clock_apbc != CLOCK_APBC::UNDEFINED) {
            EnableClock(m_desc.clock_apbc, enabled);
        }
    }

    if (enabled) {
        // prepare a new gclk clkctrl register
        uint32_t clkctrl = 0;
        {
            // target this clock ID
            clkctrl |= GetId();

            // set the generator
            clkctrl |= static_cast<uint8_t>(m_config.generator) << 8;
        }

        // disable this gclk for now
        system_gclk_chan_disable(GetId());

        // write the new clkctrl register config
        GCLK->CLKCTRL.reg = clkctrl;

        // reenable the gclk
        system_gclk_chan_enable(GetId());

        // TODO IMPLEMENT: Need this sometimes when enabling SERCOM clocks?
        // sercom_set_gclk_generator(gclk_chan_conf.source_generator, false);
    } else {
        system_gclk_chan_disable(GetId());
    }
}