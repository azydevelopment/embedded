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

#include <azydev/embedded/bus/spi/maxim/max7219/transcoder.h>

#include <azydev/embedded/bus/spi/common/bus.h>

/* PUBLIC */

// constructor

template<typename TRANSFER_TYPE>
CTranscoderMAX7219<TRANSFER_TYPE>::CTranscoderMAX7219()
    : CBusTranscoder<CSPIBus<TRANSFER_TYPE>, uint8_t>() {
}

// destructor

template<typename TRANSFER_TYPE>
CTranscoderMAX7219<TRANSFER_TYPE>::~CTranscoderMAX7219() {
}

// copy constructor
template<typename TRANSFER_TYPE>
CTranscoderMAX7219<TRANSFER_TYPE>::CTranscoderMAX7219(const CTranscoderMAX7219&) {
}

// NVI

template<typename TRANSFER_TYPE>
void CTranscoderMAX7219<TRANSFER_TYPE>::SetEnabled(bool enabled) {
    this->GetBus().Start(this->GetDeviceId());
    this->GetBus().Write(0x0C);
    this->GetBus().Write(enabled);
    this->GetBus().Stop();
}

template<typename TRANSFER_TYPE>
void CTranscoderMAX7219<TRANSFER_TYPE>::SetDecodeMode(uint8_t decodeMode) {
    this->GetBus().Start(this->GetDeviceId());
    this->GetBus().Write(0x09);
    this->GetBus().Write(0);
    this->GetBus().Stop();
}

template<typename TRANSFER_TYPE>
void CTranscoderMAX7219<TRANSFER_TYPE>::SetIntensity(uint8_t intensity) {
    this->GetBus().Start(this->GetDeviceId());
    this->GetBus().Write(0x0A);
    this->GetBus().Write(intensity);
    this->GetBus().Stop();
}

template<typename TRANSFER_TYPE>
void CTranscoderMAX7219<TRANSFER_TYPE>::SetScanLimit(uint8_t scanLimit) {
    this->GetBus().Start(this->GetDeviceId());
    this->GetBus().Write(0x0B);
    this->GetBus().Write(scanLimit);
    this->GetBus().Stop();
}

template<typename TRANSFER_TYPE>
void CTranscoderMAX7219<TRANSFER_TYPE>::SetEnabledDisplayTest(bool displayTestEnabled) {
    this->GetBus().Start(this->GetDeviceId());
    this->GetBus().Write(0x0F);
    this->GetBus().Write(displayTestEnabled);
    this->GetBus().Stop();
}

template<typename TRANSFER_TYPE>
void CTranscoderMAX7219<TRANSFER_TYPE>::SetRow(uint8_t row, uint8_t data) {
    this->GetBus().Start(this->GetDeviceId());
    this->GetBus().Write(row + 1);
    this->GetBus().Write(data);
    this->GetBus().Stop();
}

/* FORWARD DECLARED TEMPLATES */
template class CTranscoderMAX7219<uint8_t>;
template class CTranscoderMAX7219<uint16_t>;
