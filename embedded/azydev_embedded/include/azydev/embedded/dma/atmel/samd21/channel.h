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

#include <azydev/embedded/dma/common/channel.h>

#include <stdint.h>

#include <asf.h>

class CDMAChannelAtmelSAMD21 final
	: public CDMAChannel
	, public IDMAEntity::ITransferControl
{
public:
    enum class TRIGGER : uint8_t
    {
        SOFTWARE_OR_EVENT,
        SERCOM0_RX,
        SERCOM0_TX,
        SERCOM1_RX,
        SERCOM1_TX,
        SERCOM2_RX,
        SERCOM2_TX,
        SERCOM3_RX,
        SERCOM3_TX,
        SERCOM4_RX,
        SERCOM4_TX,
        SERCOM5_RX,
        SERCOM5_TX,
        TCC0_OVF,
        TCC0_MC0,
        TCC0_MC1,
        TCC0_MC2,
        TCC0_MC3,
        TCC1_OVF,
        TCC1_MC0,
        TCC1_MC1,
        TCC1_MC2,
        TCC1_MC3,
        TC0_OVF,
        TC0_MC0,
        TC0_MC1,
        TC1_OVF,
        TC1_MC0,
        TC1_MC1,
        TC2_OVF,
        TC2_MC0,
        TC2_MC1,
        TC3_OVF,
        TC3_MC0,
        TC3_MC1,
        TC4_OVF,
        TC4_MC0,
        TC4_MC1,
        ADC_RESRDY,
        DAC_EMPTY,
        I2S_RX_0,
        I2S_RX_1,
        I2S_TX_0,
        I2S_TX_1,
        UNDEFINED = 255
    };

    enum class COMMAND : uint8_t
    {
        NOACT,
        SUSPEND,
        RESUME,
        UNDEFINED = 255
    };

    enum class TRIGGER_ACTION : uint8_t
    {
        START_BLOCK       = DMAC_CHCTRLB_TRIGACT_BLOCK_Val,
        START_BEAT        = DMAC_CHCTRLB_TRIGACT_BEAT_Val,
        START_TRANSACTION = DMAC_CHCTRLB_TRIGACT_TRANSACTION_Val,
        UNDEFINED         = 255
    };

    enum class PRIORITY : uint8_t
    {
        LVL_0,
        LVL_1,
        LVL_2,
        LVL_3
    };

    enum class EVENT_INPUT_ACTION : uint8_t
    {
        NOACT,
        TRIG,
        CTRIG,
        CBLOCK,
        SUSPEND,
        RESUME,
        SSKIP
    };

    struct alignas(16) DESCRIPTOR
    {
        enum class EVENT_OUTPUT_SELECTION : uint8_t
        {
            DISABLED        = 0,
            BLOCK_COMPLETED = 1,
            BEAT_COMPLETED  = 3
        };

        enum class BLOCK_COMPLETED_ACTION : uint8_t
        {
            DISABLE_IF_LAST,
            DISABLE_IF_LAST_AND_INTERRUPT,
            SUSPEND,
            SUSPEND_AND_INTERRUPT
        };

        enum class BEAT_SIZE : uint8_t
        {
            BITS_8,
            BITS_16,
            BITS_32
        };

        enum class STEP_SIZE : uint8_t
        {
            X1,
            X2,
            X4,
            X8,
            X16,
            X32,
            X64,
            X128
        };

        enum class STEP_SIZE_SELECT : uint8_t
        {
            DESTINATION,
            SOURCE
        };

        union BLOCK_TRANSFER_CONTROL_CONFIG
        {
            struct
            {
                uint16_t valid : 1;
                uint16_t evosel : 2;
                uint16_t blockact : 2;
                uint16_t : 3;
                uint16_t beatsize : 2;
                uint16_t srcinc : 1;
                uint16_t dstinc : 1;
                uint16_t stepsel : 1;
                uint16_t stepsize : 3;
            } bits;
            uint16_t value;
        };

        BLOCK_TRANSFER_CONTROL_CONFIG btctrl = {};
        uint32_t num_beats         = 0;
        uint32_t source_address              = 0;
        uint32_t destination_address         = 0;
        uint32_t next_descriptor_address     = 0;
    };

    struct TRANSFER_DESC : CDMAChannel::TRANSFER_DESC
    {
        // channel config
        PRIORITY priority                     = PRIORITY::LVL_3;
        TRIGGER trigger                       = TRIGGER::UNDEFINED;
        TRIGGER_ACTION trigger_action         = TRIGGER_ACTION::UNDEFINED;
        bool enable_event_output              = false;
        bool enable_event_input               = false;
        EVENT_INPUT_ACTION event_input_action = EVENT_INPUT_ACTION::NOACT;

        // descriptor
        DESCRIPTOR::EVENT_OUTPUT_SELECTION event_output_selection =
            DESCRIPTOR::EVENT_OUTPUT_SELECTION::DISABLED;
        DESCRIPTOR::BLOCK_COMPLETED_ACTION block_completed_action =
            DESCRIPTOR::BLOCK_COMPLETED_ACTION::DISABLE_IF_LAST;
        DESCRIPTOR::BEAT_SIZE beat_size               = DESCRIPTOR::BEAT_SIZE::BITS_8;
        DESCRIPTOR::STEP_SIZE_SELECT step_size_select = DESCRIPTOR::STEP_SIZE_SELECT::DESTINATION;
        DESCRIPTOR::STEP_SIZE step_size               = DESCRIPTOR::STEP_SIZE::X1;

		// interrupts
        bool enable_interrupt_transfer_error    = false;
        bool enable_interrupt_channel_suspend   = false;
    };

    struct CONFIG_DESC : CDMAChannel::CONFIG_DESC
    {};

    struct DESC : CDMAChannel::DESC
    { DESCRIPTOR* descriptor; };

    // constructor
    CDMAChannelAtmelSAMD21(const DESC&);

    // destructor
    virtual ~CDMAChannelAtmelSAMD21() override final;

	// ISR
	void _ISR();

	// IDMAEntity::ITransferControl
	virtual bool IsTransferInProgress() const override final;
	virtual bool IsPendingTrigger() const override final;
	virtual void TriggerTransferStep() override final;

private:
    enum class REG_CHCTRLA : uint8_t
    {
        OFFSET_SWRST,
        OFFSET_ENABLE
    };

    enum class REG_CHCTRLB : uint8_t
    {
        EVACT,
        EVIE = 3,
        EVOE,
        LVL,
        TRIGSRC = 8,
        TRIGACT = 22,
        CMD     = 24
    };

    enum class INTERRUPT : uint8_t
    {
        ON_TRANSFER_ERROR,
        ON_TRANSFER_COMPLETE,
        ON_SUSPEND
    };

    // rule of three
    CDMAChannelAtmelSAMD21(const CDMAChannelAtmelSAMD21&);
    CDMAChannelAtmelSAMD21 operator=(const CDMAChannelAtmelSAMD21&);

    // member variables
    DESCRIPTOR* const m_descriptor;
    CONFIG_DESC m_config;
	TRIGGER_ACTION m_trigger_action;
	uint32_t m_num_beats_remaining;

    // member functions
	uint32_t GetNumBeatsRemaining() const;
    void SetEnableInterrupt(const INTERRUPT, const bool enabled);

    // CDMAChannel
    virtual void SetConfig_impl(const CDMAChannel::CONFIG_DESC&) override final;
    virtual void StartTransfer_impl(const IDMAEntity::TRANSFER_DESC&, IDMAEntity::ITransferControl**) override final;
	virtual void MarkTransferComplete_impl() override final;
};