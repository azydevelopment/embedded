///*
//* Copyright (C) Andrew Yeung - All Rights Reserved
//* Unauthorized copying of this file, via any medium is strictly prohibited
//* Proprietary and confidential
//* Written by Andrew Yeung <azy.development@gmail.com>, May 2017
//*/
//
//#include <azydev/embedded/bus/spi/microchip/mcp2030/transcoder.h>
//
//#include <azydev/embedded/pins/common/pins.h>
//#include <azydev/embedded/util/binary.h>
//
///* PUBLIC */
//
//CSPITranscoderMCP2030::CSPITranscoderMCP2030(
//    const PIN_CONFIG_DESC& pinConfig,
//    CPins<uint8_t>* const servicePins)
//    : m_service_pins(servicePins)
//    , m_device_id(0)
//    , m_spi_bus(nullptr)
//    , m_pin_config_desc(pinConfig)
//    , m_config_desc({}) {
//}
//
//CSPITranscoderMCP2030::~CSPITranscoderMCP2030() {
//}
//
//void CSPITranscoderMCP2030::Reset() {
//    WriteCmd(CMCP2030Packet::COMMAND::SOFT_RESET);
//}
//
//void CSPITranscoderMCP2030::SetConfig(const CONFIG_DESC& configDesc) {
//    m_config_desc = configDesc;
//
//    // TODO HACK: Hardcoded register values and offsets
//    uint8_t configRegisters[7];
//
//    // TODO HACK: First one needs to be static_cast to avoid warning about unsafe bool/int mixing
//    configRegisters[0] =
//        static_cast<uint8_t>(configDesc.lc_enable_config_desc.disable_lcx)
//        | configDesc.lc_enable_config_desc.disable_lcy << 1
//        | configDesc.lc_enable_config_desc.disable_lcz << 2
//        | static_cast<uint8_t>(configDesc.alert_trigger) << 3
//        | static_cast<uint8_t>(configDesc.output_enable_filter_config_desc.time_low) << 4
//        | static_cast<uint8_t>(configDesc.output_enable_filter_config_desc.time_high) << 6;
//
//    configRegisters[1] = configDesc.lc_tuning_config_desc.lcx_tuning_capacitance
//                         | static_cast<uint8_t>(configDesc.lfdata_output) << 6;
//
//    configRegisters[2] = configDesc.lc_tuning_config_desc.lcy_tuning_capacitance
//                         | static_cast<uint8_t>(configDesc.clock_divide) << 6
//                         | configDesc.enable_rssi_fet << 7;
//
//    configRegisters[3] = configDesc.lc_tuning_config_desc.lcz_tuning_capacitance;
//
//    configRegisters[4] =
//        static_cast<uint8_t>(
//            configDesc.lc_sensitivity_reduction_config_desc.lcy_sensitivity_reduction)
//        | static_cast<uint8_t>(
//              configDesc.lc_sensitivity_reduction_config_desc.lcx_sensitivity_reduction)
//              << 4;
//
//    configRegisters[5] =
//        static_cast<uint8_t>(
//            configDesc.lc_sensitivity_reduction_config_desc.lcz_sensitivity_reduction)
//        | static_cast<uint8_t>(configDesc.min_modulation_depth) << 4 | configDesc.enable_agc << 6
//        | configDesc.auto_channel_select << 7;
//
//    // calculate column parity
//    uint8_t columnParityEven = 0;
//    for (uint8_t i = 0; i < 6; i++) {
//        columnParityEven ^= configRegisters[i];
//    }
//
//    // invert since parity bits need to be odd
//    configRegisters[6] = ~columnParityEven;
//
//    // TODO HACK: Hardcoded value
//    // TODO HACK: Why do I need to do this backwards?  It seems like it wants me to set the column
//    // parity bits first.
//    for (int8_t i = 6; i >= 0; i--) {
//        WriteReg(i, configRegisters[i]);
//    }
//}
//
//CSPITranscoderMCP2030::STATUS CSPITranscoderMCP2030::GetStatus() {
//    CMCP2030Packet packet;
//
//    // TODO HACK: Waste of program memory. Basically the same as ReadReg() except I need to populate
//    // an MCP2030_STATUS struct
//
//    // TODO HACK: Hardcoded value
//    packet.InitRegRead(CMCP2030Packet::COMMAND::READ_REG, 7);
//
//    // TODO IMPLEMENT: Drive !ALERT output low (MCP2030 figure 5-10)
//
//    // send read command and reg address
//    {
//        SPIBeginTransmission();
//        SPISetDuplexMode(CSPIBus<uint8_t>::DUPLEX_MODE::HALF_WRITE);
//
//        uint8_t byteUpper = SPITransfer(packet.GetByteUpper());
//        uint8_t byteLower = SPITransfer(packet.GetByteLower());
//
//        SPISetDuplexMode(CSPIBus<uint8_t>::DUPLEX_MODE::HALF_READ);
//        SPIEndTransmission();
//    }
//
//    // receive the register read result
//    {
//        SPIBeginTransmission();
//        uint8_t byteUpper = SPITransfer(0);
//        uint8_t byteLower = SPITransfer(0);
//        SPIEndTransmission();
//
//        // reuse the packet sent in to carry the results back
//        packet.InitRaw(byteUpper, byteLower);
//    }
//
//    // TODO IMPLEMENT: Set !ALERT back to input
//
//    uint16_t payload = packet.GetPayload();
//
//    // populate struct to be sent back
//    // TODO HACK: Need the != 0 to avoid warning about implicit int to bool cast
//    STATUS status                = {};
//    status.parity_error          = Binary::BC<uint16_t>(payload, 0) != 0;
//    status.alarm_timer_timed_out = Binary::BC<uint16_t>(payload, 1) != 0;
//    status.woken_up_channel_x    = Binary::BC<uint16_t>(payload, 2) != 0;
//    status.woken_up_channel_y    = Binary::BC<uint16_t>(payload, 3) != 0;
//    status.woken_up_channel_z    = Binary::BC<uint16_t>(payload, 4) != 0;
//    status.agc_active            = Binary::BC<uint16_t>(payload, 5) != 0;
//    status.channel_active_x      = Binary::BC<uint16_t>(payload, 6) != 0;
//    status.channel_active_y      = Binary::BC<uint16_t>(payload, 7) != 0;
//    status.channel_active_z      = Binary::BC<uint16_t>(payload, 8) != 0;
//
//    return status;
//}
//
//uint8_t CSPITranscoderMCP2030::ReadReg(uint8_t regAddress) {
//    CMCP2030Packet packet;
//    packet.InitRegRead(CMCP2030Packet::COMMAND::READ_REG, regAddress);
//
//    // TODO IMPLEMENT: Drive !ALERT output low (MCP2030 figure 5-10)
//
//    // send read command and reg address
//    {
//        SPIBeginTransmission();
//        SPISetDuplexMode(CSPIBus<uint8_t>::DUPLEX_MODE::HALF_WRITE);
//
//        uint8_t byteUpper = SPITransfer(packet.GetByteUpper());
//        uint8_t byteLower = SPITransfer(packet.GetByteLower());
//
//        SPISetDuplexMode(CSPIBus<uint8_t>::DUPLEX_MODE::HALF_READ);
//        SPIEndTransmission();
//    }
//
//    // receive the register read result
//    {
//        SPIBeginTransmission();
//        uint8_t byteUpper = SPITransfer(0);
//        uint8_t byteLower = SPITransfer(0);
//        SPIEndTransmission();
//
//        // reuse the packet sent in to carry the results back
//        packet.InitRaw(byteUpper, byteLower);
//    }
//
//    // TODO IMPLEMENT: Set !ALERT back to input
//
//    return packet.GetData();
//}
//
//void CSPITranscoderMCP2030::WriteReg(uint8_t regAddress, uint8_t data) {
//    CMCP2030Packet packet;
//    packet.InitRegWrite(CMCP2030Packet::COMMAND::WRITE_REG, regAddress, data);
//
//    // TODO IMPLEMENT: Drive !ALERT output low (MCP2030 figure 5-10)
//    SPIBeginTransmission();
//    SPISetDuplexMode(CSPIBus<uint8_t>::DUPLEX_MODE::HALF_WRITE);
//    SPITransfer(packet.GetByteUpper());
//    SPITransfer(packet.GetByteLower());
//    SPISetDuplexMode(CSPIBus<uint8_t>::DUPLEX_MODE::HALF_READ);
//    SPIEndTransmission();
//
//    // TODO IMPLEMENT: Set !ALERT back to input
//}
//
//// ISPITranscoder
//
//void CSPITranscoderMCP2030::AttachToBus(CSPIBus<uint8_t>* bus, uint8_t deviceId) {
//    m_device_id = deviceId;
//    m_spi_bus   = bus;
//}
//
///* PRIVATE */
//
//void CSPITranscoderMCP2030::SPISetDuplexMode(CSPIBus<uint8_t>::DUPLEX_MODE duplexMode) {
//    m_spi_bus->SetDuplexMode(duplexMode);
//}
//
//void CSPITranscoderMCP2030::SPIBeginTransmission() {
//    m_service_pins->DisableInterrupt(m_pin_config_desc.alert);
//    m_spi_bus->BeginTransmission(m_device_id);
//}
//
//void CSPITranscoderMCP2030::SPIEndTransmission() {
//    m_spi_bus->EndTransmission();
//    m_service_pins->EnableInterrupt(
//        m_pin_config_desc.alert, CPins<uint8_t>::INTERRUPT_TRIGGER::TRIGGER_FALLING, true);
//}
//
//uint8_t CSPITranscoderMCP2030::SPITransfer(uint8_t data) {
//    m_spi_bus->Write(data);
//    m_spi_bus->WaitForTransfer();
//    return m_spi_bus->Read();
//}
//
//void CSPITranscoderMCP2030::WriteCmd(CMCP2030Packet::COMMAND command) {
//    CMCP2030Packet packet;
//
//    // TODO ERROR_HANDLING: Only allow standalone commands.  No register read/write commands allowed
//    packet.InitCmd(command);
//
//    SPIBeginTransmission();
//    SPITransfer(packet.GetByteUpper());
//    SPITransfer(packet.GetByteLower());
//    SPIEndTransmission();
//}
