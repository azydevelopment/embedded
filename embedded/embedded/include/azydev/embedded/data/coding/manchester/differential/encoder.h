/*
* Copyright (C) Andrew Yeung - All Rights Reserved
* Unauthorized copying of this file, via any medium is strictly prohibited
* Proprietary and confidential
* Written by Andrew Yeung <azy.development@gmail.com>, May 2017
*/

#pragma once

#include <azydev/embedded/data/coding/common/coder.h>

template<typename ARRAY_INDEX_TYPE, typename DATA_PRIMITIVE_TYPE>
class CManchesterDifferentialEncoder : public IDataCoder<ARRAY_INDEX_TYPE, DATA_PRIMITIVE_TYPE, DATA_PRIMITIVE_TYPE>
{
public:
    CManchesterDifferentialEncoder();
    virtual ~CManchesterDifferentialEncoder();

    // IDataDecoder

    virtual void Code(const DATA_PRIMITIVE_TYPE input[], ARRAY_INDEX_TYPE inLength, DATA_PRIMITIVE_TYPE output[]) const;
};
