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

#include <azydev/embedded/dma/atmel/samd21/channel.h>

/* PUBLIC */

// constructor

CDMAChannelAtmelSAMD21::CDMAChannelAtmelSAMD21(const DESC& desc)
    : CDMAChannel(desc)
    , m_descriptor(desc.descriptor)
    , m_config({}) {
}

// destructor

CDMAChannelAtmelSAMD21::~CDMAChannelAtmelSAMD21() {
}

/* PRIVATE */

// member variables

void CDMAChannelAtmelSAMD21::SetEnableInterrupt(const INTERRUPT interrupt, const bool enabled) {
    volatile uint8_t& reg = enabled ? DMAC->CHINTENSET.reg : DMAC->CHINTENCLR.reg;
    uint8_t regOffset     = static_cast<uint8_t>(interrupt);
    reg                   = 1 << regOffset;
}

// CDMAChannel

void CDMAChannelAtmelSAMD21::SetConfig_impl(const CDMAChannel::CONFIG_DESC& config) {
    m_config = static_cast<const CDMAChannelAtmelSAMD21::CONFIG_DESC&>(config);
}

void CDMAChannelAtmelSAMD21::StartTransfer_impl(const CDMAChannel::TRANSFER_DESC& transfer) {
    const CDMAChannelAtmelSAMD21::TRANSFER_DESC& transferSAMD21 =
        static_cast<const CDMAChannelAtmelSAMD21::TRANSFER_DESC&>(transfer);

    // TODO IMPLEMENT: transferSAMD21.callback_on_complete

    // enter critsec
    system_interrupt_enter_critical_section();
    {
        // select the channel we'll be using
        uint8_t channelId = GetId();
        DMAC->CHID.reg    = channelId;

        // enable and reset this channel
        DMAC->CHCTRLA.reg &= ~(1 << static_cast<uint8_t>(REG_CHCTRLA::OFFSET_ENABLE));
        DMAC->CHCTRLA.reg = 1 << static_cast<uint8_t>(REG_CHCTRLA::OFFSET_SWRST);

        // clear the software trigger
        DMAC->SWTRIGCTRL.reg &= ~(1 << channelId);

        // write the channel config for this transfer
        {
            uint32_t chctrlb = static_cast<uint8_t>(transferSAMD21.event_input_action)
                                   << static_cast<uint8_t>(REG_CHCTRLB::EVACT)
                               | static_cast<uint8_t>(transferSAMD21.enable_event_input)
                                     << static_cast<uint8_t>(REG_CHCTRLB::EVIE)
                               | static_cast<uint8_t>(transferSAMD21.enable_event_output)
                                     << static_cast<uint8_t>(REG_CHCTRLB::EVOE)
                               | static_cast<uint8_t>(transferSAMD21.priority)
                                     << static_cast<uint8_t>(REG_CHCTRLB::LVL)
                               | static_cast<uint8_t>(transferSAMD21.trigger)
                                     << static_cast<uint8_t>(REG_CHCTRLB::TRIGSRC)
                               | static_cast<uint8_t>(transferSAMD21.trigger_action)
                                     << static_cast<uint8_t>(REG_CHCTRLB::TRIGACT);

            // write the config to the control register
            DMAC->CHCTRLB.reg = chctrlb;
        }

        // write the descriptor for this transfer
        {
            m_descriptor->btctrl.bits.valid = true;
            m_descriptor->btctrl.bits.evosel =
                static_cast<uint8_t>(transferSAMD21.event_output_selection);
            m_descriptor->btctrl.bits.blockact =
                static_cast<uint8_t>(transferSAMD21.block_completed_action);
            m_descriptor->btctrl.bits.beatsize = static_cast<uint8_t>(transferSAMD21.beat_size);
            m_descriptor->btctrl.bits.srcinc =
                static_cast<uint8_t>(transferSAMD21.enable_source_address_increment);
            m_descriptor->btctrl.bits.dstinc =
                static_cast<uint8_t>(transferSAMD21.enable_destination_address_increment);
            m_descriptor->btctrl.bits.stepsel =
                static_cast<uint8_t>(transferSAMD21.step_size_select);
            m_descriptor->btctrl.bits.stepsize    = static_cast<uint8_t>(transferSAMD21.step_size);
            m_descriptor->num_beats_per_block     = transferSAMD21.num_beats_per_block;
            m_descriptor->source_address          = transferSAMD21.source_address;
            m_descriptor->destination_address     = transferSAMD21.destination_address;
            m_descriptor->next_descriptor_address = 0;
        }

        // enable/disable interrupts
        {
            SetEnableInterrupt(
                INTERRUPT::ON_TRANSFER_ERROR, transferSAMD21.enable_interrupt_transfer_error);
            SetEnableInterrupt(
                INTERRUPT::ON_TRANSFER_COMPLETE, transferSAMD21.enable_interrupt_transfer_complete);
            SetEnableInterrupt(
                INTERRUPT::ON_SUSPEND, transferSAMD21.enable_interrupt_channel_suspend);
        }

        // enable the DMA interrupt
        // NVIC->ISER[0] = 1 << 6;

        // finally, enable the transfer channel
        DMAC->CHCTRLA.reg |= 1 << static_cast<uint8_t>(REG_CHCTRLA::OFFSET_ENABLE);
    }
    // enter critsec
    system_interrupt_leave_critical_section();
}
