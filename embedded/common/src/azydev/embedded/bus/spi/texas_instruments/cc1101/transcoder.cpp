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
//#include <azydev/embedded/bus/spi/texas_instruments/cc1101/transcoder.h>
//
//#include <azydev/embedded/bus/spi/common/bus.h>
//#include <azydev/embedded/time/common/time.h>
//
///* PUBLIC */
//
//CSPITranscoderCC1101::CSPITranscoderCC1101(ITime* const serviceTime)
//    : m_service_time(serviceTime) {
//}
//
//CSPITranscoderCC1101::~CSPITranscoderCC1101() {
//}
//
//void CSPITranscoderCC1101::Reset() {
//    // CC1101 spec 19.1.1
//    // TODO IMPLEMENT: Can opt to use CHIP_RDYn instead of manual reset
//    m_spi_bus->BeginTransmission(m_device_id);
//    m_service_time->DelayMs(1);
//    m_spi_bus->EndTransmission();
//    m_service_time->DelayMs(1);
//    WriteStrobe(COMMAND_STROBE_SRES);
//}
//
//void CSPITranscoderCC1101::SetSyncWord(uint16_t syncWord) {
//    WriteConfigReg(CONFIG_REGISTER_SYNC0, syncWord & 0x00FF);
//    WriteConfigReg(CONFIG_REGISTER_SYNC1, (syncWord & 0xFF00) >> 8);
//}
//
//void CSPITranscoderCC1101::SetPacketLength(uint8_t packetLength) {
//    WriteConfigReg(CONFIG_REGISTER_PKTLEN, packetLength);
//    m_packet_length = packetLength;
//}
//
//void CSPITranscoderCC1101::SetDeviceAddress(uint8_t deviceAddress) {
//    WriteConfigReg(CONFIG_REGISTER_ADDR, deviceAddress);
//    m_device_address = deviceAddress;
//}
//
//void CSPITranscoderCC1101::SetBaseFrequency(BASE_FREQUENCY frequency) {
//    // TODO HACK: Values written to registers are hardcoded
//    switch (frequency) {
//    case BASE_FREQUENCY::MHZ_433:
//        WriteConfigReg(CONFIG_REGISTER_FREQ0, 0x62);
//        WriteConfigReg(CONFIG_REGISTER_FREQ1, 0xA7);
//        WriteConfigReg(CONFIG_REGISTER_FREQ2, 0x10);
//        break;
//    case BASE_FREQUENCY::MHZ_868:
//        WriteConfigReg(CONFIG_REGISTER_FREQ0, 0x76);
//        WriteConfigReg(CONFIG_REGISTER_FREQ1, 0x62);
//        WriteConfigReg(CONFIG_REGISTER_FREQ2, 0x21);
//        break;
//    case BASE_FREQUENCY::MHZ_915:
//        WriteConfigReg(CONFIG_REGISTER_FREQ0, 0x3B);
//        WriteConfigReg(CONFIG_REGISTER_FREQ1, 0xB1);
//        WriteConfigReg(CONFIG_REGISTER_FREQ2, 0x22);
//        break;
//    }
//}
//
//void CSPITranscoderCC1101::SetChannelNumber(uint8_t channelNumber) {
//    WriteConfigReg(CONFIG_REGISTER_CHANNR, channelNumber);
//}
//
//void CSPITranscoderCC1101::SetIOConfig(const IO_CONFIG_DESC& desc) {
//    m_io_config = desc;
//
//    uint8_t iocfg0 = desc.gdo0_config | desc.gdo0_invert << 6 | desc.gdo0_enable_temp_sensor << 7;
//
//    uint8_t iocfg1 = desc.gdo1_config | desc.gdo1_invert << 6 | desc.gdo1_drive_strength << 7;
//
//    uint8_t iocfg2 = desc.gdo2_config | desc.gdo2_invert << 6;
//
//    WriteConfigReg(CONFIG_REGISTER_IOCFG0, iocfg0);
//    WriteConfigReg(CONFIG_REGISTER_IOCFG1, iocfg1);
//    WriteConfigReg(CONFIG_REGISTER_IOCFG2, iocfg2);
//}
//
//void CSPITranscoderCC1101::SetPacketControlConfig(const PACKET_CONTROL_CONFIG_DESC& desc) {
//    m_packet_control_config = desc;
//
//    uint8_t pktctrl0 = static_cast<uint8_t>(desc.packet_length_config) | desc.enable_crc << 2
//                       | static_cast<uint8_t>(desc.packet_format) << 4
//                       | desc.enable_whiten_data << 6;
//
//    uint8_t pktctrl1 = static_cast<uint8_t>(desc.address_check_config)
//                       | desc.enable_append_status_rssi_lqi_crc << 2
//                       | desc.enable_crc_autoflush << 3
//                       | (desc.preamble_quality_estimator_threshold / 4) << 5;
//
//    WriteConfigReg(CONFIG_REGISTER_PKTCTRL0, pktctrl0);
//    WriteConfigReg(CONFIG_REGISTER_PKTCTRL1, pktctrl1);
//}
//
//void CSPITranscoderCC1101::SetModemConfig(const MODEM_CONFIG_DESC& desc) {
//    m_modem_config = desc;
//
//    uint8_t mdmcfg0 = desc.channel_spacing_mantissa;
//
//    uint8_t mdmcfg1 = desc.channel_spacing_expontent
//                      | static_cast<uint8_t>(desc.num_preamble_config) << 4
//                      | desc.forward_error_correction << 7;
//
//    uint8_t mdmcfg2 = static_cast<uint8_t>(desc.sync_mode) | desc.manchester_encoding << 3
//                      | static_cast<uint8_t>(desc.modulation_format) << 4
//                      | desc.disable_dc_blocking_filter << 7;
//
//    uint8_t mdmcfg3 = desc.symbol_rate_mantissa;
//
//    uint8_t mdmcfg4 = desc.symbol_rate_exponent | desc.decimation_ratio << 4;
//
//    WriteConfigReg(CONFIG_REGISTER_MDMCFG0, mdmcfg0);
//    WriteConfigReg(CONFIG_REGISTER_MDMCFG1, mdmcfg1);
//    WriteConfigReg(CONFIG_REGISTER_MDMCFG2, mdmcfg2);
//    WriteConfigReg(CONFIG_REGISTER_MDMCFG3, mdmcfg3);
//    WriteConfigReg(CONFIG_REGISTER_MDMCFG4, mdmcfg4);
//}
//
//void CSPITranscoderCC1101::SetRadioStateMachineConfig(const RADIO_STATE_MACHINE_CONFIG_DESC& desc) {
//    m_radio_state_machine_config = desc;
//
//    uint8_t mcsm0 =
//        // TODO HACK: Is a ternary really necessary?
//        (desc.xosc_force_on ? 1 : 0) | (desc.enable_pin_radio_control ? 1 : 0) << 1
//        | static_cast<uint8_t>(desc.po_timeout) << 2
//        | static_cast<uint8_t>(desc.auto_calibrate_radio) << 4;
//
//    uint8_t mcsm1 = static_cast<uint8_t>(desc.on_tx_packet_state_change)
//                    | static_cast<uint8_t>(desc.on_rx_packet_state_change) << 2
//                    | static_cast<uint8_t>(desc.cca_mode) << 4;
//
//    uint8_t mcsm2 = desc.rx_time_sync_word_search | desc.rx_time_qual << 3 | desc.rx_time_rssi << 4;
//
//    WriteConfigReg(CONFIG_REGISTER_MCSM0, mcsm0);
//    WriteConfigReg(CONFIG_REGISTER_MCSM1, mcsm1);
//    WriteConfigReg(CONFIG_REGISTER_MCSM2, mcsm2);
//}
//
//void CSPITranscoderCC1101::SetWakeOnRadioConfig(const WAKE_ON_RADIO_CONFIG_DESC& desc) {
//    m_wake_on_radio_desc = desc;
//
//    uint8_t worctrl = static_cast<uint8_t>(desc.resolution) | desc.calibrate_rc_oscillator << 3
//                      | static_cast<uint8_t>(desc.event_1_timeout) << 4
//                      | desc.disable_rc_oscillator << 7;
//
//    uint8_t worevt0 = (desc.event_0_timeout & 0x00FF);
//
//    uint8_t worevt1 = (desc.event_0_timeout & 0xFF00) >> 8;
//
//    WriteConfigReg(CONFIG_REGISTER_WORCTRL, worctrl);
//    WriteConfigReg(CONFIG_REGISTER_WOREVT0, worevt0);
//    WriteConfigReg(CONFIG_REGISTER_WOREVT1, worevt1);
//}
//
//uint8_t CSPITranscoderCC1101::GetPartNumber() {
//    return ReadStatusReg(STATUS_REGISTER_PARTNUM);
//}
//
//uint8_t CSPITranscoderCC1101::GetVersionNumber() {
//    return ReadStatusReg(STATUS_REGISTER_VERSION);
//}
//
//uint8_t CSPITranscoderCC1101::GetStatusByte(DATA_DIRECTION dataDirection) {
//    SPIBeginTransmission();
//    uint8_t data =
//        SPITransfer(COMMAND_STROBE_SNOP | (dataDirection == DATA_DIRECTION::TX ? 0 : 1 << 7));
//    SPIEndTransmission();
//    return data;
//}
//
//void CSPITranscoderCC1101::GotoState(STATE state) {
//    uint8_t status;
//
//    switch (state) {
//    case STATE::IDLE:
//        status = WriteStrobe(COMMAND_STROBE_SIDLE);
//        break;
//    case STATE::RX:
//        status = WriteStrobe(COMMAND_STROBE_SRX);
//        break;
//    case STATE::TX:
//        // TODO IMPLEMENT: Handle CCA failure case
//        status = WriteStrobe(COMMAND_STROBE_STX);
//        break;
//    default:
//        // TODO IMPLEMENT: What to do here?
//        return;
//    }
//}
//
//void CSPITranscoderCC1101::WaitForState(STATE state) {
//    // TODO HACK: This is pretty dangerous I would think.  Easy to get into an infinite loop
//    uint16_t count = 0;
//
//    uint8_t status;
//    do {
//        status = GetStatusByte();
//
//        if (CompareState(status, STATE::RXFIFO_OVERFLOW)) {
//            uint8_t rxBytes = ReadStatusReg(STATUS_REGISTER_RXBYTES);
//            FlushRxFifo();
//            // TODO ERROR_HANDLING: Should probably find a way to alert the client that an overflow
//            // occurred
//        } else if (CompareState(status, STATE::RX)) {
//            uint8_t rxBytes = ReadStatusReg(STATUS_REGISTER_RXBYTES);
//            // sometimes we have more than 64 bytes in the RX fifo but the device doesn't report an
//            // overflow
//            if (rxBytes > 64) {
//                FlushRxFifo();
//                // TODO ERROR_HANDLING: Should probably find a way to alert the client that an
//                // overflow occurred
//            }
//        } else if (CompareState(status, STATE::TXFIFO_UNDERFLOW)) {
//            FlushTxFifo();
//            // TODO ERROR_HANDLING: Is TX succeptible to the same bad state as above?
//        }
//
//        // TODO IMPLEMENT: Timeout feature incase we get stuck here for yet unknown reasons
//        // TODO ERROR_HANDLING: Check for errors that prevent us from changing state
//    } while (!CompareState(status, state));
//}
//
//void CSPITranscoderCC1101::EnableRcOscillator(bool enable) {
//    m_wake_on_radio_desc.disable_rc_oscillator = !enable;
//    SetWakeOnRadioConfig(m_wake_on_radio_desc);
//    m_service_time->DelayMs(10); // TODO HACK to wait for RC oscillator calibration
//}
//
//void CSPITranscoderCC1101::BeginWakeOnRadio() {
//    WriteStrobe(COMMAND_STROBE_SWOR);
//}
//
//void CSPITranscoderCC1101::WriteTxPacket(
//    const uint8_t* const payloadBuffer,
//    uint8_t payloadLength) {
//    // TODO ERROR_HANDLING: Also take into account the extra bytes required for rssi, lqi, and crc
//
//    // TODO ERROR_HANDLING: Check if we even have enough room in the fifo to send stuff
//    // write the packet to the TX fifo
//    SPIBeginTransmission();
//
//    // write length byte to tx fifo (including extra byte if address check is enabled)
//    if (m_packet_control_config.packet_length_config == PACKET_LENGTH::VARIABLE) {
//        SPITransfer(FIFO_REGISTER_TX);
//        // TODO HACK: Hmm...kinda don't like this.  This assumes that a 'true' value produces a 1
//        SPITransfer(
//            payloadLength
//            + (static_cast<uint8_t>(m_packet_control_config.address_check_config) != 0));
//    }
//
//    if (static_cast<uint8_t>(m_packet_control_config.address_check_config)) {
//        SPITransfer(FIFO_REGISTER_TX);
//        SPITransfer(m_device_address);
//    }
//
//    // TODO IMPLEMENT: Packets larger than 64 bytes (limit of the FIFO).
//    // write the packet's payload
//    SPITransfer(FIFO_REGISTER_TX | static_cast<uint8_t>(TRANSACTION_MODE::WRITE_BURST));
//    for (int i = 0; i < payloadLength; i++) {
//        SPITransfer(payloadBuffer[i]);
//    }
//
//    SPIEndTransmission();
//}
//
//void CSPITranscoderCC1101::ReadRxPacket(uint8_t* rxBuffer, RX_PACKET_METADATA* metadata) {
//    uint8_t payloadLength = 0;
//
//    uint8_t rxBytes = ReadStatusReg(STATUS_REGISTER_RXBYTES);
//    // if bytes are available and no overflow has occurred
//    if (rxBytes & 0x7F && !(rxBytes & 0x80)) {
//        SPIBeginTransmission();
//        SPITransfer(FIFO_REGISTER_RX | static_cast<uint8_t>(TRANSACTION_MODE::READ_BURST));
//
//        // get packet length
//        {
//            // get the packet length
//            switch (m_packet_control_config.packet_length_config) {
//            case PACKET_LENGTH::VARIABLE:
//                payloadLength = SPITransfer(0); // first byte is the packet length
//                break;
//            case PACKET_LENGTH::FIXED:
//                // TODO BUG: What?  Why am I making the payload length equal to an enum underlying
//                // value?
//                payloadLength = static_cast<uint8_t>(m_packet_control_config.packet_length_config);
//                break;
//            default:
//                // TODO IMPLEMENT: Infinite packet length
//                break;
//            }
//
//            // TODO ERROR_HANDLING: Check for an incorrectly sized packet
//        }
//
//        // get address byte
//        {
//            if (static_cast<uint8_t>(m_packet_control_config.address_check_config)) {
//                metadata->address = SPITransfer(0);
//                payloadLength--; // address byte doesn't count as part of the payload length
//                                 // reported to the client
//            }
//        }
//
//        // get the actual payload
//        {
//            // TODO IMPLEMENT: Packets larger than 64 bytes (limit of the FIFO).
//            for (int i = 0; i < payloadLength; i++) {
//                rxBuffer[i] = SPITransfer(0);
//            }
//
//            // store final payload length
//            metadata->payload_length = payloadLength;
//
//            // get remaining packet metadata if it's expected to be there
//            if (m_packet_control_config.enable_append_status_rssi_lqi_crc) {
//                metadata->rssi   = SPITransfer(0);
//                uint8_t lqi_crc  = SPITransfer(0);
//                metadata->lqi    = lqi_crc & 0x7F;
//                metadata->crc_ok = (lqi_crc & 0x80) > 0;
//            }
//        }
//
//        SPIEndTransmission();
//    }
//}
//
//void CSPITranscoderCC1101::WaitForPacketRx() {
//    // TODO IMPLEMENT: Use GDOx when available
//    // TODO HACK: Is this dangerous?  Is there a possibility of this never to be satisfied?
//    switch (m_radio_state_machine_config.on_rx_packet_state_change) {
//    case ON_RX_PACKET_RECEIVED_STATE_CHANGE::IDLE:
//        WaitForState(STATE::IDLE);
//        break;
//    case ON_RX_PACKET_RECEIVED_STATE_CHANGE::FXTXON:
//        WaitForState(STATE::FSTXON);
//        break;
//    case ON_RX_PACKET_RECEIVED_STATE_CHANGE::TX:
//        WaitForState(STATE::TX);
//        break;
//    case ON_RX_PACKET_RECEIVED_STATE_CHANGE::STAY_RX:
//        // TODO IMPLEMENT: Can't wait for RX if already in RX
//        break;
//    default:
//        // TODO ERROR_HANDLING
//        break;
//    }
//
//    // TODO IMPLEMENT: GDO wake up path
//}
//
//void CSPITranscoderCC1101::WaitForPacketTx() {
//    // TODO HACK: Is this dangerous?  Is there a possibility of this never to be satisfied?
//    switch (m_radio_state_machine_config.on_tx_packet_state_change) {
//    case ON_TX_PACKET_SENT_STATE_CHANGE::IDLE:
//        WaitForState(STATE::IDLE);
//        break;
//    case ON_TX_PACKET_SENT_STATE_CHANGE::FXTXON:
//        WaitForState(STATE::FSTXON);
//        break;
//    case ON_TX_PACKET_SENT_STATE_CHANGE::STAY_TX:
//        // TODO IMPLEMENT: Can't wait for TX if already in TX
//        break;
//    case ON_TX_PACKET_SENT_STATE_CHANGE::RX:
//        WaitForState(STATE::RX);
//        break;
//    default:
//        // TODO ERROR_HANDLING
//        break;
//    }
//
//    // TODO IMPLEMENT: GDO wake up path
//}
//
//void CSPITranscoderCC1101::FlushRxFifo() {
//    WriteStrobe(COMMAND_STROBE_SFRX);
//}
//
//void CSPITranscoderCC1101::FlushTxFifo() {
//    WriteStrobe(COMMAND_STROBE_SFTX);
//}
//
//// ISPITranscoder
//
//void CSPITranscoderCC1101::AttachToBus(CSPIBus<uint8_t>* bus, uint8_t deviceId) {
//    m_device_id = deviceId;
//    m_spi_bus   = bus;
//}
//
///* PRIVATE */
//
//void CSPITranscoderCC1101::SPIBeginTransmission() {
//    m_spi_bus->BeginTransmission(m_device_id);
//    while (m_spi_bus->PollMiso())
//        ;
//}
//
//void CSPITranscoderCC1101::SPIEndTransmission() {
//    m_spi_bus->EndTransmission();
//}
//
//uint8_t CSPITranscoderCC1101::SPITransfer(uint8_t data) {
//    m_spi_bus->Write(data);
//    m_spi_bus->WaitForTransfer();
//    return m_spi_bus->Read();
//}
//
//uint8_t CSPITranscoderCC1101::ReadConfigReg(CONFIG_REGISTER reg) {
//    SPIBeginTransmission();
//    SPITransfer(reg | static_cast<uint8_t>(TRANSACTION_MODE::READ_SINGLE));
//    uint8_t data = SPITransfer(0);
//    SPIEndTransmission();
//    return data;
//}
//
//uint8_t CSPITranscoderCC1101::ReadStatusReg(STATUS_REGISTER reg) {
//    SPIBeginTransmission();
//    SPITransfer(
//        reg | static_cast<uint8_t>(
//                  TRANSACTION_MODE::READ_BURST)); // Status registers require burst bit to be set
//    uint8_t data = SPITransfer(0);
//    SPIEndTransmission();
//    return data;
//}
//
//uint8_t CSPITranscoderCC1101::WriteStrobe(COMMAND_STROBE strobe) {
//    SPIBeginTransmission();
//    uint8_t data = SPITransfer(strobe);
//
//    // need to wait for miso to go low to ensure proper reset when using SRES strobe
//    if (strobe == COMMAND_STROBE_SRES) {
//        while (m_spi_bus->PollMiso())
//            ;
//    }
//
//    SPIEndTransmission();
//
//    return data;
//}
//
//void CSPITranscoderCC1101::WriteConfigReg(CONFIG_REGISTER reg, uint8_t value) {
//    SPIBeginTransmission();
//    SPITransfer(reg);
//    SPITransfer(value);
//    SPIEndTransmission();
//}
//
//bool CSPITranscoderCC1101::CompareState(uint8_t statusByte, STATE state) {
//    return ((statusByte & 0x70) >> 4) == static_cast<uint8_t>(state);
//}