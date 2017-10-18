///*
//* Copyright (C) Andrew Yeung - All Rights Reserved
//* Unauthorized copying of this file, via any medium is strictly prohibited
//* Proprietary and confidential
//* Written by Andrew Yeung <azy.development@gmail.com>, May 2017
//*/
//
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