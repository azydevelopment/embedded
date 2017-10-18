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
//#include <azydev/embedded/bus/spi/atmel/common/device.h>
//
//template<typename PIN_TYPE>
//class CPins;
//
//class CSPIDeviceAtmelAVR8 : public CSPIDeviceAtmel<uint8_t, uint8_t>
//{
//public:
//    CSPIDeviceAtmelAVR8(const DESC&, CPins<uint8_t>&);
//    virtual ~CSPIDeviceAtmelAVR8() override;
//
//    // CSPIDevice
//    virtual void SetRole(ROLE) override;
//    virtual void SetActive(bool) override;
//
//private:
//    CPins<uint8_t>& m_service_pins;
//
//    PIN_CONFIG_DESC m_pin_config;
//};
