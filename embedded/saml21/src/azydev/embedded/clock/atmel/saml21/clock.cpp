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

#include <azydev/embedded/clock/atmel/saml21/clock.h>

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

void CClockAtmelSAMD21::SetEnableClock(const CLOCK_AHB clock, const bool enabled) {
    uint32_t mask = 1 << static_cast<uint8_t>(clock);
    if (enabled) {
        MCLK->AHBMASK.reg |= mask;
    } else {
        MCLK->AHBMASK.reg &= ~mask;
    }
}

void CClockAtmelSAMD21::SetEnableClock(const CLOCK_APBA clock, const bool enabled) {
    uint32_t mask = 1 << static_cast<uint8_t>(clock);
    if (enabled) {
        MCLK->APBAMASK.reg |= mask;
    } else {
        MCLK->APBAMASK.reg &= ~mask;
    }
}

void CClockAtmelSAMD21::SetEnableClock(const CLOCK_APBB clock, const bool enabled) {
    uint32_t mask = 1 << static_cast<uint8_t>(clock);
    if (enabled) {
        MCLK->APBBMASK.reg |= mask;
    } else {
        MCLK->APBBMASK.reg &= ~mask;
    }
}

void CClockAtmelSAMD21::SetEnableClock(const CLOCK_APBC clock, const bool enabled) {
    uint32_t mask = 1 << static_cast<uint8_t>(clock);
    if (enabled) {
        MCLK->APBCMASK.reg |= mask;
    } else {
        MCLK->APBCMASK.reg &= ~mask;
    }
}

void CClockAtmelSAMD21::SetEnableClock(
    const CLOCK_GCLK clock,
    const GCLK_GENERATOR gclkGenerator,
    const bool enabled) {
    // TODO IMPLEMENT: Reimplement GLCK functionality

    // uint8_t gclkId = static_cast<uint8_t>(clock);
    //
    //// disable this gclk first; need to do this for both disabling and enabling
    //{
    //// select gclk
    // GCLK->CLKCTRL.reg = gclkId << static_cast<uint8_t>(REG_CLKCTRL::ID);
    //
    //// disable gclk
    // GCLK->CLKCTRL.reg &= ~(1 << static_cast<uint8_t>(REG_CLKCTRL::CLKEN));
    //
    //// wait until it's disabled
    // while (GCLK->CLKCTRL.reg & (1 << static_cast<uint8_t>(REG_CLKCTRL::CLKEN))) {
    //}
    //}
    //
    //// reenable it with a new config if requested
    // if (enabled) {
    //// write the new clkctrl register config
    //{
    //// prepare a new gclk clkctrl register
    // uint32_t clkctrl = 0;
    //{
    //// target this clock ID
    // clkctrl |= gclkId << static_cast<uint8_t>(REG_CLKCTRL::ID);
    //
    //// set the generator
    // clkctrl |= static_cast<uint8_t>(gclkGenerator) << static_cast<uint8_t>(REG_CLKCTRL::GEN);
    //}
    //
    //// actually write the new config
    // GCLK->CLKCTRL.reg = clkctrl;
    //}
    //
    //// reenable the gclk
    //{
    //// select gclk
    // GCLK->CLKCTRL.reg = gclkId << static_cast<uint8_t>(REG_CLKCTRL::ID);
    //
    //// enable gclk
    // GCLK->CLKCTRL.reg |= 1 << static_cast<uint8_t>(REG_CLKCTRL::CLKEN);
    //}
    //
    //// TODO IMPLEMENT: Need this sometimes when enabling SERCOM clocks?
    //// sercom_set_gclk_generator(gclk_chan_conf.source_generator, false);
    //}
}

// CClock

void CClockAtmelSAMD21::SetConfig_impl(const CClock::CONFIG_DESC& config) {
    m_config = static_cast<const CONFIG_DESC&>(config);
}

void CClockAtmelSAMD21::SetEnabled_impl(const bool enabled) {
    // enable/disable clocks
    {
        if (m_desc.clock_ahb != CLOCK_AHB::UNDEFINED) {
            SetEnableClock(m_desc.clock_ahb, enabled);
        }

        if (m_desc.clock_apba != CLOCK_APBA::UNDEFINED) {
            SetEnableClock(m_desc.clock_apba, enabled);
        }

        if (m_desc.clock_apbb != CLOCK_APBB::UNDEFINED) {
            SetEnableClock(m_desc.clock_apbb, enabled);
        }

        if (m_desc.clock_apbc != CLOCK_APBC::UNDEFINED) {
            SetEnableClock(m_desc.clock_apbc, enabled);
        }

        if (m_desc.clock_gclk != CLOCK_GCLK::UNDEFINED) {
            SetEnableClock(m_desc.clock_gclk, m_config.gclk_generator, enabled);
        }
    }
}