///*
//* Copyright (C) Andrew Yeung - All Rights Reserved
//* Unauthorized copying of this file, via any medium is strictly prohibited
//* Proprietary and confidential
//* Written by Andrew Yeung <azy.development@gmail.com>, May 2017
//*/
//
//#include <azydev/embedded/bus/spi/atmel/avr8/device.h>
//
//#include <azydev/embedded/pins/common/pins.h>
//
//CSPIDeviceAtmelAVR8::CSPIDeviceAtmelAVR8(const DESC& desc, CPins<uint8_t>& servicePins)
//    : CSPIDeviceAtmel(desc)
//    , m_service_pins(servicePins)
//    , m_pin_config(desc.pin_config) {
//}
//
//CSPIDeviceAtmelAVR8::~CSPIDeviceAtmelAVR8() {
//}
//
//// CSPIDevice
//
//void CSPIDeviceAtmelAVR8::SetRole(ROLE role) {
//    // configure pins
//
//    CPins<uint8_t>::CONFIG_DESC configSs = {};
//    switch (role) {
//    case ROLE::MANAGER:
//        configSs.direction = CPins<uint8_t>::DIRECTION::IN;
//        configSs.pull      = CPins<uint8_t>::PULL::UP;
//        break;
//    case ROLE::WORKER:
//        // set not active when setting as worker to default to not-asserted
//        SetActive(false);
//        configSs.direction = CPins<uint8_t>::DIRECTION::OUT;
//        break;
//    }
//
//    m_service_pins.SetPinConfig(m_pin_config.ss, configSs);
//}
//
//void CSPIDeviceAtmelAVR8::SetActive(bool active) {
//    CPins<uint8_t>::DIGITAL_STATE pinState;
//    if (active) {
//        pinState = CPins<uint8_t>::DIGITAL_STATE::DIGITAL_LOW;
//    } else {
//        pinState = CPins<uint8_t>::DIGITAL_STATE::DIGITAL_HIGH;
//    }
//    m_service_pins.PinWriteDigital(m_pin_config.ss, pinState);
//}