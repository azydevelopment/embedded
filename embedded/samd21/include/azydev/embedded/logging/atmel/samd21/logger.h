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

#include <abdeveng/core/logging/common/logger.h>

#include <asf.h>
#include <stdint.h>

class CLoggerAtmelSAMD21 final : public CLogger
{
public:
    // constructor
    CLoggerAtmelSAMD21();

    // destructor
    virtual ~CLoggerAtmelSAMD21() override final;

    // copy constructor
    CLoggerAtmelSAMD21(const CLoggerAtmelSAMD21&);

private:
    // rule of three
    CLoggerAtmelSAMD21& operator=(const CLoggerAtmelSAMD21&);

    // member variables
    struct usart_module m_usart;

    // CLogger
    void Init_impl() override final;
    void SetEnabled_impl(const bool) override final;
    void Log_impl(const char[], const bool eol = true) override final;
};