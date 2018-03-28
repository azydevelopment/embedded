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
class CDMANode : public IDMAEntity
{
public:
    struct DESC
    { bool is_incrementing = false; };

    // constructor
    CDMANode(const DESC&);

    // destructor
    virtual ~CDMANode();

    // NVI
    virtual uint8_t GetSizeOfBeatPrimitive() const final;
    virtual uint32_t GetAddress() const final;
    virtual bool IsIncrementing() const final;
    virtual RESULT Reset() final;
    virtual RESULT RecordWrite(const BEAT_PRIMITIVE) final;
    virtual RESULT RecordRead(const uint32_t count = 1) final;

private:
    // rule of three
    CDMANode(const CDMANode&);
    CDMANode& operator=(const CDMANode&);

    // member variables
    const bool m_is_incrementing;

    // abstract
    virtual uint32_t GetAddress_impl() const              = 0;
    virtual RESULT Reset_impl()                           = 0;
    virtual RESULT RecordWrite_impl(const BEAT_PRIMITIVE) = 0;
    virtual RESULT RecordRead_impl(const uint32_t numBeats = 1) = 0;
};

/* FORWARD DECLARED TEMPLATES */
template class CDMANode<uint8_t>;
template class CDMANode<uint16_t>;