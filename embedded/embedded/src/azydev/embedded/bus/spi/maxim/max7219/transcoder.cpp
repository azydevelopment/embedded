/*
* Copyright (C) Andrew Yeung - All Rights Reserved
* Unauthorized copying of this file, via any medium is strictly prohibited
* Proprietary and confidential
* Written by Andrew Yeung <azy.development@gmail.com>, May 2017
*/

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
