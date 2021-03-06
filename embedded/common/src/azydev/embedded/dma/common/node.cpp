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

#include <azydev/embedded/dma/common/node.h>

/* PUBLIC */

// constructor

template<typename BEAT_PRIMITIVE>
IDMANode<BEAT_PRIMITIVE>::IDMANode(const DESC& desc)
    : IDMAEntity() {
}

// destructor

template<typename BEAT_PRIMITIVE>
IDMANode<BEAT_PRIMITIVE>::~IDMANode() {
}

// NVI

template<typename BEAT_PRIMITIVE>
uint8_t IDMANode<BEAT_PRIMITIVE>::GetSizeOfBeatPrimitive() const {
    return sizeof(BEAT_PRIMITIVE);
}

template<typename BEAT_PRIMITIVE>
uintptr_t IDMANode<BEAT_PRIMITIVE>::GetBaseAddress() const {
    return GetBaseAddress_impl();
}

template<typename BEAT_PRIMITIVE>
uint32_t IDMANode<BEAT_PRIMITIVE>::GetNumBeats() const {
    return GetNumBeats_impl();
}

template<typename BEAT_PRIMITIVE>
bool IDMANode<BEAT_PRIMITIVE>::IsIncrementing() const {
    return IsIncrementing_impl();
}

template<typename BEAT_PRIMITIVE>
IDMAEntity::RESULT IDMANode<BEAT_PRIMITIVE>::Reset() {
    return Reset_impl();
}
