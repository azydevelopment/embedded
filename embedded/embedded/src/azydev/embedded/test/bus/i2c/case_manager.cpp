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
//#include <azydev/embedded/test/bus/i2c/case_manager.h>
//
//#include <abdeveng/core/logging/common/logger.h>
//#include <abdeveng/core/test/common/evaluator.h>
//
//#include <azydev/embedded/bus/i2c/common/bus.h>
//#include <azydev/embedded/bus/i2c/common/fragment_data.h>
//#include <azydev/embedded/bus/i2c/common/fragment_start.h>
//#include <azydev/embedded/bus/i2c/common/fragment_stop.h>
//#include <azydev/embedded/bus/i2c/common/message.h>
//
///* STATICS */
//
// const uint8_t CTestCaseI2CManager::TEST_PACKET_SIZE      = 8;
// const uint8_t CTestCaseI2CManager::TARGET_DEVICE_ADDRESS = 0xAA;
//
///* PUBLIC */
//
// CTestCaseI2CManager::~CTestCaseI2CManager() {
//}
//
//// ITestCase
//
// void CTestCaseI2CManager::Run(
// volatile void* dut,
// const CTestHarness::ITestCase::TEST_CASE_CONFIG_DESC& config,
// const CEvaluator& evaluator,
// CLogger& logger) {
//
//// TODO HACK: const_cast
// CI2CBus& bus = *const_cast<CI2CBus*>(static_cast<volatile CI2CBus*>(dut));
//
// CI2CMessage i2cMessage = CI2CMessage();
//
// bool targetFound = false;
//
//// run the test
// CI2CFragmentStart fragmentStartTx = CI2CFragmentStart();
// CI2CFragmentData fragmentDataTx   = CI2CFragmentData(TEST_PACKET_SIZE);
// CI2CFragmentStart fragmentStartRx = CI2CFragmentStart();
// CI2CFragmentData fragmentDataRx   = CI2CFragmentData(TEST_PACKET_SIZE);
// CI2CFragmentStop fragmentStop     = CI2CFragmentStop();
//
// CI2CFragment* fragments[5] = {
//&fragmentStartTx, &fragmentDataTx, &fragmentStartRx, &fragmentDataRx, &fragmentStop};
//
// logger.Log("Preparing I2C message fragments");
//
//// prepare the message fragments
//{
// fragmentStartTx.SetSlarw(TARGET_DEVICE_ADDRESS, CI2CFragment::TRANSMISSION_TYPE::WRITE);
// fragmentStartRx.SetSlarw(TARGET_DEVICE_ADDRESS, CI2CFragment::TRANSMISSION_TYPE::READ);
//
//// create the TX payload
//// TODO IMPLEMENT: Should we allow this test to expand beyond a 256 byte payload?
// uint8_t payloadTx[TEST_PACKET_SIZE];
// for (uint8_t i = 0; i < TEST_PACKET_SIZE; i++) {
// payloadTx[i] = i;
//}
//
//// set payload and expected payload size for the manager transmit state
// fragmentDataTx.WriteBytes(payloadTx, TEST_PACKET_SIZE);
//
//// set the expected payload size for the manager receiver part of the message to the same
//// size as the TX payload
// fragmentDataRx.SetExpectedPayloadSize(TEST_PACKET_SIZE);
//}
//
//// process the actual message
//// TODO HACK: Hardcoded
// i2cMessage.Init(fragments, 5);
// bus.ProcessMessage(i2cMessage);
//
// logger.Log("Processing I2C message");
//
//// wait until we can evaluate the test results
// while (bus.IsTransferInProgress()) {
//}
//
// logger.Log("Validating I2C responses");
//
//// validate no errors during transmission
// if (!evaluator.Validate(!i2cMessage.IsInErrorState())) {
// return;
//}
//
//// check the RX fragment has the expected data
//{
//// generate expected payload
// uint8_t payloadExpected[TEST_PACKET_SIZE];
// for (int i = 0; i < TEST_PACKET_SIZE; i++) {
// payloadExpected[i] = TEST_PACKET_SIZE - i;
//}
//
//// get actual payload
// const uint8_t* payloadActual      = fragmentDataRx.GetWrittenData();
// const uint8_t payloadLengthActual = fragmentDataRx.GetWrittenDataLength();
//
//// execute the RX payload validation
// CEvaluator::EVALUATION<uint8_t> evaluation;
// evaluation.len_payload_actual = payloadLengthActual;
// evaluation.payload_actual     = const_cast<uint8_t*>(
// payloadActual); // TODO HACK: Hmmm, would prefer if didn't need cast
// evaluation.len_payload_expected = TEST_PACKET_SIZE;
// evaluation.payload_expected     = payloadExpected;
//
// evaluator.Validate<uint8_t>(evaluation, CEvaluator::COMPARATOR::EQUAL);
//}
//}
//
///* PRIVATE */
