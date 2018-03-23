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

#include <azydev/embedded/dma/atmel/samd21/engine.h>

#include <asf.h>

/* FILE SCOPED STATICS */

// TODO HACK: Hardcoded number of channels
const uint8_t NUM_CHANNELS = 1;

template<typename BEAT_PRIMITIVE>
static typename CDMATransferAtmelSAMD21<BEAT_PRIMITIVE>::DESCRIPTOR fs_descriptors[NUM_CHANNELS];

template<typename BEAT_PRIMITIVE>
static typename CDMATransferAtmelSAMD21<BEAT_PRIMITIVE>::DESCRIPTOR
    fs_descriptors_writeback[NUM_CHANNELS];

// TODO HACK: Abstract all these ISR things away
static void (*ISR_DMAC)(void);

/* ISR */

// TODO HACK: Remove ASF ISR usage
void DMAC_Handler() {
    ISR_DMAC();
}

/* STATIC */

template<typename BEAT_PRIMITIVE>
CDMAEngineAtmelSAMD21<BEAT_PRIMITIVE>* CDMAEngineAtmelSAMD21<BEAT_PRIMITIVE>::s_dma_engine;

template<typename BEAT_PRIMITIVE>
void CDMAEngineAtmelSAMD21<BEAT_PRIMITIVE>::_ISR() {
    system_interrupt_enter_critical_section();
    {
        // TODO HACK: Hardcoded mask
        uint8_t pendingChannelId = DMAC->INTPEND.reg & 0xF;

        CDMAChannelAtmelSAMD21<BEAT_PRIMITIVE>& channel =
            s_dma_engine->GetChannel(pendingChannelId);

        // execute ISR for this particular channel
        channel._ISR();
    }
    system_interrupt_leave_critical_section();
}

/* PUBLIC */

// constructor

template<typename BEAT_PRIMITIVE>
CDMAEngineAtmelSAMD21<BEAT_PRIMITIVE>::CDMAEngineAtmelSAMD21(const DESC& desc)
    : CDMAEngine<BEAT_PRIMITIVE>(desc)
    , m_num_channels(NUM_CHANNELS)
    , m_channels(nullptr) {
    // create space for the channel object pointers
    m_channels = new CDMAChannel<BEAT_PRIMITIVE>*[GetNumChannels()];

    // create the actual channels
    for (uint8_t i = 0; i < GetNumChannels(); i++) {
        typename CDMAChannelAtmelSAMD21<BEAT_PRIMITIVE>::DESC channelDesc = {};
        channelDesc.id                                                    = i;
        channelDesc.descriptor = &fs_descriptors<BEAT_PRIMITIVE>[i];
        m_channels[i]          = new CDMAChannelAtmelSAMD21<BEAT_PRIMITIVE>(channelDesc);
    }

    // TODO HACK: Only one engine can ever exist
    s_dma_engine = this;
    ISR_DMAC     = &(s_dma_engine->_ISR);
}

// destructor

template<typename BEAT_PRIMITIVE>
CDMAEngineAtmelSAMD21<BEAT_PRIMITIVE>::~CDMAEngineAtmelSAMD21() {
    // TODO IMPLEMENT: Check if all DMA transfers are done

    // delete all the channels
    for (uint8_t i = 0; i < GetNumChannels(); i++) {
        delete m_channels[i];
    }

    delete[] m_channels;
}

/* PRIVATE */

// member functions

template<typename BEAT_PRIMITIVE>
uint8_t CDMAEngineAtmelSAMD21<BEAT_PRIMITIVE>::GetNumChannels() const {
    return m_num_channels;
}

template<typename BEAT_PRIMITIVE>
CDMAChannelAtmelSAMD21<BEAT_PRIMITIVE>&
CDMAEngineAtmelSAMD21<BEAT_PRIMITIVE>::GetChannel(const uint8_t channelId) const {
    return *static_cast<CDMAChannelAtmelSAMD21<BEAT_PRIMITIVE>*>(m_channels[channelId]);
}

template<typename BEAT_PRIMITIVE>
void CDMAEngineAtmelSAMD21<BEAT_PRIMITIVE>::SetEnablePriority(
    const typename CDMATransferAtmelSAMD21<BEAT_PRIMITIVE>::PRIORITY priority,
    const bool enabled) {
    uint8_t regOffset = static_cast<uint8_t>(REG_CTRL::LVLENX);
    if (enabled) {
        DMAC->CTRL.reg |= 1 << (regOffset + static_cast<uint8_t>(priority));
    } else {
        DMAC->CTRL.reg &= ~(1 << (regOffset + static_cast<uint8_t>(priority)));
    }
}

// CDMAEngine

template<typename BEAT_PRIMITIVE>
void CDMAEngineAtmelSAMD21<BEAT_PRIMITIVE>::SetEnabled_impl(const bool enabled) {
    // enter critsec
    system_interrupt_enter_critical_section();

    // reset DMAC
    DMAC->CTRL.reg &= ~(1 << static_cast<uint8_t>(REG_CTRL::DMAENABLE));
    DMAC->CTRL.reg = 1 << static_cast<uint8_t>(REG_CTRL::SWRST);

    // setup descriptor sections
    // TODO HACK: Should this be cast to a uintptr_t?

    DMAC->BASEADDR.reg = reinterpret_cast<uint32_t>(fs_descriptors<BEAT_PRIMITIVE>);
    DMAC->WRBADDR.reg  = reinterpret_cast<uint32_t>(fs_descriptors_writeback<BEAT_PRIMITIVE>);

    // enable priority level 0
    // TODO IMPLEMENT: Usage of other priorities
    SetEnablePriority(CDMATransferAtmelSAMD21<BEAT_PRIMITIVE>::PRIORITY::LVL_0, true);

    // enable the DMA module
    DMAC->CTRL.reg |= 1 << static_cast<uint8_t>(REG_CTRL::DMAENABLE);

    // TODO IMPLEMENT: CRC

    // enable the DMA interrupt
    NVIC->ISER[0] = 1 << 6;

    // exit critsec
    system_interrupt_leave_critical_section();
}

template<typename BEAT_PRIMITIVE>
CDMAChannel<BEAT_PRIMITIVE>* CDMAEngineAtmelSAMD21<BEAT_PRIMITIVE>::AcquireFreeChannel_impl() {
    CDMAChannel<BEAT_PRIMITIVE>* channel = nullptr;

    // find a free channel if there is one
    for (uint8_t i = 0; i < GetNumChannels(); i++) {
        CDMAChannel<BEAT_PRIMITIVE>& channelTemp = GetChannel(i);
        if (!channelTemp.IsTransferInProgress()) {
            channel = &channelTemp;
            break;
        }
    }

    return channel;
}