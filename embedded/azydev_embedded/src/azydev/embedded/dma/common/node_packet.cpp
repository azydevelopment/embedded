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

#include <azydev/embedded/dma/common/node_packet.h>

/* PUBLIC */

// constructor

CDMANodePacket::CDMANodePacket(const DESC& desc)
    : IDMANode(desc)
    , m_packet_type(PACKET_TYPE::WRITE)
    , m_num_beats_max(desc.num_beats_max)
    , m_num_beats(0)
    , m_data({}) {
    switch (GetPrimitiveType()) {
    case BEAT_PRIMITIVE::UINT8_T:
        m_data.data_8bit = new uint8_t[m_num_beats_max];
        break;
    case BEAT_PRIMITIVE::UINT16_T:
        m_data.data_16bit = new uint16_t[m_num_beats_max];
        break;
    case BEAT_PRIMITIVE::UINT32_T:
        m_data.data_32bit = new uint32_t[m_num_beats_max];
        break;
    }
}

// destructor

CDMANodePacket::~CDMANodePacket() {
    switch (GetPrimitiveType()) {
    case BEAT_PRIMITIVE::UINT8_T:
        delete[] m_data.data_8bit;
        break;
    case BEAT_PRIMITIVE::UINT16_T:
        delete[] m_data.data_16bit;
        break;
    case BEAT_PRIMITIVE::UINT32_T:
        delete[] m_data.data_32bit;
        break;
    }
}

// NVI

void CDMANodePacket::Reset(const CONFIG_DESC& config) {
    m_packet_type = config.packet_type;
    m_num_beats   = 0;
}

CDMANodePacket::PACKET_TYPE CDMANodePacket::GetPacketType() const {
    return m_packet_type;
}

uint32_t CDMANodePacket::GetNumBeatsMax() const {
    return m_num_beats_max;
}

void CDMANodePacket::Write(const uint8_t data) {
    if (GetPacketType() == PACKET_TYPE::WRITE && m_num_beats < GetNumBeatsMax()) {
        m_data.data_8bit[m_num_beats] = data;
        m_num_beats++;
    }
}

void CDMANodePacket::Write(const uint16_t data) {
    if (GetPacketType() == PACKET_TYPE::WRITE && m_num_beats < GetNumBeatsMax()) {
        m_data.data_16bit[m_num_beats] = data;
        m_num_beats++;
    }
}

void CDMANodePacket::Write(const uint32_t data) {
    if (GetPacketType() == PACKET_TYPE::WRITE && m_num_beats < GetNumBeatsMax()) {
        m_data.data_32bit[m_num_beats] = data;
        m_num_beats++;
    }
}

void CDMANodePacket::PrepareForRead(const uint32_t numBeats) {
    if (GetPacketType() == PACKET_TYPE::READ) {
        m_num_beats = numBeats;
    }
}

/* PRIVATE */

// IDMANode

uint32_t CDMANodePacket::GetAddress_impl() const {
    uint32_t address = 0;
    switch (GetPrimitiveType()) {
    case BEAT_PRIMITIVE::UINT8_T:
        address = reinterpret_cast<uint32_t>(m_data.data_8bit);
        break;
    case BEAT_PRIMITIVE::UINT16_T:
        address = reinterpret_cast<uint32_t>(m_data.data_16bit);
        break;
    case BEAT_PRIMITIVE::UINT32_T:
        address = reinterpret_cast<uint32_t>(m_data.data_32bit);
        break;
    }
    return address;
}