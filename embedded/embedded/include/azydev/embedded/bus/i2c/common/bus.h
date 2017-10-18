/*
* Copyright (C) Andrew Yeung - All Rights Reserved
* Unauthorized copying of this file, via any medium is strictly prohibited
* Proprietary and confidential
* Written by Andrew Yeung <azy.development@gmail.com>, May 2017
*/

#pragma once

#include <stdint.h>

#include <azydev/embedded/bus/i2c/common/entity.h>

class CI2CBus : public CI2CEntity
{
public:
    enum class TRANSFER_DIRECTION : uint8_t
    {
        WRITE = 0,
        READ  = 1
    };

    struct CONFIG_DESC
    {};

    // destructor
    virtual ~CI2CBus() override;

    // NVI
    virtual void SetConfig(const CONFIG_DESC&) final;
    virtual STATUS SetEnabled(bool) final;
    virtual STATUS Start(const uint8_t address, const TRANSFER_DIRECTION) final;
    virtual STATUS Write(const uint8_t) final;
    virtual STATUS Read(uint8_t& outData) final;
    virtual STATUS Stop() final;

protected:
    // constructor
    CI2CBus(const DESC&);

private:
    // rule of three
    CI2CBus(const CI2CBus&);
    CI2CBus& operator=(const CI2CBus&);

    // member variables
    bool m_enabled;
    bool m_transfer_in_progress;

    // abstract
    virtual void SetConfig_impl(const CONFIG_DESC&) = 0;
    virtual STATUS SetEnabled_impl(const bool)        = 0;
    virtual STATUS Start_impl(const uint8_t address, const TRANSFER_DIRECTION) = 0;
    virtual STATUS Write_impl(const uint8_t)   = 0;
    virtual STATUS Read_impl(uint8_t& outData) = 0;
    virtual STATUS Stop_impl()                 = 0;
};
