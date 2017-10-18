/*
* Copyright (C) Andrew Yeung - All Rights Reserved
* Unauthorized copying of this file, via any medium is strictly prohibited
* Proprietary and confidential
* Written by Andrew Yeung <azy.development@gmail.com>, May 2017
*/

#include <azydev/embedded/bus/spi/atmel/samd21/device.h>

#include <azydev/embedded/pins/atmel/samd21/pins.h>

CSPIDeviceAtmelSAMD21::CSPIDeviceAtmelSAMD21(const DESC& desc, CPinsAtmelSAMD21& pins)
    : CSPIDevice(desc)
    , m_service_pins(pins)
    , m_pin_config(desc.pin_config) {
}

CSPIDeviceAtmelSAMD21::~CSPIDeviceAtmelSAMD21() {
}

// CSPIDevice

CSPIEntity::STATUS CSPIDeviceAtmelSAMD21::SetRole_impl(const ROLE role) {
    // configure pins
    CPinsAtmelSAMD21::CONFIG_DESC configSs = {};
    switch (role) {
    case ROLE::MANAGER:
        configSs.direction = CPinsAtmelSAMD21::DIRECTION::IN;
        configSs.pull      = CPinsAtmelSAMD21::PULL::UP;
        break;
    case ROLE::WORKER:
        // set not active when setting as worker to default to not-asserted
        SetActive(false);
        configSs.direction = CPinsAtmelSAMD21::DIRECTION::OUT;
        configSs.pull      = CPinsAtmelSAMD21::PULL::NONE;
        break;
    default:
        // TODO ERROR_HANDLING
        break;
    }

    // set the pin mux type for SAMD21 devices
    configSs.mux_type = CPinsAtmelSAMD21::MUX_TYPE::GPIO;

    m_service_pins.SetPinConfig(m_pin_config.ss, configSs);

    return STATUS::OK;
}

CSPIEntity::STATUS CSPIDeviceAtmelSAMD21::SetActive_impl(const bool active) {
    // TODO IMPLEMENT address_enabled and address config setting specifically for a SAMD21 device
    CPinsAtmelSAMD21::DIGITAL_STATE pinState;
    if (active) {
        pinState = CPinsAtmelSAMD21::DIGITAL_STATE::DIGITAL_LOW;
    } else {
        pinState = CPinsAtmelSAMD21::DIGITAL_STATE::DIGITAL_HIGH;
    }
    m_service_pins.PinWriteDigital(m_pin_config.ss, pinState);

    return STATUS::OK;
}