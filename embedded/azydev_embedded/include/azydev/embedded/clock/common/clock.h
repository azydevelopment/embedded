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

class CClock
{
public:
    enum class STATE : uint8_t
    {
        CS0, // disabled
        CS1  // enabled
    };

    struct DESC
    { uint8_t id; };

    struct CONFIG_DESC
    {};

    // destructor
    virtual ~CClock();

    // NVI
    virtual uint8_t GetId() const final;
    virtual void SetConfig(const CONFIG_DESC&) final;
    virtual void Acquire() final;
    virtual void Release() final;

protected:
    // constructor
    CClock(const DESC&);

private:
    // rule of three
    CClock(const CClock&);
    CClock& operator=(const CClock&);

    // member variables
    uint8_t m_id;
    uint8_t m_num_consumers;
    STATE m_state;

    // final
    STATE GetState() const;
    void SetState(const STATE);

    // abstract
    virtual void SetConfig_impl(const CONFIG_DESC&) = 0;
    virtual void SetState_impl(const STATE)         = 0;
};