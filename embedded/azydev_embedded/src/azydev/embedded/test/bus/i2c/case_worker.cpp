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
//#include <azydev/embedded/test/bus/i2c/case_worker.h>
//
//#include <abdeveng/core/logging/common/logger.h>
//#include <abdeveng/core/test/common/evaluator.h>
//
//#include <azydev/embedded/bus/i2c/common/bus.h>
//
///* STATICS */
//
// const uint8_t CTestCaseI2CWorker::TEST_PACKET_SIZE      = 8;
// const uint8_t CTestCaseI2CWorker::TARGET_DEVICE_ADDRESS = 0xAA;
//
///* PUBLIC */
//
// CTestCaseI2CWorker::~CTestCaseI2CWorker() {
//}
//
//// ITestCase
//
// void CTestCaseI2CWorker::Run(
//    volatile void* dut,
//    const CTestHarness::ITestCase::TEST_CASE_CONFIG_DESC& config,
//    const CEvaluator& evaluator,
//    CLogger& logger) {
//
//    // TODO HACK: const_cast
//    CI2CBus& bus = *const_cast<CI2CBus*>(static_cast<volatile CI2CBus*>(dut));
//
//    // TODO IMPLEMENT: Enable address change on CI2CBus
//
//    CI2CMessage i2cMessage = CI2CMessage();
//
//    bool targetFound = false;
//
//    // run the test
//    CI2CFragmentData fragmentDataRx = CI2CFragmentData(TEST_PACKET_SIZE);
//    CI2CFragmentData fragmentDataTx = CI2CFragmentData(TEST_PACKET_SIZE);
//
//    CI2CFragment* fragments[2] = {&fragmentDataRx, &fragmentDataTx};
//
//    logger.Log("Preparing I2C message fragments");
//
//    // prepare the message fragments
//    {
//        // set the expected payload size for the worker receiver part of the message to the same
//        size
//        // as the TX payload
//        fragmentDataRx.SetExpectedPayloadSize(TEST_PACKET_SIZE);
//
//        // create the TX payload
//        // TODO IMPLEMENT: Should we allow this test to expand beyond a 256 byte payload?
//        uint8_t payloadTx[TEST_PACKET_SIZE];
//        for (uint8_t i = 0; i < TEST_PACKET_SIZE; i++) {
//            payloadTx[i] = TEST_PACKET_SIZE - i;
//        }
//
//        // set payload and expected payload size for the worker transmit state
//        fragmentDataTx.WriteBytes(payloadTx, TEST_PACKET_SIZE);
//    }
//
//    // process the actual message
//    i2cMessage.Init(fragments, 2);
//    bus.ProcessMessage(i2cMessage);
//
//    logger.Log("Waiting to be asserted...", false);
//
//    // wait until we can evaluate the test results
//    while (bus.IsTransferInProgress()) {
//        // TODO ERROR_HANDLING: Watchdog timeout
//    }
//
//    logger.Log("success");
//
//    logger.Log("Validating I2C responses");
//
//    // validate no errors during transmission
//    if (!evaluator.Validate(!i2cMessage.IsInErrorState())) {
//        return;
//    }
//
//    // check the RX fragment has the expected data
//    {
//        // generate expected payload
//        uint8_t payloadExpected[TEST_PACKET_SIZE];
//        for (int i = 0; i < TEST_PACKET_SIZE; i++) {
//            payloadExpected[i] = i;
//        }
//
//        // get actual payload
//        const uint8_t* payloadActual      = fragmentDataRx.GetWrittenData();
//        const uint8_t payloadLengthActual = fragmentDataRx.GetWrittenDataLength();
//
//        // execute the RX payload validation
//        CEvaluator::EVALUATION<uint8_t> evaluation;
//        evaluation.len_payload_actual = payloadLengthActual;
//        evaluation.payload_actual     = const_cast<uint8_t*>(
//            payloadActual); // TODO HACK: Hmmm, would prefer if didn't need cast
//        evaluation.len_payload_expected = TEST_PACKET_SIZE;
//        evaluation.payload_expected     = payloadExpected;
//
//        evaluator.Validate<uint8_t>(evaluation, CEvaluator::COMPARATOR::EQUAL);
//    }
//}
//
///* PRIVATE */
