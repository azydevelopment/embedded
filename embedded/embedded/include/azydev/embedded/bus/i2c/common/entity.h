/*
* Copyright (C) Andrew Yeung - All Rights Reserved
* Unauthorized copying of this file, via any medium is strictly prohibited
* Proprietary and confidential
* Written by Andrew Yeung <azy.development@gmail.com>, May 2017
*/

#pragma once

#include <stdint.h>

class CI2CEntity
{
public:
    enum class STATUS : uint8_t {
        OK,
        ERROR_UNKNOWN,
        ERROR_TIMEOUT
    };

    enum class ROLE : uint8_t
    {
        UNDEFINED = 255,
        WORKER     = 0,
        MANAGER    = 1
    };

    struct DESC
    {
        uint8_t id = 255;
    };

    // destructor
    virtual ~CI2CEntity();

    // NVI
    virtual uint8_t GetId() const final;
    virtual STATUS SetRole(const ROLE) final;

protected:
    // constructor
    CI2CEntity(const DESC&);

private:
    // rule of three
    CI2CEntity(const CI2CEntity&);
    CI2CEntity& operator=(const CI2CEntity&);

    // member variables
    const uint8_t m_id;
    ROLE m_role;

    // abstract
    virtual STATUS SetRole_impl(const ROLE) = 0;
};