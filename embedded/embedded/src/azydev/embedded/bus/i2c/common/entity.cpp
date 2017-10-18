/*
* Copyright (C) Andrew Yeung - All Rights Reserved
* Unauthorized copying of this file, via any medium is strictly prohibited
* Proprietary and confidential
* Written by Andrew Yeung <azy.development@gmail.com>, May 2017
*/

#pragma once

#include <azydev/embedded/bus/i2c/common/entity.h>

/* PUBLIC */

CI2CEntity::~CI2CEntity() {
}

uint8_t CI2CEntity::GetId() const {
    return m_id;
}

CI2CEntity::STATUS CI2CEntity::SetRole(const ROLE role) {
    STATUS status = SetRole_impl(role);
    m_role = role;
    return status;
}

/* PROTECTED */

CI2CEntity::CI2CEntity(const DESC& desc)
    : m_id(desc.id)
    , m_role(ROLE::UNDEFINED) {
}
