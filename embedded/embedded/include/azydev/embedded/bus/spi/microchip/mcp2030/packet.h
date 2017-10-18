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
//#pragma once
//
//#include <stdint.h>
//
// class CMCP2030Packet final
//{
// public:
// enum class COMMAND : uint8_t
//{
// CLAMP_ON         = 0,
// CLAMP_OFF        = 1,
// SLEEP            = 2,
// AGC_PRESERVE_ON  = 3,
// AGC_PRESERVE_OFF = 4,
// SOFT_RESET       = 5,
// READ_REG         = 6,
// WRITE_REG        = 7
//};
//
// CMCP2030Packet();
// virtual ~CMCP2030Packet();
//
// virtual void InitCmd(COMMAND command);
// virtual void InitRegRead(COMMAND command, uint8_t regAddress);
// virtual void InitRegWrite(COMMAND command, uint8_t regAddress, uint8_t data);
// virtual void InitRaw(uint8_t byteUpper, uint8_t byteLower);
// virtual uint8_t GetData() const;
//
//// TODO HACK: Get rid of this; just for debugging
// virtual uint8_t GetByteUpper() const;
// virtual uint8_t GetByteLower() const;
// virtual uint16_t GetPayload() const;
//
// private:
// uint16_t m_payload;
//};