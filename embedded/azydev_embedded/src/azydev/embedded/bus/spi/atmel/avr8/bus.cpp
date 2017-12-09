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
//#include <azydev/embedded/bus/spi/atmel/avr8/bus.h>
//
//#include <azydev/embedded/pins/common/pins.h>
//#include <azydev/embedded/util/binary.h>
//
//CSPIBusAtmelAVR8::CSPIBusAtmelAVR8(const DESC& desc, CPins<uint8_t>& servicePins)
//    : CSPIBusAtmel<uint8_t, uint8_t>(desc)
//    , m_service_pins(servicePins)
//    , m_pin_config(desc.pin_config)
//    , m_register_config(desc.register_config)
//    , m_bus_config({})
//    , m_duplex_mode(DUPLEX_MODE::UNDEFINED) {
//}
//
//CSPIBusAtmelAVR8::~CSPIBusAtmelAVR8() {
//}
//
//// CSPIBus
//
//void CSPIBusAtmelAVR8::SetEnabled(bool enable) {
//    // TODO ERROR_HANDLING: Check that the bus is configured to have a role other than UNDEFINED
//
//    // call the parent method
//    CSPIBus<uint8_t>::SetEnabled(enable);
//
//    // configure pins
//    CPins<uint8_t>::CONFIG_DESC configSclkSs = {};
//    CPins<uint8_t>::CONFIG_DESC configMiso   = {};
//
//    if (IsEnabled()) {
//        // configure pins based on role
//        switch (GetRole()) {
//        case ROLE::MANAGER:
//            // if we're a manager device, ensure the SS pin is high to stay in that mode
//            m_service_pins.PinWriteDigital(
//                m_pin_config.ss, CPins<uint8_t>::DIGITAL_STATE::DIGITAL_HIGH);
//
//            configSclkSs.direction = CPins<uint8_t>::DIRECTION::OUT;
//
//            configMiso.direction = CPins<uint8_t>::DIRECTION::IN;
//            configMiso.pull      = CPins<uint8_t>::PULL::UP;
//
//            break;
//        case ROLE::WORKER:
//            configSclkSs.direction = CPins<uint8_t>::DIRECTION::IN;
//            configSclkSs.pull      = CPins<uint8_t>::PULL::UP;
//
//            configMiso.direction = CPins<uint8_t>::DIRECTION::OUT;
//
//            break;
//        default:
//            // TODO ERROR_HANDLING
//            break;
//        }
//
//        // set the MOSI pin based on the duplex mode
//        SetDuplexMode(m_duplex_mode);
//
//    } else {
//        configSclkSs.direction = CPins<uint8_t>::DIRECTION::IN;
//        configSclkSs.pull      = CPins<uint8_t>::PULL::UP;
//
//        configMiso.direction = CPins<uint8_t>::DIRECTION::IN;
//        configMiso.pull      = CPins<uint8_t>::PULL::UP;
//
//        // if disabled, default to HALF_READ which sets MOSI as INPUT_PULLUP
//        SetDuplexMode(DUPLEX_MODE::HALF_READ);
//    }
//
//    m_service_pins.SetPinConfig(m_pin_config.sclk, configSclkSs);
//    m_service_pins.SetPinConfig(m_pin_config.miso, configMiso);
//    m_service_pins.SetPinConfig(m_pin_config.ss, configSclkSs);
//
//    REGISTER_CONFIG_DESC* reg = &m_register_config;
//
//    // enable SPI
//    Binary::BS<volatile uint8_t>(*(reg->enable.address), reg->enable.offset, true);
//
//    // set role
//    switch (GetRole()) {
//    case ROLE::WORKER:
//        Binary::BS<volatile uint8_t>(*(reg->role.address), reg->role.offset, false);
//        break;
//    case ROLE::MANAGER:
//        Binary::BS<volatile uint8_t>(*(reg->role.address), reg->role.offset, true);
//        break;
//    default:
//        // TODO ERROR_HANDLING
//        break;
//    }
//
//    // set endianness
//    switch (m_bus_config.endianness) {
//    case ENDIANNESS::BIG:
//        Binary::BS<volatile uint8_t>(*(reg->endianness.address), reg->endianness.offset, false);
//        break;
//    case ENDIANNESS::LITTLE:
//        Binary::BS<volatile uint8_t>(*(reg->endianness.address), reg->endianness.offset, true);
//        break;
//    default:
//        // TODO ERROR_HANDLING
//        break;
//    }
//
//    // set clock polarity
//    Binary::BS<volatile uint8_t>(
//        *(reg->clock_polarity.address),
//        reg->clock_polarity.offset,
//        static_cast<uint8_t>(m_bus_config.clock_polarity) != 0);
//
//    // set clock phase
//    Binary::BS<volatile uint8_t>(
//        *(reg->clock_phase.address),
//        reg->clock_phase.offset,
//        static_cast<uint8_t>(m_bus_config.clock_phase) != 0);
//
//    // set clock rate divider
//    switch (m_bus_config.clock_rate_divider) {
//    case CLOCK_RATE_DIVIDER::DIVIDE_4:
//        Binary::BS<volatile uint8_t>(
//            *(reg->clock_rate_divider[0].address), reg->clock_rate_divider[0].offset, false);
//        Binary::BS<volatile uint8_t>(
//            *(reg->clock_rate_divider[1].address), reg->clock_rate_divider[1].offset, false);
//        break;
//    case CLOCK_RATE_DIVIDER::DIVIDE_16:
//        Binary::BS<volatile uint8_t>(
//            *(reg->clock_rate_divider[0].address), reg->clock_rate_divider[0].offset, true);
//        Binary::BS<volatile uint8_t>(
//            *(reg->clock_rate_divider[1].address), reg->clock_rate_divider[1].offset, false);
//        break;
//    case CLOCK_RATE_DIVIDER::DIVIDE_64:
//        Binary::BS<volatile uint8_t>(
//            *(reg->clock_rate_divider[0].address), reg->clock_rate_divider[0].offset, false);
//        Binary::BS<volatile uint8_t>(
//            *(reg->clock_rate_divider[1].address), reg->clock_rate_divider[1].offset, true);
//        break;
//    case CLOCK_RATE_DIVIDER::DIVIDE_128:
//        Binary::BS<volatile uint8_t>(
//            *(reg->clock_rate_divider[0].address), reg->clock_rate_divider[0].offset, true);
//        Binary::BS<volatile uint8_t>(
//            *(reg->clock_rate_divider[1].address), reg->clock_rate_divider[1].offset, true);
//        break;
//    default:
//        // TODO ERROR_HANDLING
//        break;
//    }
//
//    // set clock rate multiplier
//    switch (m_bus_config.clock_rate_multiplier) {
//    case CLOCK_RATE_MULTIPLIER::NONE:
//        Binary::BS<volatile uint8_t>(
//            *(reg->clock_rate_multiplier.address), reg->clock_rate_multiplier.offset, false);
//        break;
//    case CLOCK_RATE_MULTIPLIER::MULTIPLY_2:
//        Binary::BS<volatile uint8_t>(
//            *(reg->clock_rate_multiplier.address), reg->clock_rate_multiplier.offset, true);
//        break;
//    default:
//        // TODO ERROR_HANDLING
//        break;
//    }
//
//    // read SPSR and SPDR to clear the SPI Interrupt flag
//    uint8_t clear;
//    clear = *reg->status_register;
//    clear = *reg->data_register;
//
//    // initialize the data register to a newline character
//    *reg->data_register = '\n';
//}
//
//void CSPIBusAtmelAVR8::SetConfig(const CSPIBus::CONFIG_DESC& busConfig) {
//    // TODO ERROR_HANDLING: Dangerous cast.  Can use GUID for struct type for safety?
//    m_bus_config  = static_cast<const CONFIG_DESC&>(busConfig);
//    m_duplex_mode = m_bus_config.duplex_mode_intial;
//}
//
//void CSPIBusAtmelAVR8::SetDuplexMode(DUPLEX_MODE duplexMode) {
//    // modify pin config based on duplex mode
//    // NOTE: base this switch off of the duplex mode member not the bus config
//    // the bus config is from init, duplex mode may have been changed after the fact
//
//    CPins<uint8_t>::CONFIG_DESC configMosi = {};
//    m_duplex_mode                          = duplexMode;
//    switch (duplexMode) {
//    case DUPLEX_MODE::FULL:
//    case DUPLEX_MODE::HALF_WRITE:
//        configMosi.direction = CPins<uint8_t>::DIRECTION::OUT;
//        break;
//    case DUPLEX_MODE::HALF_READ:
//        configMosi.direction = CPins<uint8_t>::DIRECTION::IN;
//        configMosi.pull      = CPins<uint8_t>::PULL::UP;
//        break;
//    default:
//        // TODO ERROR_HANDLING
//        break;
//    }
//    m_service_pins.SetPinConfig(m_pin_config.mosi, configMosi);
//}
//
//uint8_t CSPIBusAtmelAVR8::PollMiso() {
//    return m_service_pins.PinReadDigital(m_pin_config.miso);
//}
//
//uint8_t CSPIBusAtmelAVR8::Read() {
//    return *m_register_config.data_register;
//}
//
//void CSPIBusAtmelAVR8::Write(uint8_t data) {
//    *m_register_config.data_register = data;
//}
//
//void CSPIBusAtmelAVR8::WaitForTransfer() {
//    while (!Binary::BC<uint8_t>(
//        *m_register_config.interrupt_flag.address, m_register_config.interrupt_flag.offset)) {
//    }
//}
