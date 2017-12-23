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

#include <azydev/embedded/bus/spi/atmel/samd21/bus.h>

#include <azydev/embedded/pins/atmel/samd21/pins.h>
#include <azydev/embedded/util/binary.h>

#include <asf/sam0/drivers/sercom/sercom.h>

/* PUBLIC */

CSPIBusAtmelSAMD21::CSPIBusAtmelSAMD21(const DESC& desc, CPinsAtmelSAMD21& pins)
    : CSPIBus<uint16_t>(desc)
    , m_sercom_spi(desc.sercomSpi)
    , m_service_pins(pins)
    , m_pin_config(desc.pin_config)
    , m_bus_config({})
    , m_duplex_mode(DUPLEX_MODE::UNDEFINED) {
}

CSPIBusAtmelSAMD21::~CSPIBusAtmelSAMD21() {
}

/* PRIVATE */

// methods

CSPIEntity::STATUS CSPIBusAtmelSAMD21::WaitForBusSync() {
    // wait for bus to finish sync
    while (m_sercom_spi->SYNCBUSY.reg != 0) {
        // TODO ERROR_HANDLING: Timeout
    }
}

CSPIEntity::STATUS CSPIBusAtmelSAMD21::WaitForTransfer() {
    while (!Binary::BC(m_sercom_spi->INTFLAG.reg, SERCOM_SPI_INTFLAG_DRE_Pos)) {
        // TODO ERROR_HANDLING: Timeout
    }

    return STATUS::OK;
}

// CSPIEntity

CSPIEntity::STATUS CSPIBusAtmelSAMD21::SetRole_impl(const ROLE) {
    return STATUS::OK;
}

// CSPIBus

void CSPIBusAtmelSAMD21::SetConfig_impl(const CSPIBus::CONFIG_DESC& busConfig) {
    // TODO ERROR_HANDLING: Dangerous cast.  Can use GUID for struct type for safety?
    m_bus_config  = static_cast<const CONFIG_DESC&>(busConfig);
    m_duplex_mode = m_bus_config.duplex_mode_intial;
}

CSPIEntity::STATUS CSPIBusAtmelSAMD21::SetEnabled_impl(bool enable) {
    // TODO HACK: Only enabling is allowed right now. Need to be able to disable.
    // TODO HACK: Only manager mode is implemented
    if (enable) {
        // TODO ERROR_HANDLING: Check that the bus is configured to have a role other than UNDEFINED

        // setup pins
        {
            CPinsAtmelSAMD21::CONFIG_DESC config = {};
            config.mux_type                      = CPinsAtmelSAMD21::MUX_TYPE::PERIPHERAL;
            config.direction                     = CPinsAtmelSAMD21::DIRECTION::IN;
            config.pull                          = CPinsAtmelSAMD21::PULL::UP;

            // TODO IMPLEMENT: Both manager and worker mode
            // TODO HACK: Apparently all of the pins are in the upper 16 bits and need to be shifted
            // down
            m_service_pins.SetPinConfig(m_pin_config.pad0, config);
            m_service_pins.SetPinConfig(m_pin_config.pad1, config);
            m_service_pins.SetPinConfig(m_pin_config.pad2, config);
            m_service_pins.SetPinConfig(m_pin_config.pad3, config);
        }

        // setup CTRLA register
        uint32_t ctrla = 0;
        {
            // select SPI operating mode
            {
                // TODO HACK: Remove need for sercom.h
                // TODO HACK: Only supports OPERATING_MODE::MANAGER right now
                ctrla |= SERCOM_SPI_CTRLA_MODE(static_cast<uint32_t>(OPERATING_MODE::MANAGER));
            }

            // select clock polarity and phase
            {
                // TODO HACK: Remove need for sercom.h
                Binary::BS(
                    ctrla,
                    SERCOM_SPI_CTRLA_CPOL_Pos,
                    static_cast<uint32_t>(m_bus_config.clock_polarity));

                // TODO HACK: Remove need for sercom.h
                Binary::BS(
                    ctrla,
                    SERCOM_SPI_CTRLA_CPHA_Pos,
                    static_cast<uint32_t>(m_bus_config.clock_phase));
            }

            // select frame format
            {
                // TODO HACK: Remove need for sercom.h
                ctrla |= SERCOM_SPI_CTRLA_FORM(static_cast<uint32_t>(m_bus_config.frame_format));
            }

            // select data in pinout
            {
                // TODO HACK: Remove need for sercom.h
                // TODO HACK: Hardcoded value
                Binary::BSR(
                    ctrla,
                    SERCOM_SPI_CTRLA_DIPO_Pos,
                    2,
                    static_cast<uint32_t>(m_pin_config.data_in_pinout));
            }

            // select data out pinout
            {
                // TODO HACK: Remove need for sercom.h
                // TODO HACK: Hardcoded value
                Binary::BSR(
                    ctrla,
                    SERCOM_SPI_CTRLA_DOPO_Pos,
                    2,
                    static_cast<uint32_t>(m_pin_config.data_out_pinout));
            }

            // select data order
            {
                // TODO HACK: Remove need for sercom.h
                Binary::BS(
                    ctrla,
                    SERCOM_SPI_CTRLA_DORD_Pos,
                    static_cast<uint32_t>(m_bus_config.endianness));
            }
        }

        // setup CTRLB register
        uint32_t ctrlb = 0;
        {
            // select character size
            {
                // TODO HACK: Remove need for sercom.h
                // TODO HACK: Hardcoded value
                Binary::BSR(
                    ctrlb,
                    SERCOM_SPI_CTRLB_CHSIZE_Pos,
                    3,
                    static_cast<uint32_t>(m_bus_config.character_size));
            }

            // select hardware SS control
            {
                // TODO HACK: Remove need for sercom.h
                Binary::BS(
                    ctrlb,
                    SERCOM_SPI_CTRLB_MSSEN_Pos,
                    static_cast<uint32_t>(m_bus_config.enable_manager_worker_select));
            }

            // enable the receiver
            {
                // TODO HACK: Remove need for sercom.h
                Binary::BS(ctrlb, SERCOM_SPI_CTRLB_RXEN_Pos, static_cast<uint32_t>(true));
            }
        }

        // write config registers
        m_sercom_spi->CTRLA.reg = ctrla;
        m_sercom_spi->CTRLB.reg = ctrlb;
        m_sercom_spi->BAUD.reg  = m_bus_config.baud_rate;

        // enable SPI
        m_sercom_spi->CTRLA.reg |= SERCOM_SPI_CTRLA_ENABLE;

        // TODO ERROR_HANDLING: Timeout
        STATUS status = WaitForBusSync();
    }

    return STATUS::OK;
}

CSPIEntity::STATUS CSPIBusAtmelSAMD21::SetDuplexMode_impl(const DUPLEX_MODE duplexMode) {
    // modify pin config based on duplex mode
    // NOTE: base this switch off of the duplex mode member not the bus config the bus config is
    // from init, duplex mode may have been changed after the fact
    m_duplex_mode = duplexMode;

    // TODO IMPLEMENT

    return STATUS::OK;
}

CSPIEntity::STATUS CSPIBusAtmelSAMD21::Start_impl(const uint8_t deviceId) {
    return STATUS::OK;
}

CSPIEntity::STATUS CSPIBusAtmelSAMD21::Read_impl(uint16_t& outData) {
    // TODO ERROR_HANDLING: RX buffer overflow

    switch (m_bus_config.character_size) {
    case CHARACTER_SIZE::BITS_8:
        outData = m_sercom_spi->DATA.reg & Binary::BM<uint16_t>(0, 8);
        break;
    case CHARACTER_SIZE::BITS_9:
        outData = m_sercom_spi->DATA.reg & Binary::BM<uint16_t>(0, 9);
        break;
    default:
        // TODO ERROR_HANDLING
        break;
    }

    return WaitForTransfer();
}

CSPIEntity::STATUS CSPIBusAtmelSAMD21::Write_impl(const uint16_t data) {
    switch (m_bus_config.character_size) {
    case CHARACTER_SIZE::BITS_8:
        m_sercom_spi->DATA.reg = Binary::BM<uint16_t>(data, 0, 8);
        break;
    case CHARACTER_SIZE::BITS_9:
        m_sercom_spi->DATA.reg = Binary::BM<uint16_t>(data, 0, 9);
        break;
    default:
        // TODO ERROR_HANDLING
        break;
    }

    return WaitForTransfer();
}

CSPIEntity::STATUS CSPIBusAtmelSAMD21::Stop_impl() {
    // wait until transmission is complete
    while (!Binary::BC(m_sercom_spi->INTFLAG.reg, SERCOM_SPI_INTFLAG_TXC_Pos)) {
        // TODO ERROR_HANDLING: Timeout
    }

    return STATUS::OK;
}