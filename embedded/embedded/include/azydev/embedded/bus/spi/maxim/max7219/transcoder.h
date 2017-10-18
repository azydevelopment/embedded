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

#include <azydev/embedded/bus/common/transcoder.h>

template<typename TRANSFER_TYPE>
class CSPIBus;

template<typename TRANSFER_TYPE>
class CTranscoderMAX7219 final : public CBusTranscoder<CSPIBus<TRANSFER_TYPE>, uint8_t>
{
public:
    // constructor
    CTranscoderMAX7219();

    // destructor
    virtual ~CTranscoderMAX7219() override final;

    // copy constructor
    CTranscoderMAX7219(const CTranscoderMAX7219&);

    // NVI
    // TODO IMPLEMENT: Replace all these parameters with proper enums or normalized scales
    void SetEnabled(bool enabled);
    void SetDecodeMode(uint8_t decodeMode);
    void SetIntensity(uint8_t intensity);
    void SetScanLimit(uint8_t scanLimit);
    void SetEnabledDisplayTest(bool displayTestEnabled);
    void SetRow(uint8_t row, uint8_t data);

private:
    // rule of three
    CTranscoderMAX7219& operator=(const CTranscoderMAX7219&);
};
