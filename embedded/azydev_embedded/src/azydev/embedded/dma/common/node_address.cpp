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

#include <azydev/embedded/dma/common/node_address.h>

/* PUBLIC */

// constructor

template<typename BEAT_PRIMITIVE>
CDMANodeAddress<BEAT_PRIMITIVE>::CDMANodeAddress(const DESC& desc)
    : IDMANode<BEAT_PRIMITIVE>(desc)
    , m_address(desc.address) {
}

// destructor

template<typename BEAT_PRIMITIVE>
CDMANodeAddress<BEAT_PRIMITIVE>::~CDMANodeAddress() {
}

/* PRIVATE */

// IDMANode
template<typename BEAT_PRIMITIVE>
uintptr_t CDMANodeAddress<BEAT_PRIMITIVE>::GetBaseAddress_impl() const {
    return reinterpret_cast<uintptr_t>(m_address);
}

template<typename BEAT_PRIMITIVE>
uint32_t CDMANodeAddress<BEAT_PRIMITIVE>::GetNumBeats_impl() const {
    return 1;
}

template<typename BEAT_PRIMITIVE>
bool CDMANodeAddress<BEAT_PRIMITIVE>::IsIncrementing_impl() const {
    return false;
}

template<typename BEAT_PRIMITIVE>
IDMAEntity::RESULT CDMANodeAddress<BEAT_PRIMITIVE>::Reset_impl() {
    return IDMAEntity::RESULT::SUCCESS;
}
