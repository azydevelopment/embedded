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

#include <azydev/embedded/dma/common/node.h>

/* PUBLIC */

// constructor

CDMAChannelAtmelSAMD21::CDMAChannelAtmelSAMD21(const DESC& desc)
    : CDMAChannel(desc)
    , m_descriptor(desc.descriptor)
    , m_config({})
    , m_trigger_action(TRIGGER_ACTION::UNDEFINED)
    , m_num_beats_remaining(0) {
}

// destructor

CDMAChannelAtmelSAMD21::~CDMAChannelAtmelSAMD21() {
}

// ISR

void CDMAChannelAtmelSAMD21::_ISR() {
    system_interrupt_enter_critical_section();
    {
        // clean up
        MarkTransferComplete();
    }
    system_interrupt_leave_critical_section();
}

// IDMAEntity::ITransferControl

bool CDMAChannelAtmelSAMD21::IsTransferInProgress() const {
    return CDMAChannel::IsTransferInProgress();
}

bool CDMAChannelAtmelSAMD21::IsPendingTrigger() const {
    bool pendingTrigger = false;

    if (IsTransferInProgress() && m_num_beats_remaining != 0) {
        switch (m_trigger_action) {
        case TRIGGER_ACTION::START_BEAT:
            pendingTrigger = (DMAC->PENDCH.reg & (1 << GetId())) == 0;
            break;
        case TRIGGER_ACTION::START_BLOCK:
        case TRIGGER_ACTION::START_TRANSACTION:
            pendingTrigger = true;
            break;
        default:
            break;
        }
    }

    return pendingTrigger;
}

void CDMAChannelAtmelSAMD21::TriggerTransferStep() {
    switch (m_trigger_action) {
    case TRIGGER_ACTION::START_BEAT:
        DMAC->SWTRIGCTRL.reg |= 1 << GetId();
        m_num_beats_remaining--;
        break;
    case TRIGGER_ACTION::START_BLOCK:
    case TRIGGER_ACTION::START_TRANSACTION:
        DMAC->SWTRIGCTRL.reg |= 1 << GetId();
        m_num_beats_remaining = 0; // all beats will be triggered by this
        break;
    default:
        break;
    }
}

/* PRIVATE */

// member functions

uint32_t CDMAChannelAtmelSAMD21::GetNumBeatsRemaining() const {
	return m_num_beats_remaining;
}

void CDMAChannelAtmelSAMD21::SetEnableInterrupt(const INTERRUPT interrupt, const bool enabled) {
    volatile uint8_t& reg = enabled ? DMAC->CHINTENSET.reg : DMAC->CHINTENCLR.reg;
    uint8_t regOffset     = static_cast<uint8_t>(interrupt);
    reg                   = 1 << regOffset;
}

// CDMAChannel

void CDMAChannelAtmelSAMD21::SetConfig_impl(const CDMAChannel::CONFIG_DESC& config) {
    m_config = static_cast<const CDMAChannelAtmelSAMD21::CONFIG_DESC&>(config);
}

void CDMAChannelAtmelSAMD21::StartTransfer_impl(
    const IDMAEntity::TRANSFER_DESC& transfer,
    IDMAEntity::ITransferControl** transferControl) {
    const CDMAChannelAtmelSAMD21::TRANSFER_DESC& transferSAMD21 =
        static_cast<const CDMAChannelAtmelSAMD21::TRANSFER_DESC&>(transfer);

    IDMANode& nodeSrc  = *(transferSAMD21.node_source);
    IDMANode& nodeDest = *(transferSAMD21.node_destination);

    // TODO IMPLEMENT: transferSAMD21.callback_on_complete

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

        // store the trigger action and num beats to determine when the channel is pending
        m_trigger_action      = transferSAMD21.trigger_action;
        m_num_beats_remaining = transferSAMD21.num_beats;

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
            // calculate source data address
            uint32_t addressSrc = 0;
            {
				uint32_t offset = 0;
				if(nodeSrc.IsIncrementing()) {
					offset = GetNumBeatsRemaining() * static_cast<uint8_t>(nodeSrc.GetPrimitiveType());
				}
				
                addressSrc = nodeSrc.GetAddress() + offset;
            }
			
			// calculate source data destination
			uint32_t addressDest = 0;
			{
				uint32_t offset = 0;
				if(nodeDest.IsIncrementing()) {
					offset = GetNumBeatsRemaining() * static_cast<uint8_t>(nodeDest.GetPrimitiveType());
				}
				
				addressDest = nodeDest.GetAddress() + offset;
			}


            m_descriptor->btctrl.bits.valid = true;
            m_descriptor->btctrl.bits.evosel =
                static_cast<uint8_t>(transferSAMD21.event_output_selection);
            m_descriptor->btctrl.bits.blockact =
                static_cast<uint8_t>(transferSAMD21.block_completed_action);
            m_descriptor->btctrl.bits.beatsize = static_cast<uint8_t>(transferSAMD21.beat_size);
            m_descriptor->btctrl.bits.srcinc =
                static_cast<uint8_t>(nodeSrc.IsIncrementing());
            m_descriptor->btctrl.bits.dstinc =
                static_cast<uint8_t>(nodeDest.IsIncrementing());
            m_descriptor->btctrl.bits.stepsel =
                static_cast<uint8_t>(transferSAMD21.step_size_select);
            m_descriptor->btctrl.bits.stepsize    = static_cast<uint8_t>(transferSAMD21.step_size);
            m_descriptor->num_beats               = GetNumBeatsRemaining();
            m_descriptor->source_address          = addressSrc;
            m_descriptor->destination_address     = addressDest;
            m_descriptor->next_descriptor_address = 0;

            // TODO IMPLEMENT: Linked descriptors
        }

        // enable/disable interrupts
        {
            // always enable the transfer complete interrupt flags
            SetEnableInterrupt(INTERRUPT::ON_TRANSFER_COMPLETE, true);

            // optionally enable the transfer error and suspend interrupt flags
            SetEnableInterrupt(
                INTERRUPT::ON_TRANSFER_ERROR, transferSAMD21.enable_interrupt_transfer_error);
            SetEnableInterrupt(
                INTERRUPT::ON_SUSPEND, transferSAMD21.enable_interrupt_channel_suspend);
        }

        // enable the transfer channel
        DMAC->CHCTRLA.reg |= 1 << static_cast<uint8_t>(REG_CHCTRLA::OFFSET_ENABLE);

        // return a transfer control object if requested
        if (transferControl != nullptr) {
            *transferControl = this;
        }
    }
    system_interrupt_leave_critical_section();
}

void CDMAChannelAtmelSAMD21::MarkTransferComplete_impl() {
    system_interrupt_enter_critical_section();
    {
        // select our channel
        DMAC->CHID.reg = GetId();

        // clear the transfer complete interrupt flag
        DMAC->CHINTFLAG.reg = 1 << static_cast<uint8_t>(INTERRUPT::ON_TRANSFER_COMPLETE);
    }
    system_interrupt_leave_critical_section();
}