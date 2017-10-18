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

template<typename BUS_TYPE, typename DEVICE_ID_TYPE>
class CBusTranscoder
{
public:
    // destructor
    virtual ~CBusTranscoder();

    // NVI
    virtual void AttachToBus(BUS_TYPE& bus, const DEVICE_ID_TYPE deviceId) final;

protected:
    // constructor
    CBusTranscoder();

    // methods
    virtual BUS_TYPE& GetBus() const final;
    virtual const DEVICE_ID_TYPE GetDeviceId() const final;

private:
    // rule of three
    CBusTranscoder(const CBusTranscoder&);
    CBusTranscoder& operator=(const CBusTranscoder&);

    // member variables
    BUS_TYPE* m_bus;
    DEVICE_ID_TYPE m_device_id;
};