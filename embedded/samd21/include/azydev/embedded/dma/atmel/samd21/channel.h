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

#include <azydev/embedded/dma/atmel/samd21/transfer.h>
#include <azydev/embedded/dma/common/channel.h>

#include <stdint.h>

#include <asf.h>

template<typename BEAT_PRIMITIVE>
class CDMAChannelAtmelSAMD21 final : public CDMAChannel<BEAT_PRIMITIVE>, public CDMATransfer<BEAT_PRIMITIVE>::ITransferControl
{
public:
    enum class COMMAND : uint8_t
    {
        NOACT,
        SUSPEND,
        RESUME,
        UNDEFINED = 255
    };

    struct CONFIG_DESC : CDMAChannel<BEAT_PRIMITIVE>::CONFIG_DESC
    {
    };

    struct DESC : CDMAChannel<BEAT_PRIMITIVE>::DESC
    {
        typename CDMATransferAtmelSAMD21<BEAT_PRIMITIVE>::DESCRIPTOR* descriptor;
    };

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
    typename CDMATransferAtmelSAMD21<BEAT_PRIMITIVE>::DESCRIPTOR* const m_descriptor;
    CONFIG_DESC m_config;
    typename CDMATransferAtmelSAMD21<BEAT_PRIMITIVE>::TRIGGER_ACTION m_trigger_action;
    uint16_t m_num_beats_remaining;

    // member functions
    uint16_t GetNumBeatsRemaining() const;
    void SetEnableInterrupt(const INTERRUPT, const bool enabled);

    // CDMAChannel
    virtual void SetConfig_impl(const typename CDMAChannel<BEAT_PRIMITIVE>::CONFIG_DESC&) override final;
    virtual void StartTransfer_impl(
        CDMATransfer<BEAT_PRIMITIVE>&,
        const typename CDMATransfer<BEAT_PRIMITIVE>::CONFIG_DESC&,
        typename CDMATransfer<BEAT_PRIMITIVE>::ITransferControl**) override final;
    virtual void MarkTransferEnded_impl(const IDMAEntity::RESULT) override final;
};

/* FORWARD DECLARED TEMPLATES */
template class CDMAChannelAtmelSAMD21<uint8_t>;
template class CDMAChannelAtmelSAMD21<uint16_t>;