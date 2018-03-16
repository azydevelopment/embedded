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

#include <stdint.h>

class CDMANode
{
public:
    enum class BEAT_PRIMITIVE
    {
        UINT8_T = 1,
        UINT16_T,
        UINT32_T
    };

    struct DESC
    {
        BEAT_PRIMITIVE data_type = BEAT_PRIMITIVE::UINT8_T;
        bool is_incrementing     = false;
    };

    // constructor
    CDMANode(const DESC&);

    // destructor
    virtual ~CDMANode();

    // NVI
    virtual BEAT_PRIMITIVE GetPrimitiveType() const final;
    virtual uint32_t GetAddress() const final;
    virtual bool IsIncrementing() const final;

private:
    // rule of three
    CDMANode(const CDMANode&);
    CDMANode& operator=(const CDMANode&);

    // member variables
    const BEAT_PRIMITIVE m_primitive_type;
    const bool m_is_incrementing;

    // abstract
    virtual uint32_t GetAddress_impl() const = 0;
};
