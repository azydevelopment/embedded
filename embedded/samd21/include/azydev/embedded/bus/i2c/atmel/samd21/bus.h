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

#include <azydev/embedded/bus/i2c/common/bus.h>

#include <sercom.h>

class CPinsAtmelSAMD21;

class CI2CBusAtmelSAMD21 final : public CI2CBus
{
public:
    enum class INACTIVE_TIMEOUT
    {
        DISABLED = 0x0,
        US_55    = 0x1,
        US_105   = 0x2,
        US_205   = 0x3
    };

    enum class SCL_CLK_STRETCH_MODE
    {
        DEFAULT       = 0,
        AFTER_ACK_BIT = 1
    };

    enum class TRANSFER_SPEED
    {
        STANDARD   = 0x0,
        FAST_PLUS  = 0x1,
        HIGH_SPEED = 0x2
    };

    enum class SDA_HOLD_TIME
    {
        DISABLED = 0x0,
        NS_75    = 0x1,
        NS_450   = 0x2,
        NS_600   = 0x3
    };

    // TODO IMPLEMENT: Maybe use PINMUX_UNUSED as defaults instead
    struct PIN_CONFIG_DESC
    {
        uint32_t pad0 = 255;
        uint32_t pad1 = 255;
        uint32_t pad2 = 255;
        uint32_t pad3 = 255;
    };

    struct CONFIG_DESC : CI2CBus::CONFIG_DESC
    {
        // ctrla
        bool enable_scl_low_timeout                = false;
        INACTIVE_TIMEOUT inactive_timeout          = INACTIVE_TIMEOUT::DISABLED;
        SCL_CLK_STRETCH_MODE scl_clk_stretch_mode  = SCL_CLK_STRETCH_MODE::DEFAULT;
        TRANSFER_SPEED transfer_speed              = TRANSFER_SPEED::STANDARD;
        bool enable_worker_scl_low_extend_timeout  = false;
        bool enable_manager_scl_low_extend_timeout = false;
        SDA_HOLD_TIME sda_hold_time                = SDA_HOLD_TIME::DISABLED;
        bool enable_four_wire                      = false;
        bool enable_run_in_standby                 = false;

        // ctrlb
        bool enable_smart_mode = false;

        // baud
        uint8_t high_speed_manager_baud_low = 0;
        uint8_t high_speed_manager_baud     = 0;
        uint8_t manager_baud_low            = 0;
        uint8_t manager_baud                = 0;
    };

    // TODO HACK: Limited to M implementation of SercomI2cm
    struct DESC : CI2CBus::DESC
    {
        SercomI2cm* sercomI2c      = nullptr;
        PIN_CONFIG_DESC pin_config = {};
    };

    // constructor
    CI2CBusAtmelSAMD21(const DESC&, CPinsAtmelSAMD21&);

    // destructor
    virtual ~CI2CBusAtmelSAMD21() override;

private:
    // member variables
    CPinsAtmelSAMD21& m_service_pins;
    CONFIG_DESC m_config;
    PIN_CONFIG_DESC m_pin_config;
    SercomI2cm* m_sercom_i2c;

    // methods
    STATUS WaitForBusSync();
    STATUS WaitForTransfer();

    // CI2CEntity
    virtual STATUS SetRole_impl(const ROLE) override final;

    // CI2CBus
    virtual void SetConfig_impl(const CI2CBus::CONFIG_DESC&) override final;
    virtual STATUS SetEnabled_impl(const bool) override final;
    virtual STATUS Start_impl(const uint8_t address, const TRANSFER_DIRECTION) override final;
    virtual STATUS Write_impl(const uint8_t) override final;
    virtual STATUS Read_impl(uint8_t& outData) override final;
    virtual STATUS Stop_impl() override final;
};