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
//#include <azydev/embedded/bus/spi/common/transcoder.h>
//
//class ITime;
//
//template<typename TRANSFER_TYPE>
//class CSPIBus;
//
//// TODO IMPLEMENT: Test register config descs
//
//class CSPITranscoderCC1101 final : public ISPITranscoder<uint8_t>
//{
//public:
//    enum class TRANSACTION_MODE : uint8_t
//    {
//        WRITE_BURST = 0x40,
//        READ_SINGLE = 0x80,
//        READ_BURST  = 0xC0
//    };
//
//    enum class STATE : uint8_t
//    {
//        IDLE             = 0,
//        RX               = 1,
//        TX               = 2,
//        FSTXON           = 3,
//        CALIBRATE        = 4,
//        SETTLING         = 5,
//        RXFIFO_OVERFLOW  = 6,
//        TXFIFO_UNDERFLOW = 7
//    };
//
//    enum class DATA_DIRECTION : uint8_t
//    {
//        TX = 0,
//        RX = 1
//    };
//
//    enum class PACKET_LENGTH : uint8_t
//    {
//        FIXED    = 0,
//        VARIABLE = 1,
//        INFINITE = 2
//    };
//
//    enum class ADDRESS_CHECK : uint8_t
//    {
//        NONE               = 0,
//        NO_BROADCAST       = 1,
//        WITH_BROADCAST     = 2,
//        WITH_BROADCAST_255 = 3
//    };
//
//    enum class PACKET_FORMAT : uint8_t
//    {
//        NORMAL             = 0,
//        SYNCHRONOUS_SERIAL = 1,
//        RANDOM_TX          = 3,
//        ASYNC_SERIAL       = 4
//    };
//
//    enum class MODULATION_FORMAT : uint8_t
//    {
//        MOD_2FSK    = 0,
//        MOD_GFSK    = 1,
//        MOD_ASK_OOK = 3,
//        MOD_4FSK    = 4,
//        MOD_MSK     = 7
//    };
//
//    enum class SYNC_MODE : uint8_t
//    {
//        NONE                                        = 0,
//        BITS_15_OF_16                               = 1,
//        BITS_16_OF_16                               = 2,
//        BITS_30_OF_32                               = 3,
//        NONE_CARRIER_SENSE_ABOVE_THRESHOLD          = 4,
//        BITS_15_OF_16_CARRIER_SENSE_ABOVE_THRESHOLD = 5,
//        BITS_16_OF_16_CARRIER_SENSE_ABOVE_THRESHOLD = 6,
//        BITS_30_OF_32_CARRIER_SENSE_ABOVE_THRESHOLD = 7
//    };
//
//    enum class NUM_PREAMBLE : uint8_t
//    {
//        BYTES_2  = 0,
//        BYTES_3  = 1,
//        BYTES_4  = 2,
//        BYTES_6  = 3,
//        BYTES_8  = 4,
//        BYTES_12 = 5,
//        BYTES_16 = 6,
//        BYTES_24 = 7
//    };
//
//    enum class WOR_RESOLUTION : uint8_t
//    {
//        PERIODS_1        = 0,
//        PERIODS_2_POW_5  = 1,
//        PERIODS_2_POW_10 = 2,
//        PERIODS_2_POW_15 = 3
//    };
//
//    enum class EVENT_1_TIMEOUT : uint8_t
//    {
//        CLOCKS_4  = 0,
//        CLOCKS_6  = 1,
//        CLOCKS_8  = 2,
//        CLOCKS_12 = 3,
//        CLOCKS_16 = 4,
//        CLOCKS_24 = 5,
//        CLOCKS_32 = 6,
//        CLOCKS_48 = 7
//    };
//
//    enum class PO_TIMEOUT : uint8_t
//    {
//        COUNT_1   = 0,
//        COUNT_16  = 1,
//        COUNT_64  = 2,
//        COUNT_256 = 3
//    };
//
//    enum class AUTO_CALIBRATE_RADIO : uint8_t
//    {
//        NEVER                    = 0,
//        IDLE_TO_RXTX             = 1,
//        RXTX_TO_IDLE             = 2,
//        FOURTH_TIME_RXTX_TO_IDLE = 3
//    };
//
//    enum class ON_TX_PACKET_SENT_STATE_CHANGE : uint8_t
//    {
//        IDLE    = 0,
//        FXTXON  = 1,
//        STAY_TX = 2,
//        RX      = 3
//    };
//
//    enum class ON_RX_PACKET_RECEIVED_STATE_CHANGE : uint8_t
//    {
//        IDLE    = 0,
//        FXTXON  = 1,
//        TX      = 2,
//        STAY_RX = 3
//    };
//
//    enum class CCA_MODE : uint8_t
//    {
//        ALWAYS                                        = 0,
//        RSSI_BELOW_THRESHOLD                          = 1,
//        UNLESS_RECEIVING_PACKET                       = 2,
//        RSSI_BELOW_THRESHOLD_AND_NOT_RECEIVING_PACKET = 3
//    };
//
//    enum class BASE_FREQUENCY : uint8_t
//    {
//        MHZ_433 = 0,
//        MHZ_868 = 1,
//        MHZ_915 = 2
//    };
//
//    // TODO IMPLEMENT: PATABLE stuff
//
//    // CC1101 Table 41
//    struct IO_CONFIG_DESC
//    {
//        uint8_t gdo0_config          = 0x2e;
//        bool gdo0_invert             = false;
//        bool gdo0_enable_temp_sensor = false;
//        uint8_t gdo1_config          = 0x2e;
//        bool gdo1_invert             = false;
//        bool gdo1_drive_strength     = false;
//        uint8_t gdo2_config          = 0x29;
//        bool gdo2_invert             = false;
//    };
//
//    struct FIFO_THRESHOLDS_CONFIG_DESC
//    {
//        // TODO IMPLEMENT
//    };
//
//    struct PACKET_CONTROL_CONFIG_DESC
//    {
//        uint8_t preamble_quality_estimator_threshold = 0;
//        bool enable_crc_autoflush                    = false;
//        bool enable_append_status_rssi_lqi_crc       = true;
//        ADDRESS_CHECK address_check_config           = ADDRESS_CHECK::NONE;
//        bool enable_whiten_data                      = true;
//        PACKET_FORMAT packet_format                  = PACKET_FORMAT::NORMAL;
//        bool enable_crc                              = true;
//        PACKET_LENGTH packet_length_config           = PACKET_LENGTH::VARIABLE;
//    };
//
//    struct FREQUENCY_SYNTHESIZER_CONTROL_CONFIG_DESC
//    {
//        // TODO IMPLEMENT
//    };
//
//    struct MODEM_CONFIG_DESC
//    {
//        uint8_t channel_spacing_mantissa    = 248;
//        uint8_t channel_spacing_expontent   = 2;
//        NUM_PREAMBLE num_preamble_config    = NUM_PREAMBLE::BYTES_4;
//        bool forward_error_correction       = false;
//        SYNC_MODE sync_mode                 = SYNC_MODE::BITS_16_OF_16;
//        bool manchester_encoding            = false;
//        MODULATION_FORMAT modulation_format = MODULATION_FORMAT::MOD_2FSK;
//        bool disable_dc_blocking_filter     = false;
//        uint8_t symbol_rate_mantissa        = 34;
//        uint8_t symbol_rate_exponent        = 12;
//        uint8_t decimation_ratio            = 0;
//    };
//
//    struct RADIO_STATE_MACHINE_CONFIG_DESC
//    {
//        bool xosc_force_on                        = false;
//        bool enable_pin_radio_control             = false;
//        PO_TIMEOUT po_timeout                     = PO_TIMEOUT::COUNT_16;
//        AUTO_CALIBRATE_RADIO auto_calibrate_radio = AUTO_CALIBRATE_RADIO::NEVER;
//        ON_TX_PACKET_SENT_STATE_CHANGE on_tx_packet_state_change =
//            ON_TX_PACKET_SENT_STATE_CHANGE::IDLE;
//        ON_RX_PACKET_RECEIVED_STATE_CHANGE on_rx_packet_state_change =
//            ON_RX_PACKET_RECEIVED_STATE_CHANGE::IDLE;
//        CCA_MODE cca_mode                = CCA_MODE::RSSI_BELOW_THRESHOLD_AND_NOT_RECEIVING_PACKET;
//        uint8_t rx_time_sync_word_search = 7; // MCSM2.RX_TIME
//        bool rx_time_qual                = 0; // MCSM2.RX_TIME_QUAL
//        bool rx_time_rssi                = 0; // MCSM2.RX_TIME_RSSI
//    };
//
//    struct FREQUENCY_OFFSET_COMPENSATION_CONFIG_DESC
//    {
//        // TODO IMPLEMENT
//    };
//
//    struct BIT_SYNCHRONIZATION_CONFIG_DESC
//    {
//        // TODO IMPLEMENT
//    };
//
//    struct AGC_CONTROL_CONFIG_DESC
//    {
//        // TODO IMPLEMENT
//    };
//
//    struct WAKE_ON_RADIO_CONFIG_DESC
//    {
//        WOR_RESOLUTION resolution       = WOR_RESOLUTION::PERIODS_1;
//        bool calibrate_rc_oscillator    = true;
//        EVENT_1_TIMEOUT event_1_timeout = EVENT_1_TIMEOUT::CLOCKS_48;
//        bool disable_rc_oscillator      = true;
//        uint16_t event_0_timeout        = 0x876b;
//    };
//
//    struct FRONT_END_RX_TX_CONFIG_DESC
//    {
//        // TODO IMPLEMENT
//    };
//
//    struct FREQUENCY_SYNTHESIZER_CALIBRATION_CONFIG_DESC
//    {
//        // TODO IMPLEMENT
//    };
//
//    struct RC_OSCILLATOR_CONFIG_DESC
//    {
//        // TODO IMPLEMENT
//    };
//
//    struct RX_PACKET_METADATA
//    {
//        // TODO IMPLEMENT: Packet sizes larger than 255
//        uint8_t payload_length = 0;
//        uint8_t address        = 0;
//        uint8_t rssi           = 0;
//        uint8_t lqi            = 0;
//        bool crc_ok            = 1;
//    };
//
//    CSPITranscoderCC1101(ITime* const serviceTime);
//
//    virtual ~CSPITranscoderCC1101();
//
//    virtual void Reset();
//
//    virtual void SetSyncWord(uint16_t syncWord);
//    virtual void SetPacketLength(uint8_t packetLength);
//    virtual void SetDeviceAddress(uint8_t deviceAddress);
//    virtual void SetBaseFrequency(BASE_FREQUENCY frequency);
//    virtual void SetChannelNumber(uint8_t channelNumber);
//
//    virtual void SetIOConfig(const IO_CONFIG_DESC& desc);
//    virtual void SetPacketControlConfig(const PACKET_CONTROL_CONFIG_DESC& desc);
//    virtual void SetModemConfig(const MODEM_CONFIG_DESC& desc);
//    virtual void SetRadioStateMachineConfig(const RADIO_STATE_MACHINE_CONFIG_DESC& desc);
//    virtual void SetWakeOnRadioConfig(const WAKE_ON_RADIO_CONFIG_DESC& desc);
//
//    virtual uint8_t GetPartNumber();
//    virtual uint8_t GetVersionNumber();
//    virtual uint8_t GetStatusByte(DATA_DIRECTION dataDirection = DATA_DIRECTION::TX);
//
//    virtual void GotoState(STATE state);
//    virtual void WaitForState(STATE state);
//
//    virtual void EnableRcOscillator(bool enable);
//    virtual void BeginWakeOnRadio();
//
//    virtual void WriteTxPacket(const uint8_t* const payloadBuffer, uint8_t payloadLength);
//    virtual void ReadRxPacket(uint8_t* payloadBuffer, RX_PACKET_METADATA* metadata);
//
//    virtual void WaitForPacketRx();
//    virtual void WaitForPacketTx();
//
//    virtual void FlushTxFifo();
//    virtual void FlushRxFifo();
//
//    // ISPITranscoder
//    virtual void AttachToBus(CSPIBus<uint8_t>* bus, uint8_t deviceId);
//
//private:
//    enum CONFIG_REGISTER : uint8_t
//    {
//        CONFIG_REGISTER_IOCFG2   = 0x00,
//        CONFIG_REGISTER_IOCFG1   = 0x01,
//        CONFIG_REGISTER_IOCFG0   = 0x02,
//        CONFIG_REGISTER_FIFOTHR  = 0x03,
//        CONFIG_REGISTER_SYNC1    = 0x04,
//        CONFIG_REGISTER_SYNC0    = 0x05,
//        CONFIG_REGISTER_PKTLEN   = 0x06,
//        CONFIG_REGISTER_PKTCTRL1 = 0x07,
//        CONFIG_REGISTER_PKTCTRL0 = 0x08,
//        CONFIG_REGISTER_ADDR     = 0x09,
//        CONFIG_REGISTER_CHANNR   = 0x0A,
//        CONFIG_REGISTER_FSCTRL1  = 0x0B,
//        CONFIG_REGISTER_FSCTRL0  = 0x0C,
//        CONFIG_REGISTER_FREQ2    = 0x0D,
//        CONFIG_REGISTER_FREQ1    = 0x0E,
//        CONFIG_REGISTER_FREQ0    = 0x0F,
//        CONFIG_REGISTER_MDMCFG4  = 0x10,
//        CONFIG_REGISTER_MDMCFG3  = 0x11,
//        CONFIG_REGISTER_MDMCFG2  = 0x12,
//        CONFIG_REGISTER_MDMCFG1  = 0x13,
//        CONFIG_REGISTER_MDMCFG0  = 0x14,
//        CONFIG_REGISTER_DEVIATN  = 0x15,
//        CONFIG_REGISTER_MCSM2    = 0x16,
//        CONFIG_REGISTER_MCSM1    = 0x17,
//        CONFIG_REGISTER_MCSM0    = 0x18,
//        CONFIG_REGISTER_FOCCFG   = 0x19,
//        CONFIG_REGISTER_BSCFG    = 0x1A,
//        CONFIG_REGISTER_AGCCTRL2 = 0x1B,
//        CONFIG_REGISTER_AGCCTRL1 = 0x1C,
//        CONFIG_REGISTER_AGCCTRL0 = 0x1D,
//        CONFIG_REGISTER_WOREVT1  = 0x1E,
//        CONFIG_REGISTER_WOREVT0  = 0x1F,
//        CONFIG_REGISTER_WORCTRL  = 0x20,
//        CONFIG_REGISTER_FREND1   = 0x21,
//        CONFIG_REGISTER_FREND0   = 0x22,
//        CONFIG_REGISTER_FSCAL3   = 0x23,
//        CONFIG_REGISTER_FSCAL2   = 0x24,
//        CONFIG_REGISTER_FSCAL1   = 0x25,
//        CONFIG_REGISTER_FSCAL0   = 0x26,
//        CONFIG_REGISTER_RCCTRL1  = 0x27,
//        CONFIG_REGISTER_RCCTRL0  = 0x28,
//        CONFIG_REGISTER_FSTEST   = 0x29,
//        CONFIG_REGISTER_PTEST    = 0x2A,
//        CONFIG_REGISTER_AGCTEST  = 0x2B,
//        CONFIG_REGISTER_TEST2    = 0x2C,
//        CONFIG_REGISTER_TEST1    = 0x2D,
//        CONFIG_REGISTER_TEST0    = 0x2E
//    };
//
//    enum COMMAND_STROBE : uint8_t
//    {
//        COMMAND_STROBE_SRES =
//            0x30, // Special strobe, must wait for MISO to go low to ensure reset is complete
//        COMMAND_STROBE_SFSTXON = 0x31,
//        COMMAND_STROBE_SXOFF   = 0x32,
//        COMMAND_STROBE_SCAL    = 0x33,
//        COMMAND_STROBE_SRX     = 0x34,
//        COMMAND_STROBE_STX     = 0x35,
//        COMMAND_STROBE_SIDLE   = 0x36,
//        COMMAND_STROBE_SAFC    = 0x37,
//        COMMAND_STROBE_SWOR    = 0x38,
//        COMMAND_STROBE_SPWD    = 0x39,
//        COMMAND_STROBE_SFRX    = 0x3A,
//        COMMAND_STROBE_SFTX    = 0x3B,
//        COMMAND_STROBE_SWORRST = 0x3C,
//        COMMAND_STROBE_SNOP    = 0x3D
//    };
//
//    enum STATUS_REGISTER : uint8_t
//    {
//        STATUS_REGISTER_PARTNUM    = 0x30,
//        STATUS_REGISTER_VERSION    = 0x31,
//        STATUS_REGISTER_FREQEST    = 0x32,
//        STATUS_REGISTER_LQI        = 0x33,
//        STATUS_REGISTER_RSSI       = 0x34,
//        STATUS_REGISTER_MARCSTATE  = 0x35,
//        STATUS_REGISTER_WORTIME1   = 0x36,
//        STATUS_REGISTER_WORTIME0   = 0x37,
//        STATUS_REGISTER_PKTSTATUS  = 0x38,
//        STATUS_REGISTER_VCO_VC_DAC = 0x39,
//        STATUS_REGISTER_TXBYTES    = 0x3A,
//        STATUS_REGISTER_RXBYTES    = 0x3B
//    };
//
//    enum FIFO_REGISTER : uint8_t
//    {
//        FIFO_REGISTER_TX = 0x3F,
//        FIFO_REGISTER_RX = 0x3F
//    };
//
//    ITime* const m_service_time;
//
//    uint8_t m_device_id;
//    CSPIBus<uint8_t>* m_spi_bus;
//
//    // current state
//    uint8_t m_packet_length                                      = 255;
//    uint8_t m_device_address                                     = 0;
//    IO_CONFIG_DESC m_io_config                                   = {};
//    RADIO_STATE_MACHINE_CONFIG_DESC m_radio_state_machine_config = {};
//    MODEM_CONFIG_DESC m_modem_config                             = {};
//    PACKET_CONTROL_CONFIG_DESC m_packet_control_config           = {};
//    WAKE_ON_RADIO_CONFIG_DESC m_wake_on_radio_desc               = {};
//
//    // SPI helpers
//    virtual void SPIBeginTransmission();
//    virtual void SPIEndTransmission();
//    virtual uint8_t SPITransfer(uint8_t data);
//
//    // other helpers
//    virtual uint8_t ReadConfigReg(CONFIG_REGISTER reg);
//    virtual uint8_t ReadStatusReg(STATUS_REGISTER reg);
//    virtual uint8_t WriteStrobe(COMMAND_STROBE strobe);
//    virtual void WriteConfigReg(CONFIG_REGISTER reg, uint8_t data);
//
//    virtual bool CompareState(uint8_t statusByte, STATE state);
//};
