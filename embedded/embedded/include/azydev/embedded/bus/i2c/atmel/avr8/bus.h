// TODO HACK: Didn't keep this up to date

///*
//* Copyright (C) Andrew Yeung - All Rights Reserved
//* Unauthorized copying of this file, via any medium is strictly prohibited
//* Proprietary and confidential
//* Written by Andrew Yeung <azy.development@gmail.com>, May 2017
//*/
//
//#pragma once
//
//#include <stdint.h>
//
//#include <azydev/embedded/bus/i2c/common/bus.h>
//
//#include <azydev/embedded/util/binary.h>
//
// template<typename PIN_TYPE>
// class CPins;
//
// class CI2CMessage;
//
// class CI2CBusAtmelAVR8 : public CI2CBus<uint8_t>
//{
// public:
//    enum class STATUS : uint8_t
//    {
//        MX_START          = 0x08,
//        MX_START_REPEATED = 0x10,
//        MX_ARB_LOST       = 0x38,
//
//        MT_SLAW_ACK  = 0x18,
//        MT_SLAW_NACK = 0x20,
//        MT_DATA_ACK  = 0x28,
//        MT_DATA_NACK = 0x30,
//
//        MR_SLAR_ACK  = 0x40,
//        MR_SLAR_NACK = 0x48,
//        MR_DATA_ACK  = 0x50,
//        MR_DATA_NACK = 0x58,
//
//        SR_SLAW_ACK                          = 0x60,
//        SR_ARBITRATION_LOST_SLAW_ACK         = 0x68,
//        SR_GENERAL_CALL_ACK                  = 0x70,
//        SR_ARBITRATION_LOST_GENERAL_CALL_ACK = 0x78,
//        SR_DATA_ACK                          = 0x80,
//        SR_DATA_NACK                         = 0x88,
//        SR_GENERAL_CALL_DATA_ACK             = 0x90,
//        SR_GENERAL_CALL_DATA_NACK            = 0x98,
//        SR_STOP_OR_START_REPEATED            = 0xA0,
//
//        ST_SLAR_ACK                  = 0xA8,
//        ST_ARBITRATION_LOST_SLAR_ACK = 0xB0,
//        ST_DATA_ACK                  = 0xB8,
//        ST_DATA_NACK                 = 0xC0,
//        ST_LAST_DATA_ACK             = 0xC8
//    };
//
//    struct REGISTER_CONFIG_DESC
//    {
//        volatile uint8_t* bitrate_register = nullptr;
//        volatile uint8_t* control_register = nullptr;
//        volatile uint8_t* data_register    = nullptr;
//
//        BIT_POINTER<uint8_t> interrupt_flag                  = {};
//        BIT_POINTER<uint8_t> enable_acknowledge              = {};
//        BIT_POINTER<uint8_t> start_condition                 = {};
//        BIT_POINTER<uint8_t> stop_condition                  = {};
//        BIT_POINTER<uint8_t> write_collision_flag            = {};
//        BIT_POINTER<uint8_t> enable                          = {};
//        BIT_POINTER<uint8_t> enable_interrupt                = {};
//        BIT_RANGE<uint8_t> status                            = {};
//        BIT_RANGE<uint8_t> prescalar                         = {};
//        BIT_RANGE<uint8_t> address                           = {};
//        BIT_POINTER<uint8_t> enable_general_call_recognition = {};
//        BIT_RANGE<uint8_t> address_mask                      = {};
//    };
//
//    struct PIN_CONFIG_DESC
//    {
//        uint8_t scl = 255;
//        uint8_t sda = 255;
//    };
//
//    struct DESC : CI2CBus::DESC
//    {
//        PIN_CONFIG_DESC pin_config           = {};
//        REGISTER_CONFIG_DESC register_config = {};
//    };
//
//    struct CONFIG_DESC : CI2CBusAtmel::CONFIG_DESC
//    {
//        uint8_t bit_rate                     = 0;
//        bool enable_interrupt                = false;
//        uint8_t prescalar                    = 0;
//        bool enable_general_call_recognition = false;
//        uint8_t address_mask                 = 0;
//
//        // TODO HACK: Do I really need this overload?
//        const volatile CONFIG_DESC& operator=(const CONFIG_DESC& desc) volatile {
//            bit_rate                        = desc.bit_rate;
//            enable_interrupt                = desc.enable_interrupt;
//            prescalar                       = desc.prescalar;
//            enable_general_call_recognition = desc.enable_general_call_recognition;
//            address_mask                    = desc.address_mask;
//            return desc;
//        }
//    };
//
//    // TODO HACK: How do I not restrict everything to literally one available bus
//    static volatile CI2CBusAtmelAVR8* g_i2c_bus_avr8;
//    static void I2CBusAVR8Init(const CI2CBusAtmelAVR8::DESC& desc, CPins<uint8_t>& servicePins);
//
//    CI2CBusAtmelAVR8(const DESC& desc, CPins<uint8_t>& servicePins);
//    virtual ~CI2CBusAtmelAVR8() override;
//
//    void UpdateStateMachine() volatile;
//
//    // CI2CBus
//    virtual void SetEnabled(bool) volatile override;
//    virtual void ProcessMessage(CI2CMessage&) volatile override;
//    virtual void SetConfig(const CI2CBus::CONFIG_DESC&) volatile override;
//
// private:
//    CPins<uint8_t>& m_service_pins;
//
//    CONFIG_DESC m_bus_config;
//    PIN_CONFIG_DESC m_pin_config;
//    REGISTER_CONFIG_DESC m_register_config;
//
//    uint8_t m_enabled_acknowledge;
//
//    STATUS GetStatus() volatile;
//    void UpdateConfigReg(bool twint, bool twsta, bool twsto, bool twwc) volatile;
//    void EnableAcknowledgements(bool) volatile;
//
//    // final
//    virtual void WaitForStop() volatile final;
//    virtual uint8_t ReadData() volatile final;
//    virtual void WriteData(uint8_t) volatile final;
//
//    // CI2CBus
//    virtual void ForceStop() volatile final override;
//};