/*
* Copyright (C) Andrew Yeung - All Rights Reserved
* Unauthorized copying of this file, via any medium is strictly prohibited
* Proprietary and confidential
* Written by Andrew Yeung <azy.development@gmail.com>, May 2017
*/

#pragma once

#include <stdint.h>

#include <azydev/embedded/bus/spi/common/device.h>

class CPinsAtmelSAMD21;

class CSPIDeviceAtmelSAMD21 final : public CSPIDevice
{
public:
    struct PIN_CONFIG_DESC
    { uint32_t ss = 0; };

    struct DESC : CSPIDevice::DESC
    { PIN_CONFIG_DESC pin_config = {}; };

    CSPIDeviceAtmelSAMD21(const DESC&, CPinsAtmelSAMD21&);
    virtual ~CSPIDeviceAtmelSAMD21() override;

private:
    CPinsAtmelSAMD21& m_service_pins;
    PIN_CONFIG_DESC m_pin_config;

    // CSPIEntity
    virtual STATUS SetRole_impl(const ROLE) override final;

    // CSPIDevice
    virtual STATUS SetActive_impl(const bool) override final;
};
