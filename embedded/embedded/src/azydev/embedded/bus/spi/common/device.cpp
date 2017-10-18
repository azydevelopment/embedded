/*
* Copyright (C) Andrew Yeung - All Rights Reserved
* Unauthorized copying of this file, via any medium is strictly prohibited
* Proprietary and confidential
* Written by Andrew Yeung <azy.development@gmail.com>, May 2017
*/

#include <azydev/embedded/bus/spi/common/device.h>

/* PUBLIC */

CSPIDevice::~CSPIDevice() {
}

CSPIEntity::STATUS CSPIDevice::SetActive(const bool active) {
    STATUS status = SetActive_impl(active);
    m_active = active;
    return status;
}

/* PROTECTED */

CSPIDevice::CSPIDevice(const DESC& desc)
    : CSPIEntity(desc)
    , m_active(false) {
}