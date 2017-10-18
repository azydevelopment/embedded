/*
* Copyright (C) Andrew Yeung - All Rights Reserved
* Unauthorized copying of this file, via any medium is strictly prohibited
* Proprietary and confidential
* Written by Andrew Yeung <azy.development@gmail.com>, May 2017
*/

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