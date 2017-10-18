/*
* Copyright (C) Andrew Yeung - All Rights Reserved
* Unauthorized copying of this file, via any medium is strictly prohibited
* Proprietary and confidential
* Written by Andrew Yeung <azy.development@gmail.com>, May 2017
*/

#pragma once

template<typename ARRAY_INDEX_TYPE, typename INPUT_PRIMITIVE_TYPE, typename OUTPUT_PRIMITIVE_TYPE>
class IDataCoder
{
public:
    virtual ~IDataCoder() {
    }

    virtual void Code(const INPUT_PRIMITIVE_TYPE input[], ARRAY_INDEX_TYPE inLength, OUTPUT_PRIMITIVE_TYPE output[]) const = 0;
};
