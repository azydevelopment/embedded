/*
* Copyright (C) Andrew Yeung - All Rights Reserved
* Unauthorized copying of this file, via any medium is strictly prohibited
* Proprietary and confidential
* Written by Andrew Yeung <azy.development@gmail.com>, May 2017
*/

#pragma once

#include <stdint.h>

template<typename PIN_TYPE>
class CPins
{
public:
    enum class DIRECTION : uint8_t
    {
        UNDEFINED = 255,
        IN        = 0,
        OUT       = 1
    };

    enum class PULL : uint8_t
    {
        NONE = 0,
        UP   = 1,
        DOWN = 2
    };

    enum class DIGITAL_STATE : uint8_t
    {
        UNDEFINED    = 255,
        DIGITAL_LOW  = 0,
        DIGITAL_HIGH = 1
    };

    enum class INTERRUPT_TRIGGER : uint8_t
    {
        UNDEFINED       = 255,
        TRIGGER_CHANGE  = 1,
        TRIGGER_FALLING = 2,
        TRIGGER_RISING  = 3
    };

    struct CONFIG_DESC
    {
        DIRECTION direction = DIRECTION::UNDEFINED;
        PULL pull           = PULL::NONE;
    };

    // destructor
    virtual ~CPins();

    // NVI
    void SetPinConfig(const PIN_TYPE pinId, const CONFIG_DESC&);
    void PinWriteDigital(const PIN_TYPE pinId, const DIGITAL_STATE);
    uint8_t PinReadDigital(const PIN_TYPE pinId) const;
    void EnableInterrupt(const PIN_TYPE pinId, const INTERRUPT_TRIGGER, const bool ignorePending);
    void DisableInterrupt(const PIN_TYPE pinId);

protected:
    // constructor
    CPins();

private:
    // rule of three
    CPins(const CPins&);
    CPins& operator=(const CPins&);

    // abstract
    virtual void SetPinConfig_impl(const PIN_TYPE pinId, const CONFIG_DESC&)     = 0;
    virtual void PinWriteDigital_impl(const PIN_TYPE pinId, const DIGITAL_STATE) = 0;
    virtual uint8_t PinReadDigital_impl(const PIN_TYPE pinId) const = 0;
    virtual void EnableInterrupt_impl(
        const PIN_TYPE pinId,
        const INTERRUPT_TRIGGER,
        const bool ignorePending)                            = 0;
    virtual void DisableInterrupt_impl(const PIN_TYPE pinId) = 0;
};