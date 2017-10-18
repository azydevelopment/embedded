/*
* Copyright (C) Andrew Yeung - All Rights Reserved
* Unauthorized copying of this file, via any medium is strictly prohibited
* Proprietary and confidential
* Written by Andrew Yeung <azy.development@gmail.com>, May 2017
*/

#include <azydev/embedded/data/coding/manchester/standard/decoder.h>

#include <azydev/embedded/util/binary.h>

/* PUBLIC */

template<typename ARRAY_INDEX_TYPE, typename DATA_PRIMITIVE_TYPE>
CManchesterDecoder<ARRAY_INDEX_TYPE, DATA_PRIMITIVE_TYPE>::CManchesterDecoder()
    : IDataCoder<ARRAY_INDEX_TYPE, DATA_PRIMITIVE_TYPE, DATA_PRIMITIVE_TYPE>() {
}

template<typename ARRAY_INDEX_TYPE, typename DATA_PRIMITIVE_TYPE>
CManchesterDecoder<ARRAY_INDEX_TYPE, DATA_PRIMITIVE_TYPE>::~CManchesterDecoder() {
}

// IDataDecoder

template<typename ARRAY_INDEX_TYPE, typename DATA_PRIMITIVE_TYPE>
void CManchesterDecoder<ARRAY_INDEX_TYPE, DATA_PRIMITIVE_TYPE>::Code(
    const DATA_PRIMITIVE_TYPE input[],
    ARRAY_INDEX_TYPE inLength,
    DATA_PRIMITIVE_TYPE output[]) const {
    // TODO ERROR_HANDLING: Make sure output is big enough somehow

    // TODO HACK: Assuming no datatype larger than 256 bits
    uint8_t numDataPrimitiveBits = sizeof(DATA_PRIMITIVE_TYPE) * 8;
    uint8_t maxBitIndex          = numDataPrimitiveBits - 1;

    // encode the data
    for (ARRAY_INDEX_TYPE i = 0; i < inLength; i++) {
        // iterate over every two bits since this is decoded using edges (requires before and after
        // information)
        // TODO HACK: Assuming no datatype larger than 256 bits
        for (uint8_t j = 0; j < numDataPrimitiveBits; j += 2) {
            uint8_t inputBit = numDataPrimitiveBits - 2 - j;

            ARRAY_INDEX_TYPE outputPrimitiveIndex = i / 2;
            uint8_t outputBit = maxBitIndex - ((i % 2) * numDataPrimitiveBits + j) / 2;

            // TODO IMPLEMENT: Ability to change polarity of the decoder
            Binary::BS<DATA_PRIMITIVE_TYPE>(
                output[outputPrimitiveIndex],
                outputBit,
                Binary::BC<DATA_PRIMITIVE_TYPE>(input[i], inputBit + 1)
                    < Binary::BC<DATA_PRIMITIVE_TYPE>(input[i], inputBit));
        }
    }
}

/* FORWARD DECLARED TEMPLATES */
template class CManchesterDecoder<uint8_t, uint8_t>;