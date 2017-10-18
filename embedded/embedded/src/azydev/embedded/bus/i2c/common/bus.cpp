/*
* Copyright (C) Andrew Yeung - All Rights Reserved
* Unauthorized copying of this file, via any medium is strictly prohibited
* Proprietary and confidential
* Written by Andrew Yeung <azy.development@gmail.com>, May 2017
*/

#pragma once

#include <azydev/embedded/bus/i2c/common/bus.h>

/* PUBLIC */

CI2CBus::~CI2CBus() {
}

// NVI

void CI2CBus::SetConfig(const CONFIG_DESC& config) {
    SetConfig_impl(config);
}

CI2CEntity::STATUS CI2CBus::SetEnabled(const bool enabled) {
    STATUS status = SetEnabled_impl(enabled);
    m_enabled = enabled;
    return status;
}

CI2CEntity::STATUS CI2CBus::Start(uint8_t address, const TRANSFER_DIRECTION direction) {
    // TODO ERROR_HANDLING: Check for transfer already in progress
    STATUS status = Start_impl(address, direction);
    m_transfer_in_progress = true;
    return status;
}

CI2CEntity::STATUS CI2CBus::Write(uint8_t data) {
    return Write_impl(data);
}

CI2CEntity::STATUS CI2CBus::Read(uint8_t& outData) {
    return Read_impl(outData);
}

CI2CEntity::STATUS CI2CBus::Stop() {
    STATUS status = Stop_impl();
    m_transfer_in_progress = false;
    return status;
}

/* PROTECTED */

CI2CBus::CI2CBus(const DESC& desc)
    : CI2CEntity(desc)
    , m_enabled(false)
    , m_transfer_in_progress(false) {
}

