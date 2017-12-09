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

#include <azydev/embedded/bus/i2c/atmel/samd21/bus.h>

#include <azydev/embedded/pins/atmel/samd21/pins.h>

/* PUBLIC */

CI2CBusAtmelSAMD21::CI2CBusAtmelSAMD21(const DESC& desc, CPinsAtmelSAMD21& servicePins)
    : CI2CBus(desc)
    , m_service_pins(servicePins)
    , m_config({})
    , m_pin_config(desc.pin_config)
    , m_sercom_i2c(nullptr) {
    // TODO HACK: Hardcoded to SERCOM0
    m_sercom_i2c = &(SERCOM0->I2CM);
}

CI2CBusAtmelSAMD21::~CI2CBusAtmelSAMD21() {
}

/* PRIVATE */

CI2CBusAtmelSAMD21::STATUS CI2CBusAtmelSAMD21::WaitForBusSync() {
    // wait for bus to finish sync
    while (m_sercom_i2c->SYNCBUSY.reg != 0) {
        // TODO ERROR_HANDLING: Timeout
    }
}

CI2CBusAtmelSAMD21::STATUS CI2CBusAtmelSAMD21::WaitForTransfer() {
    // TODO IMPLEMENT: Worker mode
    while (!m_sercom_i2c->INTFLAG.bit.MB && !m_sercom_i2c->INTFLAG.bit.SB) {
        // TODO ERROR_HANDLING: Timeout
    }
    return STATUS::OK;
}

// CI2CEntity

CI2CEntity::STATUS CI2CBusAtmelSAMD21::SetRole_impl(const ROLE role) {
    // TODO IMPLEMENT
    return STATUS::ERROR_UNKNOWN;
}

// CI2CBus

void CI2CBusAtmelSAMD21::SetConfig_impl(const CI2CBus::CONFIG_DESC& busConfig) {
    // TODO ERROR_HANDLING: Dangerous cast.  Can use GUID for struct type for safety?
    m_config = static_cast<const CONFIG_DESC&>(busConfig);
}

CI2CEntity::STATUS CI2CBusAtmelSAMD21::SetEnabled_impl(const bool enabled) {
    // TODO ERROR_HANDLING: Handle if already enabled
    if (enabled) {
        // TODO ERROR_HANDLING: Check that the bus is configured to have a role other than UNDEFINED

        // setup clocks
        // TODO HACK: Better abstract clock setup
        {
            uint32_t sercom_index = _sercom_get_sercom_inst_index(SERCOM0);
            uint32_t pm_index, gclk_index;
            pm_index   = sercom_index + PM_APBCMASK_SERCOM0_Pos;
            gclk_index = sercom_index + SERCOM0_GCLK_ID_CORE;

            system_apb_clock_set_mask(SYSTEM_CLOCK_APB_APBC, 1 << pm_index);

            // TODO HACK: Hardcoded clock generator source
            struct system_gclk_chan_config gclk_chan_conf;
            gclk_chan_conf.source_generator = GCLK_GENERATOR_0;

            system_gclk_chan_set_config(gclk_index, &gclk_chan_conf);
            system_gclk_chan_enable(gclk_index);
            sercom_set_gclk_generator(gclk_chan_conf.source_generator, false);
        }

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
            // select operating mode
            {
                // TODO IMPLEMENT: Worker mode
                // TODO HACK: Remove need for sercom.h
                // TODO HACK: Hardcoded value
                ctrla |= SERCOM_I2CM_CTRLA_MODE(0x05);
            }

            // select SCL low timeout
            ctrla |= m_config.enable_scl_low_timeout ? SERCOM_I2CM_CTRLA_LOWTOUTEN : 0;

            // select inactive bus timeout
            ctrla |= SERCOM_I2CM_CTRLA_INACTOUT(static_cast<uint8_t>(m_config.inactive_timeout));

            // select SCL clock stretch mode
            ctrla |= (m_config.scl_clk_stretch_mode == SCL_CLK_STRETCH_MODE::AFTER_ACK_BIT)
                         ? SERCOM_I2CM_CTRLA_SCLSM
                         : 0;

            // select transfer speed
            ctrla |= SERCOM_I2CM_CTRLA_SPEED(static_cast<uint8_t>(m_config.transfer_speed));

            // select worker SCL low extend timeout
            ctrla |= m_config.enable_worker_scl_low_extend_timeout ? SERCOM_I2CM_CTRLA_SEXTTOEN : 0;

            // select manager SCL low extend timeout
            ctrla |= m_config.enable_manager_scl_low_extend_timeout ? SERCOM_I2CM_CTRLA_MEXTTOEN : 0;

            // select SDA hold time
            ctrla |= SERCOM_I2CM_CTRLA_SDAHOLD(static_cast<uint8_t>(m_config.sda_hold_time));

            // select pin usage
            ctrla |= m_config.enable_four_wire ? SERCOM_I2CM_CTRLA_PINOUT : 0;

            // select run in standby
            ctrla |= m_config.enable_run_in_standby ? SERCOM_I2CM_CTRLA_RUNSTDBY : 0;
        }

        // setup CTRLB register
        // TODO IMPLEMENT: Remaining CTRLB register options
        uint32_t ctrlb = 0;
        {
            // select Smart Mode
            ctrlb |= m_config.enable_smart_mode ? SERCOM_I2CM_CTRLB_SMEN : 0;
        }

        // setup BAUD register
        uint32_t baud = 0;
        {
            baud |= SERCOM_I2CM_BAUD_HSBAUDLOW(m_config.high_speed_manager_baud_low);
            baud |= SERCOM_I2CM_BAUD_HSBAUD(m_config.high_speed_manager_baud);
            baud |= SERCOM_I2CM_BAUD_BAUDLOW(m_config.manager_baud_low);
            baud |= SERCOM_I2CM_BAUD_BAUD(m_config.manager_baud);
        }

        // write config registers
        m_sercom_i2c->CTRLA.reg = ctrla;
        m_sercom_i2c->CTRLB.reg = ctrlb;
        m_sercom_i2c->BAUD.reg  = baud;

        // enable SPI
        m_sercom_i2c->CTRLA.reg |= SERCOM_I2CM_CTRLA_ENABLE;

        // TODO ERROR_HANDLING: Check this for success
        STATUS status = WaitForBusSync();

        m_sercom_i2c->STATUS.reg = SERCOM_I2CM_STATUS_BUSSTATE(1);

    } else {
        // TODO IMPLEMENT
    }

    return STATUS::OK;
}

CI2CEntity::STATUS
CI2CBusAtmelSAMD21::Start_impl(uint8_t address, const TRANSFER_DIRECTION direction) {
    // TODO IMPLEMENT: 10bit mode
    // TODO IMPLEMENT: High speed mode
    m_sercom_i2c->ADDR.reg = static_cast<uint32_t>(direction) | (address << 1)
                             | static_cast<uint32_t>(m_config.transfer_speed)
                                   << SERCOM_I2CM_ADDR_HS_Pos;

    STATUS status = WaitForTransfer();
    if (status != STATUS::OK) {
        return status;
    }

    if (m_sercom_i2c->INTFLAG.bit.ERROR) {
        return STATUS::ERROR_UNKNOWN;
    }

    if (m_sercom_i2c->INTFLAG.bit.SB) {
        m_sercom_i2c->INTFLAG.bit.SB = 1;
    }

    if (m_sercom_i2c->INTFLAG.bit.MB) {
        // TODO ERROR_HANDLING
    }

    // TODO IMPLEMENT: Read as well as Write pre-transmission work

    return STATUS::OK;
}

CI2CEntity::STATUS CI2CBusAtmelSAMD21::Write_impl(uint8_t data) {
    m_sercom_i2c->DATA.reg = data;
    return WaitForTransfer();
}

CI2CEntity::STATUS CI2CBusAtmelSAMD21::Read_impl(uint8_t& outData) {
    outData = m_sercom_i2c->DATA.reg;
    return WaitForTransfer();
}

CI2CEntity::STATUS CI2CBusAtmelSAMD21::Stop_impl() {
    m_sercom_i2c->CTRLB.reg |= SERCOM_I2CM_CTRLB_CMD(3);
    return STATUS::OK;
}
