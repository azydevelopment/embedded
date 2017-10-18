/*
* Copyright (C) Andrew Yeung - All Rights Reserved
* Unauthorized copying of this file, via any medium is strictly prohibited
* Proprietary and confidential
* Written by Andrew Yeung <azy.development@gmail.com>, May 2017
*/

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