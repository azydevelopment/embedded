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

#include <azydev/embedded/data/coding/manchester/differential/encoder.h>

#include <azydev/embedded/util/binary.h>

/* PUBLIC */

template<typename ARRAY_INDEX_TYPE, typename DATA_PRIMITIVE_TYPE>
CManchesterDifferentialEncoder<ARRAY_INDEX_TYPE, DATA_PRIMITIVE_TYPE>::
    CManchesterDifferentialEncoder()
    : IDataCoder<ARRAY_INDEX_TYPE, DATA_PRIMITIVE_TYPE, DATA_PRIMITIVE_TYPE>() {
}

template<typename ARRAY_INDEX_TYPE, typename DATA_PRIMITIVE_TYPE>
CManchesterDifferentialEncoder<ARRAY_INDEX_TYPE, DATA_PRIMITIVE_TYPE>::
    ~CManchesterDifferentialEncoder() {
}

// IDataDecoder

template<typename ARRAY_INDEX_TYPE, typename DATA_PRIMITIVE_TYPE>
void CManchesterDifferentialEncoder<ARRAY_INDEX_TYPE, DATA_PRIMITIVE_TYPE>::Code(
    const DATA_PRIMITIVE_TYPE input[],
    ARRAY_INDEX_TYPE inLength,
    DATA_PRIMITIVE_TYPE output[]) const {
    // TODO ERROR_HANDLING: Make sure output is big enough somehow

    // TODO HACK: Assuming no datatype larger than 256 bits
    uint8_t numDataPrimitiveBits = sizeof(DATA_PRIMITIVE_TYPE) * 8;

    bool lastBit = false;
    // encode the data
    for (ARRAY_INDEX_TYPE i = 0; i < inLength; i++) {
        // TODO HACK: Assuming no datatype larger than 256 bits
        for (uint8_t j = numDataPrimitiveBits; j-- > 0;) {
            ARRAY_INDEX_TYPE outputPrimitiveIndex =
                i * 2 + (numDataPrimitiveBits - 1 - j) / (numDataPrimitiveBits / 2);
            uint8_t outputBit = (j * 2) % numDataPrimitiveBits;

            // TODO IMPLEMENT: Ability to change polarity of the encoder
            bool secondBit = !lastBit ^ (Binary::BC<DATA_PRIMITIVE_TYPE>(input[i], j) != 0);
            Binary::BS<DATA_PRIMITIVE_TYPE>(output[outputPrimitiveIndex], outputBit, secondBit);

            Binary::BS<DATA_PRIMITIVE_TYPE>(output[outputPrimitiveIndex], outputBit + 1, !lastBit);

            lastBit = secondBit;
        }
    }
}

/* PRIVATE */

/* FORWARD DECLARED TEMPLATES */
template class CManchesterDifferentialEncoder<uint8_t, uint8_t>;