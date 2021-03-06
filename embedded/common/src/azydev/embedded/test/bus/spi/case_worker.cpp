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

#include <azydev/embedded/test/bus/spi/case_worker.h>

#include <abdeveng/core/logging/common/logger.h>
#include <abdeveng/core/test/common/evaluator.h>

#include <azydev/embedded/bus/spi/common/bus.h>

/* STATICS */

const uint8_t CTestCaseSPIWorker::TEST_PACKET_SIZE                     = 200;
const uint8_t CTestCaseSPIWorker::TARGET_DETECT_PREAMBLE_ATTEMPTS      = 8;
const uint8_t CTestCaseSPIWorker::TARGET_DETECT_PREAMBLE_BYTE          = 0xAA;
const uint8_t CTestCaseSPIWorker::TARGET_DETECT_PREAMBLE_BYTE_RESPONSE = 0xBB;

/* PUBLIC */

CTestCaseSPIWorker::~CTestCaseSPIWorker() {
}

// ITestCase

void CTestCaseSPIWorker::Run(
    volatile void* dut,
    const CTestHarness::ITestCase::TEST_CASE_CONFIG_DESC& config,
    const CEvaluator& evaluator,
    CLogger& logger) {
    // TODO HACK: const_cast
    // CSPIBus is known to not be volatile so casting that away
    CSPIBus<uint8_t>& bus =
        *const_cast<CSPIBus<uint8_t>*>(static_cast<volatile CSPIBus<uint8_t>*>(dut));

    bus.SetRole(CSPIBus<uint8_t>::ROLE::WORKER);

    bool targetFound = false;

    logger.Log("Waiting to be asserted...", false);

    // wait for preamble byte and signal the test can begin
    while (!targetFound) {
        // TODO ERROR_HANDLING: Use watchdog to timeout if preamble byte not detected after some
        // time
        uint8_t data = 0;
        bus.Read(data);
        if (data == TARGET_DETECT_PREAMBLE_BYTE) {
            targetFound = true;

            // send the magic response to let the manager know we're ready to begin the test
            bus.Write(TARGET_DETECT_PREAMBLE_BYTE_RESPONSE);
        }
    }

    if (!targetFound) {
        logger.Log("fail");
        // TODO IMPLEMENT: Failure case (eg. watchdog timeout)
    } else {
        logger.Log("success");

        // generate TX payload
        uint8_t payloadTx[TEST_PACKET_SIZE];
        {
            for (uint8_t i = 0; i < TEST_PACKET_SIZE; i++) {
                // worker sends decrementing values starting with the test packet size
                payloadTx[i] = TEST_PACKET_SIZE - i;
            }
        }

        // create space for RX payload
        uint8_t payloadRx[TEST_PACKET_SIZE];

        // execute the transfer
        {
            for (uint8_t i = 0; i < TEST_PACKET_SIZE; i++) {
                // send outgoing
                bus.Write(payloadTx[i]);

                // read incoming
                bus.Read(payloadRx[i]);
            }
        }

        logger.Log("Validating SPI responses");

        // validate RX payload
        {
            // generate expected payload
            uint8_t payloadExpected[TEST_PACKET_SIZE];
            for (int i = 0; i < TEST_PACKET_SIZE; i++) {
                payloadExpected[i] = i;
            }

            // execute the RX payload validation
            CEvaluator::EVALUATION<uint8_t> evaluation;
            evaluation.len_payload_actual = TEST_PACKET_SIZE;
            evaluation.payload_actual =
                payloadRx; // TODO HACK: Hmmm, would prefer if didn't need cast
            evaluation.len_payload_expected = TEST_PACKET_SIZE;
            evaluation.payload_expected     = payloadExpected;

            evaluator.Validate<uint8_t>(evaluation, CEvaluator::COMPARATOR::EQUAL);
        }
    }
}

/* PRIVATE */
