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

#include <azydev/embedded/dma/common/entity.h>

#include <stdint.h>

template<typename BEAT_PRIMITIVE>
class IDMANode;

template<typename BEAT_PRIMITIVE>
class CDMATransfer : public IDMAEntity
{
public:
    class ITransferControl
    {
    public:
        virtual bool IsTransferInProgress() const = 0;
        virtual bool IsPendingTrigger() const     = 0;
        virtual void TriggerTransferStep()        = 0;
    };

    struct STEP_DESC
    {
        uint32_t num_beats                               = 0;
        const IDMANode<BEAT_PRIMITIVE>* node_source      = nullptr;
        const IDMANode<BEAT_PRIMITIVE>* node_destination = nullptr;
    };

    struct CONFIG_DESC
    { OnTransferEnded callback_on_transfer_ended = nullptr; };

    struct DESC
    { uint8_t id_initial = 0; };

    // destructor
    virtual ~CDMATransfer();

    // NVI
    virtual uint8_t GetId() volatile const final;
    virtual void Reset(const uint8_t id) final;
    virtual bool IsStepAvailable(uint8_t const numSteps = 1) const final;
    virtual RESULT AddStep(const STEP_DESC&) final;
    virtual uint32_t GetNumBeats() const final;

protected:
    // constructor
    CDMATransfer(const DESC&);

private:
    // rule of three
    CDMATransfer(const CDMATransfer&);
    CDMATransfer& operator=(const CDMATransfer&);

    // member variables
    uint8_t m_id;
    uint32_t m_num_beats;

    // abstract
    virtual void Reset_impl()                                       = 0;
    virtual bool IsStepAvailable_impl(uint8_t const numSteps) const = 0;
    virtual RESULT AddStep_impl(const STEP_DESC&)                   = 0;
};

/* FORWARD DECLARED TEMPLATES */
template class CDMATransfer<uint8_t>;
template class CDMATransfer<uint16_t>;