/*
* Copyright (C) Andrew Yeung - All Rights Reserved
* Unauthorized copying of this file, via any medium is strictly prohibited
* Proprietary and confidential
* Written by Andrew Yeung <azy.development@gmail.com>, May 2017
*/

#pragma once

#include <azydev/embedded/pins/common/pins.h>

#include <stdint.h>

class CPinsAtmelSAMD21 final : public CPins<uint32_t>
{
public:
    enum class MUX_TYPE : uint8_t
    {
        UNDEFINED  = 255,
        GPIO       = 1 << 7,
        PERIPHERAL = 0
    };

    struct CONFIG_DESC : CPins::CONFIG_DESC
    { MUX_TYPE mux_type = MUX_TYPE::UNDEFINED; };

    // constructor
    CPinsAtmelSAMD21();

    // destructor
    virtual ~CPinsAtmelSAMD21() override final;

    // copy constructor
    CPinsAtmelSAMD21(const CPinsAtmelSAMD21&);

private:
    // rule of three
    CPinsAtmelSAMD21& operator=(const CPinsAtmelSAMD21&);

    // CPins
    virtual void SetPinConfig_impl(const uint32_t pinId, const CPins::CONFIG_DESC&) override final;
    virtual void PinWriteDigital_impl(const uint32_t pinId, const DIGITAL_STATE) override final;
    virtual uint8_t PinReadDigital_impl(const uint32_t pinId) const override final;
    virtual void EnableInterrupt_impl(
        const uint32_t pinId,
        const INTERRUPT_TRIGGER,
        const bool ignorePending) override final;
    virtual void DisableInterrupt_impl(const uint32_t pinId) override final;
};