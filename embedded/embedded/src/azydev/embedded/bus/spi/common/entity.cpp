/*
* Copyright (C) Andrew Yeung - All Rights Reserved
* Unauthorized copying of this file, via any medium is strictly prohibited
* Proprietary and confidential
* Written by Andrew Yeung <azy.development@gmail.com>, May 2017
*/

#include <azydev/embedded/bus/spi/common/entity.h>

/* PUBLIC */

CSPIEntity::~CSPIEntity() {
}

uint8_t CSPIEntity::GetId() const {
    return m_id;
}

CSPIEntity::STATUS CSPIEntity::SetRole(const ROLE role) {
    STATUS status = SetRole_impl(role);
    m_role = role;
    return status;
}

/* PROTECTED */

CSPIEntity::CSPIEntity(const CSPIEntity::DESC& desc)
    : m_id(desc.id)
    , m_role(ROLE::UNDEFINED) {
}