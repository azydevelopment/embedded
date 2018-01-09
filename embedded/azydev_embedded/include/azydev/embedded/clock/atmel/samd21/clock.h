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

#include <asf.h>

class CClockAtmelSAMD21 final : public CClock
{
public:
    enum class CLOCK_GCLK : uint8_t
    {
        CLOCK_DFLL48M_REF,
        CLOCK_DPLL,
        CLOCK_DPLL_32K,
        CLOCK_WDT,
        CLOCK_TRC,
        CLOCK_EIC,
        CLOCK_USB,
        CLOCK_EVSYS_CHANNEL_0,
        CLOCK_EVSYS_CHANNEL_1,
        CLOCK_EVSYS_CHANNEL_2,
        CLOCK_EVSYS_CHANNEL_3,
        CLOCK_EVSYS_CHANNEL_4,
        CLOCK_EVSYS_CHANNEL_5,
        CLOCK_EVSYS_CHANNEL_6,
        CLOCK_EVSYS_CHANNEL_7,
        CLOCK_EVSYS_CHANNEL_8,
        CLOCK_EVSYS_CHANNEL_9,
        CLOCK_EVSYS_CHANNEL_10,
        CLOCK_EVSYS_CHANNEL_11,
        CLOCK_SERCOMX_SLOW,
        CLOCK_SERCOM0_CORE,
        CLOCK_SERCOM1_CORE,
        CLOCK_SERCOM2_CORE,
        CLOCK_SERCOM3_CORE,
        CLOCK_SERCOM4_CORE,
        CLOCK_SERCOM5_CORE,
        CLOCK_TCC0_TCC1,
        CLOCK_TC2_TC3,
        CLOCK_TC4_TC5,
        CLOCK_TC6_TC7,
        CLOCK_ADC,
        CLOCK_AC_DIG,
        CLOCK_AC_ANA = 0x21,
        CLOCK_DAC    = 0x23,
        CLOCK_PTC,
        CLOCK_I2S_0,
        CLOCK_I2S_1,
        UNDEFINED = 0xFF
    };

    enum class CLOCK_AHB : uint8_t
    {
        CLOCK_HPB0,
        CLOCK_HPB1,
        CLOCK_HPB2,
        CLOCK_DSU,
        CLOCK_NVMCTRL,
        CLOCK_DMAC,
        CLOCK_USB,
        UNDEFINED = 0xFF
    };

    enum class CLOCK_APBA : uint8_t
    {
        CLOCK_PAC0,
        CLOCK_PM,
        CLOCK_SYSCTRL,
        CLOCK_GCLK,
        CLOCK_WDT,
        CLOCK_RTC,
        CLOCK_EIC,
        UNDEFINED = 0xFF
    };

    enum class CLOCK_APBB : uint8_t
    {
        CLOCK_PAC1,
        CLOCK_DSU,
        CLOCK_NVMCTRL,
        CLOCK_PORT,
        CLOCK_DMAC,
        CLOCK_USB,
        UNDEFINED = 0xFF
    };

    enum class CLOCK_APBC : uint8_t
    {
        CLOCK_PAC0,
        CLOCK_EVSYS,
        CLOCK_SERCOM0,
        CLOCK_SERCOM1,
        CLOCK_SERCOM2,
        CLOCK_SERCOM3,
        CLOCK_SERCOM4,
        CLOCK_SERCOM5,
        CLOCK_TCC0,
        CLOCK_TCC1,
        CLOCK_TCC2,
        CLOCK_TC3,
        CLOCK_TC4,
        CLOCK_TC5,
        CLOCK_TC6,
        CLOCK_TC7,
        CLOCK_ADC,
        CLOCK_AC,
        CLOCK_DAC,
        CLOCK_PTC,
        CLOCK_I2S,
        UNDEFINED = 255
    };

    enum class GCLK_GENERATOR : uint8_t
    {
        GEN0,
        GEN1,
        GEN2,
        GEN3,
        GEN4,
        GEN5,
        GEN6,
        GEN7,
        GEN8,
        UNDEFINED = 255
    };

    struct DESC : CClock::DESC
    {
        // TODO HACK: Move PM index elsewhere?
        CLOCK_GCLK clock_gclk = CLOCK_GCLK::UNDEFINED;
        CLOCK_AHB clock_ahb   = CLOCK_AHB::UNDEFINED;
        CLOCK_APBA clock_apba = CLOCK_APBA::UNDEFINED;
        CLOCK_APBB clock_apbb = CLOCK_APBB::UNDEFINED;
        CLOCK_APBC clock_apbc = CLOCK_APBC::UNDEFINED;
    };

    // TODO IMPLEMENT: Clock division control
    struct CONFIG_DESC : CClock::CONFIG_DESC
    { GCLK_GENERATOR gclk_generator = GCLK_GENERATOR::UNDEFINED; };

    // constructor
    CClockAtmelSAMD21(const DESC&);

    // destructor
    virtual ~CClockAtmelSAMD21() override final;

private:
    // rule of three
    CClockAtmelSAMD21(const CClockAtmelSAMD21&);
    CClockAtmelSAMD21& operator=(const CClockAtmelSAMD21&);

    // member variables
    const DESC m_desc;
    CONFIG_DESC m_config;

    // static functions
    static void EnableClock(const CLOCK_AHB, const bool enabled);
    static void EnableClock(const CLOCK_APBA, const bool enabled);
    static void EnableClock(const CLOCK_APBB, const bool enabled);
    static void EnableClock(const CLOCK_APBC, const bool enabled);

    // CClock
    virtual void SetConfig_impl(const CClock::CONFIG_DESC&) override final;
    virtual void SetEnabled_impl(const bool) override final;
};