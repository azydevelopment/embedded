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

#include <azydev/embedded/util/binary.h>

#include <azydev/embedded/bus/spi/common/bus.h>
#include <azydev/embedded/dma/common/node_address.h>

#include <asf/sam0/drivers/sercom/sercom.h>

class CPinsAtmelSAMD21;

template<typename BEAT_PRIMITIVE>
class CDMAEngine;

template<typename BEAT_PRIMITIVE>
class CDMATransfer;

template<typename BEAT_PRIMITIVE>
class IDMANode;

class CSPIBusAtmelSAMD21 final : public CSPIBus<uint8_t>
{
public:
    enum class FRAME_FORMAT : uint8_t
    {
        UNDEFINED = 255,
        SPI       = 0,
        SPI_ADDR  = 2
    };

    enum class DATA_IN_PINOUT : uint8_t
    {
        UNDEFINED = 255,
        PAD_0     = 0,
        PAD_1     = 1,
        PAD_2     = 2,
        PAD_3     = 3
    };

    enum class DATA_OUT_PINOUT : uint8_t
    {
        UNDEFINED                = 255,
        DO_PAD0_SCK_PAD1_SS_PAD2 = 0,
        DO_PAD2_SCK_PAD3_SS_PAD1 = 1,
        DO_PAD3_SCK_PAD1_SS_PAD2 = 2,
        DO_PAD0_SCK_PAD3_SS_PAD1 = 3
    };

    enum class OPERATING_MODE : uint8_t
    {
        UNDEFINED = 255,
        WORKER    = 2,
        MANAGER   = 3
    };

    enum class ADDRESS_MODE : uint8_t
    {
        UNDEFINED = 255,
        MASK      = 0,
        TWO_ADDRS = 1,
        RANGE     = 2
    };

    enum class CHARACTER_SIZE : uint8_t
    {
        UNDEFINED = 255,
        BITS_8    = 0,
        BITS_9    = 1
    };

    struct CONFIG_DESC : CSPIBus<uint8_t>::CONFIG_DESC
    {
        FRAME_FORMAT frame_format                   = FRAME_FORMAT::UNDEFINED;
        bool immediate_buffer_overflow_notification = false;
        bool run_in_standby                         = false;
        ADDRESS_MODE address_mode                   = ADDRESS_MODE::UNDEFINED;
        bool enable_manager_worker_select           = false;
        bool enable_worker_select_low_detect        = false;
        bool enable_worker_data_preload             = false;

        // TODO IMPLEMENT: 9-bit mode
        // CHARACTER_SIZE character_size               = CHARACTER_SIZE::UNDEFINED;

        uint8_t baud_rate                         = false;
        bool enable_interrupt_error               = false;
        bool enable_interrupt_worker_select_low   = false;
        bool enable_interrupt_receive_complete    = false;
        bool enable_interrupt_transmit_complete   = false;
        bool enable_interrupt_data_register_empty = false;

        // DMA
        uint8_t dma_transfer_id             = 0;
        CDMAEngine<uint8_t>* dma_engine     = nullptr;
        CDMATransfer<uint8_t>* dma_transfer = nullptr;
    };

    struct PIN_CONFIG_DESC
    {
        uint32_t pad0                   = 0;
        uint32_t pad1                   = 0;
        uint32_t pad2                   = 0;
        uint32_t pad3                   = 0;
        DATA_IN_PINOUT data_in_pinout   = DATA_IN_PINOUT::UNDEFINED;
        DATA_OUT_PINOUT data_out_pinout = DATA_OUT_PINOUT::UNDEFINED;
    };

    struct DESC : CSPIBus<uint8_t>::DESC
    {
        SercomSpi* sercomSpi       = nullptr;
        PIN_CONFIG_DESC pin_config = {};
    };

    // constructor
    CSPIBusAtmelSAMD21(const DESC&, CPinsAtmelSAMD21&);

    // destructor
    virtual ~CSPIBusAtmelSAMD21() override;

    // NVI
    virtual STATUS Write(const IDMANode<uint8_t>&, const uint32_t numBeats) final;
    virtual STATUS Read(const IDMANode<uint8_t>&, const uint32_t numBeats) final;

private:
    enum class REG_INTFLAG : uint8_t
    {
        DRE,
        TXC,
        RXC,
        SSL,
        ERROR = 7
    };

    enum class DMA_TRANSFER_TYPE : uint8_t
    {
        READ,
        WRITE
    };

    // member variables
    SercomSpi* m_sercom_spi;
    CPinsAtmelSAMD21& m_service_pins;
    PIN_CONFIG_DESC m_pin_config;
    CONFIG_DESC m_bus_config;
    DUPLEX_MODE m_duplex_mode;
    CDMANodeAddress<uint8_t>* m_dma_node;

    // member functions
    CONFIG_DESC GetConfig() const;
    STATUS WaitForTransmit() const;
    STATUS
    ExecuteDMATransfer(const DMA_TRANSFER_TYPE, const IDMANode<uint8_t>&, const uint32_t numBeats);

    // CSPIEntity
    virtual STATUS SetRole_impl(const ROLE) override final;

    // CSPIBus
    virtual bool IsImmediate_impl() const override final;
    virtual void SetConfig_impl(const CSPIBus::CONFIG_DESC&) override final;
    virtual STATUS SetEnabled_impl(const bool) override final;
    virtual STATUS SetDuplexMode_impl(const DUPLEX_MODE) override final;
    virtual STATUS Start_impl(const uint8_t deviceId) override final;
    virtual STATUS Write_impl(const uint8_t) override final;
    virtual STATUS Read_impl(uint8_t&) override final;
    virtual STATUS Stop_impl() override final;
};
