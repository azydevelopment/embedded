/*
* Copyright (C) Andrew Yeung - All Rights Reserved
* Unauthorized copying of this file, via any medium is strictly prohibited
* Proprietary and confidential
* Written by Andrew Yeung <azy.development@gmail.com>, May 2017
*/

#pragma once

#include <stdint.h>

class ITime
{
public:
    virtual void DelayMs(const uint32_t) const = 0;
    virtual void DelayUs(const uint32_t) const = 0;
};