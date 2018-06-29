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

#include <azydev/embedded/dma/common/node.h>

#include <stdint.h>

template<typename BEAT_PRIMITIVE>
class CDMANodeAddress : public IDMANode<BEAT_PRIMITIVE>
{
public:
    struct DESC : IDMANode<BEAT_PRIMITIVE>::DESC
    { uintptr_t address = 0; };

    // constructor
    CDMANodeAddress(const DESC&);

    // destructor
    virtual ~CDMANodeAddress();

private:
    // rule of three
    CDMANodeAddress(const CDMANodeAddress&);
    CDMANodeAddress& operator=(const CDMANodeAddress&);

    // member variables
    const uintptr_t m_address;

    // IDMANode
    virtual uintptr_t GetBaseAddress_impl() const override final;
    virtual uint32_t GetNumBeats_impl() const override final;
    virtual bool IsIncrementing_impl() const override final;
    virtual IDMAEntity::RESULT Reset_impl() override final;
};

/* FORWARD DECLARED TEMPLATES */
template class CDMANodeAddress<uint16_t>;