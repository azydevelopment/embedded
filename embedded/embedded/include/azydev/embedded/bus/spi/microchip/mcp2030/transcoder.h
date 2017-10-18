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
//#pragma once
//
//#include <stdint.h>
//
//#include <azydev/embedded/bus/spi/common/bus.h>
//#include <azydev/embedded/bus/spi/common/transcoder.h>
//#include <azydev/embedded/bus/spi/microchip/mcp2030/packet.h>
//
//template<typename PIN_TYPE>
//class CPins;
//
//class CSPITranscoderMCP2030 final : public ISPITranscoder<uint8_t>
//{
//public:
//    enum class OUTPUT_ENABLE_FILTER_TIME_HIGH : uint8_t
//    {
//        DISABLED = 0,
//        MS_1     = 1,
//        MS_2     = 2,
//        MS_4     = 3
//    };
//
//    enum class OUTPUT_ENABLE_FILTER_TIME_LOW : uint8_t
//    {
//        MS_1 = 0,
//        MS_2 = 2,
//        MS_4 = 3
//    };
//
//    enum class ALERT_TRIGGER : uint8_t
//    {
//        PARITY_ERROR                  = 0,
//        PARITY_ERROR_OR_EXPIRED_ALARM = 1
//    };
//
//    enum class LFDATA_OUTPUT : uint8_t
//    {
//        DEMODULATED = 0,
//        CARRIER_CLK = 1,
//        RSSI        = 2
//    };
//
//    enum class CLOCK_DIVIDE : uint8_t
//    {
//        DIVIDE_1        = 0,
//        DIVIDE_1_OVER_4 = 1
//    };
//
//    enum class SENSITIVITY_REDUCTION : uint8_t
//    {
//        DB_0        = 0,
//        MINUS_DB_2  = 1,
//        MINUS_DB_4  = 2,
//        MINUS_DB_6  = 3,
//        MINUS_DB_8  = 4,
//        MINUS_DB_10 = 5,
//        MINUS_DB_12 = 6,
//        MINUS_DB_14 = 7,
//        MINUS_DB_16 = 8,
//        MINUS_DB_18 = 9,
//        MINUS_DB_20 = 10,
//        MINUS_DB_22 = 11,
//        MINUS_DB_24 = 12,
//        MINUS_DB_26 = 13,
//        MINUS_DB_28 = 14,
//        MINUS_DB_30 = 15
//    };
//
//    enum class MODULATION_DEPTH : uint8_t
//    {
//        PERCENT_33 = 0,
//        PERCENT_60 = 1,
//        PERCENT_14 = 2,
//        PERCENT_8  = 3
//    };
//
//    struct OUTPUT_ENABLE_FILTER_CONFIG_DESC
//    {
//        OUTPUT_ENABLE_FILTER_TIME_HIGH time_high = OUTPUT_ENABLE_FILTER_TIME_HIGH::DISABLED;
//        OUTPUT_ENABLE_FILTER_TIME_LOW time_low   = OUTPUT_ENABLE_FILTER_TIME_LOW::MS_1;
//    };
//
//    struct LC_ENABLE_CONFIG_DESC
//    {
//        bool disable_lcx = false;
//        bool disable_lcy = false;
//        bool disable_lcz = false;
//    };
//
//    struct LC_TUNING_CONFIG_DESC
//    {
//        // capcitance range is 0 to 63 in pF increments
//        uint8_t lcx_tuning_capacitance = 0;
//        uint8_t lcy_tuning_capacitance = 0;
//        uint8_t lcz_tuning_capacitance = 0;
//    };
//
//    struct LC_SENSITIVITY_REDUCTION_CONFIG_DESC
//    {
//        SENSITIVITY_REDUCTION lcx_sensitivity_reduction = SENSITIVITY_REDUCTION::DB_0;
//        SENSITIVITY_REDUCTION lcy_sensitivity_reduction = SENSITIVITY_REDUCTION::DB_0;
//        SENSITIVITY_REDUCTION lcz_sensitivity_reduction = SENSITIVITY_REDUCTION::DB_0;
//    };
//
//    struct PIN_CONFIG_DESC
//    {
//        // TODO HACK: Only allows devices with max 255 pins
//        uint8_t alert       = 255;
//        uint8_t rssi        = 255;
//        uint8_t lfdata_cclk = 255;
//    };
//
//    struct CONFIG_DESC
//    {
//        OUTPUT_ENABLE_FILTER_CONFIG_DESC output_enable_filter_config_desc         = {};
//        ALERT_TRIGGER alert_trigger                                               = ALERT_TRIGGER::PARITY_ERROR;
//        LC_ENABLE_CONFIG_DESC lc_enable_config_desc                               = {};
//        LFDATA_OUTPUT lfdata_output                                               = LFDATA_OUTPUT::DEMODULATED;
//        LC_TUNING_CONFIG_DESC lc_tuning_config_desc                               = {};
//        bool enable_rssi_fet                                                      = false;
//        CLOCK_DIVIDE clock_divide                                                 = CLOCK_DIVIDE::DIVIDE_1;
//        LC_SENSITIVITY_REDUCTION_CONFIG_DESC lc_sensitivity_reduction_config_desc = {};
//        bool auto_channel_select                                                  = false;
//        bool enable_agc                                                           = false;
//        MODULATION_DEPTH min_modulation_depth                                     = MODULATION_DEPTH::PERCENT_33;
//    };
//
//    struct STATUS
//    {
//        bool parity_error          = false;
//        bool alarm_timer_timed_out = false;
//        bool woken_up_channel_x    = false;
//        bool woken_up_channel_y    = false;
//        bool woken_up_channel_z    = false;
//        bool agc_active            = false;
//        bool channel_active_x      = false;
//        bool channel_active_y      = false;
//        bool channel_active_z      = false;
//    };
//
//    CSPITranscoderMCP2030(const PIN_CONFIG_DESC& pinConfig, CPins<uint8_t>* const servicePins);
//    virtual ~CSPITranscoderMCP2030();
//
//    virtual void Reset();
//
//    virtual void SetConfig(const CONFIG_DESC& configDesc);
//
//    virtual STATUS GetStatus();
//
//    // TODO HACK: Don't actually allow reading and writing using raw register addresses
//    virtual uint8_t ReadReg(uint8_t regAddress);
//    virtual void WriteReg(uint8_t regAddress, uint8_t data);
//
//    // ISPITranscoder
//    virtual void AttachToBus(CSPIBus<uint8_t>* bus, uint8_t deviceId);
//
//private:
//    CPins<uint8_t>* const m_service_pins;
//
//    uint8_t m_device_id;
//    CSPIBus<uint8_t>* m_spi_bus;
//
//    PIN_CONFIG_DESC m_pin_config_desc;
//    CONFIG_DESC m_config_desc;
//
//    virtual void WriteCmd(CMCP2030Packet::COMMAND command);
//
//    // SPI helpers
//    // MCP2030 datasheet figures 5-10 and 5-11
//    virtual void SPISetDuplexMode(CSPIBus<uint8_t>::DUPLEX_MODE duplexMode);
//    virtual void SPIBeginTransmission();
//    virtual void SPIEndTransmission();
//    virtual uint8_t SPITransfer(uint8_t data);
//};