/*
* Copyright (C) Andrew Yeung - All Rights Reserved
* Unauthorized copying of this file, via any medium is strictly prohibited
* Proprietary and confidential
* Written by Andrew Yeung <azy.development@gmail.com>, May 2017
*/

#pragma once

#include <abdeveng/core/test/common/harness.h>

#include <stdint.h>

class CEvaluator;

class CTestCaseSPIManager final : public CTestHarness::ITestCase
{
public:
    virtual ~CTestCaseSPIManager() override;

    // ITestCase
    virtual void
    Run(volatile void* dut,
        const TEST_CASE_CONFIG_DESC& config,
        const CEvaluator&,
        CLogger&) override;

private:
    static const uint8_t TEST_PACKET_SIZE;
    static const uint8_t TARGET_DEVICE_ID;
    static const uint8_t TARGET_DETECT_PREAMBLE_ATTEMPTS;
    static const uint8_t TARGET_DETECT_PREAMBLE_BYTE;
    static const uint8_t TARGET_DETECT_PREAMBLE_BYTE_RESPONSE;
};