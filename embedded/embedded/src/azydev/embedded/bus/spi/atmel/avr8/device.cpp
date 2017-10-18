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