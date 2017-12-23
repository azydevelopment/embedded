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
    , m_bus(desc.bus)
    , m_pm_index(desc.pm_index)
    , m_config({}) {
}

CClockAtmelSAMD21::~CClockAtmelSAMD21() {
}

/* PRIVATE */

// static functions

void CClockAtmelSAMD21::SetPmMask(const BUS bus, const uint32_t pmIndex, const bool enabled) {
    uint32_t mask = 1 << pmIndex;
    if (enabled) {
        switch (bus) {
        case BUS::AHB:
            PM->AHBMASK.reg |= mask;
            break;
        case BUS::APBA:
            PM->APBAMASK.reg |= mask;
            break;
        case BUS::APBB:
            PM->APBBMASK.reg |= mask;
            break;
        case BUS::APBC:
            PM->APBCMASK.reg |= mask;
            break;
        default:
            // TODO ERROR_HANDLING
            break;
        }
    } else {
        switch (bus) {
        case BUS::AHB:
            PM->AHBMASK.reg &= ~mask;
            break;
        case BUS::APBA:
            PM->APBAMASK.reg &= ~mask;
            break;
        case BUS::APBB:
            PM->APBBMASK.reg &= ~mask;
            break;
        case BUS::APBC:
            PM->APBCMASK.reg &= ~mask;
            break;
        default:
            // TODO ERROR_HANDLING
            break;
        }
    }
}

// CClock

void CClockAtmelSAMD21::SetConfig_impl(const CClock::CONFIG_DESC& config) {
    m_config = static_cast<const CONFIG_DESC&>(config);
}

void CClockAtmelSAMD21::SetEnabled_impl(const bool enabled) {
    if (enabled) {
        // enable PM
        SetPmMask(m_bus, m_pm_index, true);

        struct system_gclk_chan_config gclk_chan_conf;

        // TODO HACK: static cast to ASF type
        gclk_chan_conf.source_generator =
            static_cast<gclk_generator>(m_config.clock_source_generator);
        system_gclk_chan_set_config(GetId(), &gclk_chan_conf);
        system_gclk_chan_enable(GetId());

        // TODO IMPLEMENT: Need this sometimes when enabling SERCOM clocks?
        // sercom_set_gclk_generator(gclk_chan_conf.source_generator, false);
    } else {
        // disable PM
        SetPmMask(m_bus, m_pm_index, false);

        system_gclk_chan_disable(GetId());
    }
}