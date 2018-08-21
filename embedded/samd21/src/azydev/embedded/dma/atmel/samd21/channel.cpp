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

template<typename BEAT_PRIMITIVE>
CDMAChannelAtmelSAMD21<BEAT_PRIMITIVE>::CDMAChannelAtmelSAMD21(const DESC& desc)
    : CDMAChannel<BEAT_PRIMITIVE>(desc)
    , m_descriptor(desc.descriptor)
    , m_config({})
    , m_trigger_action(CDMATransferAtmelSAMD21<BEAT_PRIMITIVE>::TRIGGER_ACTION::UNDEFINED)
    , m_num_beats_remaining(0) {
}

// destructor

template<typename BEAT_PRIMITIVE>
CDMAChannelAtmelSAMD21<BEAT_PRIMITIVE>::~CDMAChannelAtmelSAMD21() {
}

// ISR

template<typename BEAT_PRIMITIVE>
void CDMAChannelAtmelSAMD21<BEAT_PRIMITIVE>::_ISR() {
    system_interrupt_enter_critical_section();
    {
        // clean up
        {
            if (DMAC->CHINTFLAG.reg
                & (1 << static_cast<uint8_t>(INTERRUPT::ON_TRANSFER_COMPLETE))) {
                this->MarkTransferEnded(IDMAEntity::IDMAEntity::RESULT::SUCCESS);
            } else if (
                DMAC->CHINTFLAG.reg & (1 << static_cast<uint8_t>(INTERRUPT::ON_TRANSFER_ERROR))) {
                this->MarkTransferEnded(IDMAEntity::IDMAEntity::RESULT::FAIL_ERROR);
            }
        }
    }
    system_interrupt_leave_critical_section();
}

// IDMAEntity::ITransferControl

template<typename BEAT_PRIMITIVE>
bool CDMAChannelAtmelSAMD21<BEAT_PRIMITIVE>::IsTransferInProgress() const {
    return CDMAChannel<BEAT_PRIMITIVE>::IsTransferInProgress();
}

template<typename BEAT_PRIMITIVE>
bool CDMAChannelAtmelSAMD21<BEAT_PRIMITIVE>::IsPendingTrigger() const {
    bool pendingTrigger = false;

    if (IsTransferInProgress() && m_num_beats_remaining != 0) {
        switch (m_trigger_action) {
        case CDMATransferAtmelSAMD21<BEAT_PRIMITIVE>::TRIGGER_ACTION::START_BEAT:
            pendingTrigger = (DMAC->PENDCH.reg & (1 << this->GetId())) == 0;
            break;
        case CDMATransferAtmelSAMD21<BEAT_PRIMITIVE>::TRIGGER_ACTION::START_BLOCK:
        case CDMATransferAtmelSAMD21<BEAT_PRIMITIVE>::TRIGGER_ACTION::START_TRANSACTION:
            pendingTrigger = true;
            break;
        default:
            break;
        }
    }

    return pendingTrigger;
}

template<typename BEAT_PRIMITIVE>
void CDMAChannelAtmelSAMD21<BEAT_PRIMITIVE>::TriggerTransferStep() {
    switch (m_trigger_action) {
    case CDMATransferAtmelSAMD21<BEAT_PRIMITIVE>::TRIGGER_ACTION::START_BEAT:
        DMAC->SWTRIGCTRL.reg |= 1 << this->GetId();
        m_num_beats_remaining--;
        break;
    case CDMATransferAtmelSAMD21<BEAT_PRIMITIVE>::TRIGGER_ACTION::START_BLOCK:
    case CDMATransferAtmelSAMD21<BEAT_PRIMITIVE>::TRIGGER_ACTION::START_TRANSACTION:
        DMAC->SWTRIGCTRL.reg |= 1 << this->GetId();
        m_num_beats_remaining = 0; // all beats will be triggered by this
        break;
    default:
        break;
    }
}

/* PRIVATE */

// member functions

template<typename BEAT_PRIMITIVE>
uint16_t CDMAChannelAtmelSAMD21<BEAT_PRIMITIVE>::GetNumBeatsRemaining() const {
    return m_num_beats_remaining;
}

template<typename BEAT_PRIMITIVE>
void CDMAChannelAtmelSAMD21<BEAT_PRIMITIVE>::SetEnableInterrupt(
    const INTERRUPT interrupt,
    const bool enabled) {
    volatile uint8_t& reg = enabled ? DMAC->CHINTENSET.reg : DMAC->CHINTENCLR.reg;
    uint8_t regOffset     = static_cast<uint8_t>(interrupt);
    reg                   = 1 << regOffset;
}

// CDMAChannel

template<typename BEAT_PRIMITIVE>
void CDMAChannelAtmelSAMD21<BEAT_PRIMITIVE>::SetConfig_impl(
    const typename CDMAChannel<BEAT_PRIMITIVE>::CONFIG_DESC& config) {
    m_config =
        static_cast<const typename CDMAChannelAtmelSAMD21<BEAT_PRIMITIVE>::CONFIG_DESC&>(config);
}

template<typename BEAT_PRIMITIVE>
void CDMAChannelAtmelSAMD21<BEAT_PRIMITIVE>::StartTransfer_impl(
    CDMATransfer<BEAT_PRIMITIVE>& transfer,
    const typename CDMATransfer<BEAT_PRIMITIVE>::CONFIG_DESC& transferConfig,
    typename CDMATransfer<BEAT_PRIMITIVE>::ITransferControl** transferControl) {
    const CDMATransferAtmelSAMD21<BEAT_PRIMITIVE>& transferSAMD21 =
        static_cast<const CDMATransferAtmelSAMD21<BEAT_PRIMITIVE>&>(transfer);

    const typename CDMATransferAtmelSAMD21<BEAT_PRIMITIVE>::CONFIG_DESC& transferConfigSAMD21 =
        static_cast<const typename CDMATransferAtmelSAMD21<BEAT_PRIMITIVE>::CONFIG_DESC&>(
            transferConfig);

    // TODO IMPLEMENT: transferSAMD21.callback_on_complete

    system_interrupt_enter_critical_section();
    {
        // select the channel we'll be using
        uint8_t channelId = this->GetId();
        DMAC->CHID.reg    = channelId;

        // enable and reset this channel
        DMAC->CHCTRLA.reg &= ~(1 << static_cast<uint8_t>(REG_CHCTRLA::OFFSET_ENABLE));
        DMAC->CHCTRLA.reg = 1 << static_cast<uint8_t>(REG_CHCTRLA::OFFSET_SWRST);

        // clear the software trigger
        DMAC->SWTRIGCTRL.reg &= ~(1 << channelId);

        // store the trigger action and num beats to determine when the channel is pending
        m_trigger_action      = transferConfigSAMD21.trigger_action;
        m_num_beats_remaining = transferSAMD21.GetNumBeats();

        // write the channel config for this transfer
        {
            uint32_t chctrlb = static_cast<uint8_t>(transferConfigSAMD21.event_input_action)
                                   << static_cast<uint8_t>(REG_CHCTRLB::EVACT)
                               | static_cast<uint8_t>(transferConfigSAMD21.enable_event_input)
                                     << static_cast<uint8_t>(REG_CHCTRLB::EVIE)
                               | static_cast<uint8_t>(transferConfigSAMD21.enable_event_output)
                                     << static_cast<uint8_t>(REG_CHCTRLB::EVOE)
                               | static_cast<uint8_t>(transferConfigSAMD21.priority)
                                     << static_cast<uint8_t>(REG_CHCTRLB::LVL)
                               | static_cast<uint8_t>(transferConfigSAMD21.trigger)
                                     << static_cast<uint8_t>(REG_CHCTRLB::TRIGSRC)
                               | static_cast<uint8_t>(transferConfigSAMD21.trigger_action)
                                     << static_cast<uint8_t>(REG_CHCTRLB::TRIGACT);

            // write the config to the control register
            DMAC->CHCTRLB.reg = chctrlb;
        }

        // populate the base descriptor for this transfer
        *m_descriptor = transferSAMD21.GetBaseDescriptor();

        // enable/disable interrupts
        {
            // always enable the transfer complete and transfer error interrupt flags
            SetEnableInterrupt(INTERRUPT::ON_TRANSFER_COMPLETE, true);
            SetEnableInterrupt(INTERRUPT::ON_TRANSFER_ERROR, true);
            // SetEnableInterrupt(
            // INTERRUPT::ON_SUSPEND, transferSAMD21.enable_interrupt_channel_suspend);
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

template<typename BEAT_PRIMITIVE>
void CDMAChannelAtmelSAMD21<BEAT_PRIMITIVE>::MarkTransferEnded_impl(
    const IDMAEntity::RESULT result) {
    system_interrupt_enter_critical_section();
    {
        // select our channel
        DMAC->CHID.reg = this->GetId();

        switch (result) {
        case IDMAEntity::RESULT::SUCCESS:
            // clear the transfer complete interrupt flag
            DMAC->CHINTFLAG.reg = 1 << static_cast<uint8_t>(INTERRUPT::ON_TRANSFER_COMPLETE);
            break;
        case IDMAEntity::RESULT::FAIL_ERROR:
            // clear the transfer error interrupt flag
            DMAC->CHINTFLAG.reg = 1 << static_cast<uint8_t>(INTERRUPT::ON_TRANSFER_ERROR);
            break;
        default:
            break;
        }
    }
    system_interrupt_leave_critical_section();
}