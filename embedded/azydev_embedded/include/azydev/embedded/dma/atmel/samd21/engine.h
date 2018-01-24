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

#include <azydev/embedded/dma/common/engine.h>

#include <azydev/embedded/dma/atmel/samd21/channel.h>

#include <stdint.h>

class CDMAEngineAtmelSAMD21 final : public CDMAEngine
{
public:
    struct CONFIG_DESC : CDMAEngine::CONFIG_DESC
    {};

    struct DESC : CDMAEngine::DESC
    {};

    // constructor
    CDMAEngineAtmelSAMD21(const DESC&);

    // destructor
    virtual ~CDMAEngineAtmelSAMD21() override final;

private:
    enum class REG_CTRL : uint8_t
    {
        SWRST,
        DMAENABLE,
        CRCENABLE,
        LVLENX = 8
    };

    // rule of three
    CDMAEngineAtmelSAMD21(const CDMAEngineAtmelSAMD21&);
    CDMAEngineAtmelSAMD21& operator=(const CDMAEngineAtmelSAMD21&);

    // member variables
    uint8_t m_num_channels;
    CONFIG_DESC m_config;
    CDMAChannel** m_channels;

    // member functions
    uint8_t GetNumChannels() const;
    void SetEnablePriority(const CDMAChannelAtmelSAMD21::PRIORITY, const bool enabled);

    // CDMAEngine
    virtual void SetConfig_impl(const CDMAEngine::CONFIG_DESC&) override final;
    virtual void SetEnabled_impl(const bool) override final;
    virtual CDMAChannel* AcquireFreeChannel_impl() override final;
};