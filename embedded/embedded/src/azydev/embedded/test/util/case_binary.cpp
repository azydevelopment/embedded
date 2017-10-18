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

#include <azydev/embedded/test/util/case_binary.h>

#include <abdeveng/core/logging/common/logger.h>
#include <abdeveng/core/test/common/evaluator.h>

#include <azydev/embedded/util/binary.h>

/* PUBLIC */

CTestCaseUtilBinary::~CTestCaseUtilBinary() {
}

// ITestCase

void CTestCaseUtilBinary::Run(
    volatile void* dut,
    const CTestHarness::ITestCase::TEST_CASE_CONFIG_DESC& config,
    const CEvaluator& evaluator,
    CLogger& logger) {
    logger.Log("Check binary to decimal conversion...");
    {
        const uint8_t numVariants = 4;
        uint8_t data8[numVariants];
        data8[0] = _B8(0100);
        data8[1] = _B8(1010111100000100);
        data8[2] = _B8(1111010100000100);
        data8[3] = _B8(0000111100000100);

        uint16_t data16[numVariants];
        data16[0] = _B16(11111111, 11110100);
        data16[1] = _B16(111111111111, 111111110100);
        data16[2] = _B16(1010111111111111, 1101111111110100);
        data16[3] = _B16(1110001011111111, 0101101011110100);

        uint32_t data32[numVariants];
        data32[0] = _B32(11111111, 00000000, 10101010, 10000001);
        data32[1] = _B32(111111111111, 111100000000, 111110101010, 111110000001);
        data32[2] = _B32(100111111111, 001000000000, 011010101010, 110110000001);
        data32[3] = _B32(1010101011111111, 110000000000, 0101101010101010, 1111111110000001);

        uint8_t data8_expected   = 0x04;
        uint16_t data16_expected = 0xfff4;
        uint32_t data32_expected = 0xff00aa81;

        // check 8 bit data
        for (uint8_t i = 0; i < numVariants - 1; i++) {
            evaluator.Validate(data8[i] == data8[i + 1]);
        }
        for (uint8_t i = 0; i < numVariants - 1; i++) {
            evaluator.Validate(data8[i] == data8_expected);
        }

        // check 16 bit data
        for (uint8_t i = 0; i < numVariants - 1; i++) {
            evaluator.Validate(data16[i] == data16[i + 1]);
        }
        for (uint8_t i = 0; i < numVariants - 1; i++) {
            evaluator.Validate(data16[i] == data16_expected);
        }

        // check 32 bit data
        for (uint8_t i = 0; i < numVariants - 1; i++) {
            evaluator.Validate(data32[i] == data32[i + 1]);
        }
        for (uint8_t i = 0; i < numVariants - 1; i++) {
            evaluator.Validate(data32[i] == data32_expected);
        }
    }

    logger.Log("Check one hot encoding...");
    {
        for (uint8_t i = 0; i < 16; i++) {
            evaluator.Validate(Binary::OH<uint16_t>(i) == 1 << i);
        }
    }

    logger.Log("Check bit masking...");
    {
        uint8_t mask8            = Binary::BM<uint8_t>(3, 15);
        uint16_t mask16          = Binary::BM<uint16_t>(3, 15);
        uint32_t mask32          = Binary::BM<uint32_t>(3, 15);
        uint8_t mask8_expected   = _B8(11111000);
        uint16_t mask16_expected = _B16(11111111, 11111000);
        uint32_t mask32_expected = _B32(00000000, 00000011, 11111111, 11111000);

        evaluator.Validate(mask8 == mask8_expected);
        evaluator.Validate(mask16 == mask16_expected);
        evaluator.Validate(mask32 == mask32_expected);
    }

    logger.Log("Check bit ranging...");
    {
        uint8_t range8            = Binary::BR<uint8_t>(15, 4, _B8(11001));
        uint16_t range16          = Binary::BR<uint16_t>(15, 4, _B8(11001));
        uint32_t range32          = Binary::BR<uint32_t>(15, 4, _B8(11001));
        uint8_t range8_expected   = _B8(00000000);
        uint16_t range16_expected = _B16(10000000, 00000000);
        uint32_t range32_expected = _B32(00000000, 00000100, 10000000, 00000000);

        evaluator.Validate(range8 == range8_expected);
        evaluator.Validate(range16 == range16_expected);
        evaluator.Validate(range32 == range32_expected);
    }

    logger.Log("Check bit checking...");
    {
        evaluator.Validate(Binary::BC<uint8_t>(_B8(01000000), 6) == 1);
        evaluator.Validate(Binary::BC<uint8_t>(_B8(01000000), 7) == 0);
        evaluator.Validate(Binary::BC<uint8_t>(_B8(01000000), 8) == 0);
        evaluator.Validate(Binary::BC<uint16_t>(_B16(01000000, 00000000), 14) == 1);
        evaluator.Validate(Binary::BC<uint16_t>(_B16(01000000, 00000000), 15) == 0);
        evaluator.Validate(Binary::BC<uint16_t>(_B16(01000000, 00000000), 16) == 0);
        evaluator.Validate(
            Binary::BC<uint32_t>(_B32(01000000, 00000000, 00000000, 00000000), 30) == 1);
        evaluator.Validate(
            Binary::BC<uint32_t>(_B32(01000000, 00000000, 00000000, 00000000), 31) == 0);
        evaluator.Validate(
            Binary::BC<uint32_t>(_B32(01000000, 00000000, 00000000, 00000000), 32) == 0);
    }

    logger.Log("Check bit range checking...");
    {
        evaluator.Validate(Binary::BCR<uint8_t>(_B8(11000000), 5, 2) == _B8(10));
        evaluator.Validate(Binary::BCR<uint8_t>(_B8(11000000), 6, 2) == _B8(11));
        evaluator.Validate(Binary::BCR<uint8_t>(_B8(11000000), 7, 2) == _B8(01));
        evaluator.Validate(Binary::BCR<uint16_t>(_B16(11000000, 00000000), 13, 2) == _B16(0, 10));
        evaluator.Validate(Binary::BCR<uint16_t>(_B16(11000000, 00000000), 14, 2) == _B16(0, 11));
        evaluator.Validate(Binary::BCR<uint16_t>(_B16(11000000, 00000000), 15, 2) == _B16(0, 01));
        evaluator.Validate(
            Binary::BCR<uint32_t>(_B32(11000000, 00000000, 00000000, 00000000), 29, 2)
            == _B32(0, 0, 0, 10));
        evaluator.Validate(
            Binary::BCR<uint32_t>(_B32(11000000, 00000000, 00000000, 00000000), 30, 2)
            == _B32(0, 0, 0, 11));
        evaluator.Validate(
            Binary::BCR<uint32_t>(_B32(11000000, 00000000, 00000000, 00000000), 31, 2)
            == _B32(0, 0, 0, 01));
    }

    logger.Log("Check bit setting...");
    {
        uint8_t data8   = 0;
        uint16_t data16 = 0;
        uint32_t data32 = 0;
        Binary::BS<uint8_t>(data8, 3, true);
        evaluator.Validate(data8 == _B8(00001000));
        Binary::BS<uint8_t>(data8, 5, true);

        // Overflow us undefined behaviour
        // evaluator.Validate(data8 == _B8(00101000));
        // Binary::BS<uint8_t>(data8, 10, true);

        evaluator.Validate(data8 == _B8(00101000));
        Binary::BS<uint16_t>(data16, 11, true);
        evaluator.Validate(data16 == _B16(00001000, 00000000));
        Binary::BS<uint16_t>(data16, 13, true);
        evaluator.Validate(data16 == _B16(00101000, 00000000));

        // Overflow us undefined behaviour
        // Binary::BS<uint16_t>(data16, 18, true);
        // evaluator.Validate(data16 == _B16(00101000, 00000000));

        Binary::BS<uint32_t>(data32, 27, true);
        evaluator.Validate(data32 == _B32(00001000, 00000000, 00000000, 00000000));
        Binary::BS<uint32_t>(data32, 29, true);
        evaluator.Validate(data32 == _B32(00101000, 00000000, 00000000, 00000000));

        // Overflow us undefined behaviour
        // Binary::BS<uint32_t>(data32, 34, true);
        // evaluator.Validate(data32 == _B32(00101000, 00000000, 00000000, 00000000));
    }

    logger.Log("Check bit range setting...");
    {
        uint8_t data8   = 0;
        uint16_t data16 = 0;
        uint32_t data32 = 0;

        Binary::BSR<uint8_t>(data8, 3, 2, _B8(110100110));
        evaluator.Validate(data8 == _B8(00010000));
        Binary::BSR<uint8_t>(data8, 5, 2, _B8(110100110));
        evaluator.Validate(data8 == _B8(01010000));

        // Overflow us undefined behaviour
        // Binary::BS<uint8_t>R(data8, 10, 2, _B8(110100110));
        // evaluator.Validate(data8 == _B8(01010000));

        Binary::BSR<uint16_t>(data16, 11, 2, _B16(00000000, 110100110));
        evaluator.Validate(data16 == _B16(00010000, 00000000));
        Binary::BSR<uint16_t>(data16, 13, 2, _B16(00000000, 110100110));
        evaluator.Validate(data16 == _B16(01010000, 00000000));

        // Overflow us undefined behaviour
        // Binary::BS<uint16_t>R(data16, 18, 2, _B16(00000000, 110100110));
        // evaluator.Validate(data16 == _B16(01010000, 00000000));

        Binary::BSR<uint32_t>(data32, 27, 2, _B32(00000000, 00000000, 00000000, 110100110));
        evaluator.Validate(data32 == _B32(00010000, 00000000, 00000000, 00000000));
        Binary::BSR<uint32_t>(data32, 29, 2, _B32(00000000, 00000000, 00000000, 110100110));
        evaluator.Validate(data32 == _B32(01010000, 00000000, 00000000, 00000000));

        // Overflow us undefined behaviour
        // Binary::BS<uint32_t>R(data32, 34, 2, _B32(00000000, 00000000, 00000000, 110100110));
        // evaluator.Validate(data32 == _B32(01010000, 00000000, 00000000, 00000000));
    }

    logger.Log("Check some mixing of types...");
    {
        // TODO ERROR_HANDLING: How do you prevent _B16 being truncated to an 8bit type
        evaluator.Validate(Binary::BCR<uint16_t>(_B16(11000000, 00000000), 13, 2) == _B8(10));
        evaluator.Validate(Binary::BCR<uint16_t>(_B8(11000000), 0, 10) == _B16(00000000, 11000000));
        evaluator.Validate(
            !(Binary::BCR<uint16_t>(_B8(11000000), 0, 10) == _B16(10000000, 11000000)));

        uint16_t data16 = 0;
        Binary::BSR<uint16_t>(data16, 2, 9, _B8(110110011));
        evaluator.Validate(data16 == _B16(00000010, 11001100));
    }

    logger.Log("Check parity checking...");
    {
        evaluator.Validate(Binary::IsParityEven<uint8_t>(_B8(01111001)));
        evaluator.Validate(Binary::IsParityEven<uint8_t>(_B8(101111001)));
        evaluator.Validate(Binary::IsParityOdd<uint8_t>(_B8(01111000)));
        evaluator.Validate(Binary::IsParityOdd<uint8_t>(_B8(101111000)));
    }
}