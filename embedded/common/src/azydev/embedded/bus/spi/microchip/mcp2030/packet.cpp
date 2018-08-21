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
//#include <azydev/embedded/bus/spi/microchip/mcp2030/packet.h>
//
//#include <azydev/embedded/util/binary.h>
//
//// TODO HACK: Remove defines
//#define BIT_OFFSET_COMMAND 13
//#define BIT_OFFSET_REG_ADDRESS 9
//#define BIT_OFFSET_DATA 1
//#define BIT_OFFSET_PARITY 0
//
//#define BIT_LENGTH_COMMAND 3
//#define BIT_LENGTH_REG_ADDRESS 4
//#define BIT_LENGTH_DATA 8
//#define BIT_LENGTH_PARITY 1
//
///* PUBLIC */
//
//CMCP2030Packet::CMCP2030Packet()
//    : m_payload(0) {
//}
//
//CMCP2030Packet::~CMCP2030Packet() {
//}
//
//void CMCP2030Packet::InitCmd(CMCP2030Packet::COMMAND command) {
//    m_payload =
//        Binary::BR<uint8_t>(BIT_OFFSET_COMMAND, BIT_LENGTH_COMMAND, static_cast<uint8_t>(command));
//}
//
//void CMCP2030Packet::InitRegRead(CMCP2030Packet::COMMAND command, uint8_t regAddress) {
//    m_payload =
//        Binary::BR<uint8_t>(BIT_OFFSET_COMMAND, BIT_LENGTH_COMMAND, static_cast<uint8_t>(command))
//        | Binary::BR<uint8_t>(BIT_OFFSET_REG_ADDRESS, BIT_LENGTH_REG_ADDRESS, regAddress);
//}
//
//void CMCP2030Packet::InitRegWrite(
//    CMCP2030Packet::COMMAND command,
//    uint8_t regAddress,
//    uint8_t data) {
//    // TODO IMPLEMENT: Calculate parity
//
//    uint8_t parity = Binary::IsParityOdd<uint8_t>(data);
//
//    m_payload =
//        Binary::BR<uint8_t>(BIT_OFFSET_COMMAND, BIT_LENGTH_COMMAND, static_cast<uint8_t>(command))
//        | Binary::BR<uint8_t>(BIT_OFFSET_REG_ADDRESS, BIT_LENGTH_REG_ADDRESS, regAddress)
//        | Binary::BR<uint8_t>(BIT_OFFSET_DATA, BIT_LENGTH_DATA, data)
//        | Binary::BR<uint8_t>(BIT_OFFSET_PARITY, BIT_LENGTH_PARITY, parity);
//}
//
//void CMCP2030Packet::InitRaw(uint8_t byteUpper, uint8_t byteLower) {
//    // explicit cast used to ensure that implicit integer promotion doesn't hide subtle bugs
//    m_payload = static_cast<uint16_t>(byteUpper) << 8 | byteLower;
//}
//
//uint8_t CMCP2030Packet::GetByteUpper() const {
//    return static_cast<uint8_t>(Binary::BCR<uint16_t>(m_payload, 8, 8));
//}
//
//uint8_t CMCP2030Packet::GetByteLower() const {
//    return static_cast<uint8_t>(Binary::BCR<uint16_t>(m_payload, 0, 8));
//}
//
//uint8_t CMCP2030Packet::GetData() const {
//    return static_cast<uint8_t>(Binary::BCR<uint16_t>(m_payload, BIT_OFFSET_DATA, BIT_LENGTH_DATA));
//}
//
//// TODO HACK: Get rid of this; just for debugging
//uint16_t CMCP2030Packet::GetPayload() const {
//    return m_payload;
//}