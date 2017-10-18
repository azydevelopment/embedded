/*
* Copyright (C) Andrew Yeung - All Rights Reserved
* Unauthorized copying of this file, via any medium is strictly prohibited
* Proprietary and confidential
* Written by Andrew Yeung <azy.development@gmail.com>, May 2017
*/

#pragma once

#include <stdint.h>

#include <azydev/embedded/bus/spi/common/entity.h>

class CSPIDevice;

template<typename TRANSFER_PRIMITIVE>
class CSPIBus : public CSPIEntity
{
public:
    enum class CLOCK_POLARITY : uint8_t
    {
        UNDEFINED = 255,
        IDLE_LOW  = 0,
        IDLE_HIGH = 1
    };

    enum class CLOCK_PHASE : uint8_t
    {
        UNDEFINED       = 255,
        SAMPLE_LEADING  = 0,
        SAMPLE_TRAILING = 1
    };

    enum class ENDIANNESS : uint8_t
    {
        UNDEFINED = 255,
        BIG       = 0,
        LITTLE    = 1
    };

    enum class DUPLEX_MODE : uint8_t
    {
        UNDEFINED  = 255,
        FULL       = 0,
        HALF_WRITE = 1,
        HALF_READ  = 2
    };

    struct CONFIG_DESC
    {
        CLOCK_POLARITY clock_polarity  = CLOCK_POLARITY::UNDEFINED;
        CLOCK_PHASE clock_phase        = CLOCK_PHASE::UNDEFINED;
        ENDIANNESS endianness          = ENDIANNESS::UNDEFINED;
        DUPLEX_MODE duplex_mode_intial = DUPLEX_MODE::UNDEFINED;
    };

    struct DESC : CSPIEntity::DESC
    {
        uint8_t num_devices  = 0;
        CSPIDevice** devices = nullptr;
    };

    // destructor
    virtual ~CSPIBus() override;

    // NVI
    virtual void SetConfig(const CONFIG_DESC&) final;
    virtual STATUS SetDeviceRole(const uint8_t deviceId, const ROLE) final;
    virtual STATUS SetEnabled(const bool) final;
    virtual STATUS SetDuplexMode(const DUPLEX_MODE) final;
    virtual STATUS Start(const uint8_t deviceId) final;
    virtual STATUS Write(const TRANSFER_PRIMITIVE) final;
    virtual STATUS Read(TRANSFER_PRIMITIVE&) final;
    virtual STATUS Stop() final;

protected:
    // constructor
    CSPIBus(const DESC&);

private:
    // rule of three
    CSPIBus(const CSPIBus&);
    CSPIBus& operator=(const CSPIBus&);

    // member variables
    uint8_t m_num_devices;
    CSPIDevice** m_devices;
    bool m_enabled;
    uint8_t m_active_device_id;
    DUPLEX_MODE m_duplex_mode;

    // abstract
    virtual void SetConfig_impl(const CONFIG_DESC&)      = 0;
    virtual STATUS SetEnabled_impl(const bool)           = 0;
    virtual STATUS SetDuplexMode_impl(const DUPLEX_MODE) = 0;
    virtual STATUS Start_impl(const uint8_t deviceId)    = 0;
    virtual STATUS Write_impl(const TRANSFER_PRIMITIVE)  = 0;
    virtual STATUS Read_impl(TRANSFER_PRIMITIVE&)        = 0;
    virtual STATUS Stop_impl()                           = 0;
};
