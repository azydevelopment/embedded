/*
* Copyright (C) Andrew Yeung - All Rights Reserved
* Unauthorized copying of this file, via any medium is strictly prohibited
* Proprietary and confidential
* Written by Andrew Yeung <azy.development@gmail.com>, May 2017
*/

#pragma once

#include <stdint.h>

#include <azydev/embedded/bus/spi/common/entity.h>

class CSPIDevice : public CSPIEntity
{
public:
    // destructor
    virtual ~CSPIDevice() override;

    // NVI
    virtual STATUS SetActive(const bool active) final;

protected:
    // constructor
    CSPIDevice(const DESC&);

private:
    // rule of three
    CSPIDevice(const CSPIDevice&);
    CSPIDevice& operator= (const CSPIDevice&);

    // member variables
    bool m_active;

    // abstract
    virtual STATUS SetActive_impl(const bool active) = 0;
};