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
//#pragma once
//
//#include <azydev/embedded/util/binary.h>
//
//#include <azydev/embedded/bus/spi/atmel/avr8/device.h>
//#include <azydev/embedded/bus/spi/atmel/common/bus.h>
//
//template<typename PIN_TYPE>
//class CPins;
//
//class CSPIBusAtmelAVR8 : public CSPIBusAtmel<uint8_t, uint8_t>
//{
//public:
//    enum class CLOCK_RATE_DIVIDER : uint8_t
//    {
//        UNDEFINED  = 255,
//        DIVIDE_4   = 1 << 2,
//        DIVIDE_16  = 1 << 4,
//        DIVIDE_64  = 1 << 6,
//        DIVIDE_128 = 1 << 7
//    };
//
//    enum class CLOCK_RATE_MULTIPLIER : uint8_t
//    {
//        UNDEFINED  = 255,
//        NONE       = 0,
//        MULTIPLY_2 = 1 << 2
//    };
//
//    struct REGISTER_CONFIG_DESC
//    {
//        volatile uint8_t* status_register          = nullptr;
//        volatile uint8_t* data_register            = nullptr;
//        BIT_POINTER<uint8_t> interrupt_flag        = {};
//        BIT_POINTER<uint8_t> enable                = {};
//        BIT_POINTER<uint8_t> role                  = {};
//        BIT_POINTER<uint8_t> endianness            = {};
//        BIT_POINTER<uint8_t> clock_polarity        = {};
//        BIT_POINTER<uint8_t> clock_phase           = {};
//        BIT_POINTER<uint8_t> clock_rate_divider[2] = {};
//        BIT_POINTER<uint8_t> clock_rate_multiplier = {};
//    };
//
//    struct CONFIG_DESC : CSPIBusAtmel<uint8_t, uint8_t>::CONFIG_DESC
//    {
//        CLOCK_RATE_DIVIDER clock_rate_divider       = CLOCK_RATE_DIVIDER::UNDEFINED;
//        CLOCK_RATE_MULTIPLIER clock_rate_multiplier = CLOCK_RATE_MULTIPLIER::UNDEFINED;
//    };
//
//    struct PIN_CONFIG_DESC
//    {
//        uint8_t sclk = 255;
//        uint8_t mosi = 255;
//        uint8_t miso = 255;
//        uint8_t ss   = 255;
//    };
//
//    struct DESC : CSPIBusAtmel<uint8_t, uint8_t>::DESC
//    {
//        PIN_CONFIG_DESC pin_config           = {};
//        REGISTER_CONFIG_DESC register_config = {};
//    };
//
//    CSPIBusAtmelAVR8(const DESC&, CPins<uint8_t>&);
//    virtual ~CSPIBusAtmelAVR8() override;
//
//    // CSPIBus
//    virtual void SetEnabled(bool) override;
//    virtual void SetConfig(const CSPIBus::CONFIG_DESC&) override;
//    virtual void SetDuplexMode(DUPLEX_MODE) override;
//    virtual uint8_t PollMiso() override;
//    virtual uint8_t Read() override;
//    virtual void Write(uint8_t) override;
//    virtual void WaitForTransfer() override;
//
//private:
//    CPins<uint8_t>& m_service_pins;
//
//    PIN_CONFIG_DESC m_pin_config;
//    REGISTER_CONFIG_DESC m_register_config;
//    CONFIG_DESC m_bus_config;
//    DUPLEX_MODE m_duplex_mode;
//};
