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

#include <azydev/embedded/dma/common/transfer.h>

#include <stdint.h>

template<typename BEAT_PRIMITIVE>
class CDMAChannel;

template<typename BEAT_PRIMITIVE>
class CDMAEngine : public IDMAEntity
{
public:
    struct DESC
    {
    };

    // destructor
    virtual ~CDMAEngine() override;

    // NVI
    virtual void SetEnabled(const bool) final;
    virtual RESULT StartTransfer(
        CDMATransfer<BEAT_PRIMITIVE>&,
        const typename CDMATransfer<BEAT_PRIMITIVE>::CONFIG_DESC&,
        typename CDMATransfer<BEAT_PRIMITIVE>::ITransferControl** = nullptr) final;

protected:
    // constructor
    CDMAEngine(const DESC&);

private:
    // rule of three
    CDMAEngine(const CDMAEngine&);
    CDMAEngine& operator=(const CDMAEngine&);

    // abstract
    virtual void SetEnabled_impl(const bool)       = 0;
    virtual CDMAChannel<BEAT_PRIMITIVE>* AcquireFreeChannel_impl() = 0;
};

/* FORWARD DECLARED TEMPLATES */
template class CDMAEngine<uint8_t>;
template class CDMAEngine<uint16_t>;