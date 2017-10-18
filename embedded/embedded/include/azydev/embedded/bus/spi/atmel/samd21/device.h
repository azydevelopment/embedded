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
