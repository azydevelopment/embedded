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
