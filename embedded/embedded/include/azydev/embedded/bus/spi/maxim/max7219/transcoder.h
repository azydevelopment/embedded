/*
* Copyright (C) Andrew Yeung - All Rights Reserved
* Unauthorized copying of this file, via any medium is strictly prohibited
* Proprietary and confidential
* Written by Andrew Yeung <azy.development@gmail.com>, May 2017
*/

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
