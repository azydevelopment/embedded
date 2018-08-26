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

#include <azydev/embedded/dma/common/transfer.h>

#include <stdint.h>

template<typename BEAT_PRIMITIVE>
class CDMATransferAtmelSAMD21 final : public CDMATransfer<BEAT_PRIMITIVE>
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

    enum class TRIGGER_ACTION : uint8_t
    {
        START_BLOCK,
        START_BEAT = 2,
        START_TRANSACTION,
        UNDEFINED = 255
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
        uint16_t num_beats                   = 0;
        uint32_t source_address              = 0;
        uint32_t destination_address         = 0;
        uint32_t next_descriptor_address     = 0;
    };

    struct STEP_DESC : CDMATransfer<BEAT_PRIMITIVE>::STEP_DESC
    {
        typename DESCRIPTOR::EVENT_OUTPUT_SELECTION event_output_selection =
            DESCRIPTOR::EVENT_OUTPUT_SELECTION::DISABLED;
        typename DESCRIPTOR::BLOCK_COMPLETED_ACTION block_completed_action =
            DESCRIPTOR::BLOCK_COMPLETED_ACTION::DISABLE_IF_LAST;
        typename DESCRIPTOR::BEAT_SIZE beat_size = DESCRIPTOR::BEAT_SIZE::BITS_8;
        typename DESCRIPTOR::STEP_SIZE_SELECT step_size_select =
            DESCRIPTOR::STEP_SIZE_SELECT::DESTINATION;
        typename DESCRIPTOR::STEP_SIZE step_size = DESCRIPTOR::STEP_SIZE::X1;
    };

    struct CONFIG_DESC : CDMATransfer<BEAT_PRIMITIVE>::CONFIG_DESC
    {
        PRIORITY priority                     = PRIORITY::LVL_3;
        TRIGGER trigger                       = TRIGGER::UNDEFINED;
        TRIGGER_ACTION trigger_action         = TRIGGER_ACTION::UNDEFINED;
        bool enable_event_output              = false;
        bool enable_event_input               = false;
        EVENT_INPUT_ACTION event_input_action = EVENT_INPUT_ACTION::NOACT;

        // interrupts
        // bool enable_interrupt_transfer_error = false;
        // bool enable_interrupt_channel_suspend   = false;
    };

    struct DESC : CDMATransfer<BEAT_PRIMITIVE>::DESC
    {
        uint8_t num_steps_max = 0;
    };

    // constructor
    CDMATransferAtmelSAMD21(const DESC&);

    // destructor
    virtual ~CDMATransferAtmelSAMD21();

    // NVI
    virtual const DESCRIPTOR GetBaseDescriptor() const final;

private:
    // rule of three
    CDMATransferAtmelSAMD21(const CDMATransferAtmelSAMD21&);
    CDMATransferAtmelSAMD21& operator=(const CDMATransferAtmelSAMD21&);

    // member variables
    uint8_t m_num_steps_max;
    uint8_t m_current_step;
    DESCRIPTOR* m_descriptor_chain;

    // member functions
    uint8_t GetNumStepsMax() const;
    uint8_t GetCurrentStep() const;

    // CDMATransfer
    virtual void Reset_impl() override final;
    virtual bool IsStepAvailable_impl(uint8_t const numSteps) const override final;
    virtual IDMAEntity::RESULT
    AddStep_impl(const typename CDMATransfer<BEAT_PRIMITIVE>::STEP_DESC&) override final;
};

/* FORWARD DECLARED TEMPLATES */
template class CDMATransferAtmelSAMD21<uint8_t>;
template class CDMATransferAtmelSAMD21<uint16_t>;