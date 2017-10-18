/*
* Copyright (C) Andrew Yeung - All Rights Reserved
* Unauthorized copying of this file, via any medium is strictly prohibited
* Proprietary and confidential
* Written by Andrew Yeung <azy.development@gmail.com>, May 2017
*/

#pragma once

#include <stdint.h>

class ITemplate
{
public:
    // destructor
    virtual ~ITemplate(){};

    // NVI

protected:
    // constructor
    ITemplate();

private:
    // rule of three
    ITemplate(const ITemplate&);
    ITemplate& operator=(const ITemplate&);

    // member variables

    // abstract
};