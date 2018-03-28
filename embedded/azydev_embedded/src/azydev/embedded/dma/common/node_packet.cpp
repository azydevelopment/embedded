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

template<typename BEAT_PRIMITIVE>
CDMANodePacket<BEAT_PRIMITIVE>::CDMANodePacket(const DESC& desc)
    : CDMANode<BEAT_PRIMITIVE>(desc)
    , m_packet_type(PACKET_TYPE::UNDEFINED)
    , m_num_beats_max(desc.num_beats_max)
    , m_num_beats(0)
    , m_data(nullptr) {
    m_data = new BEAT_PRIMITIVE[m_num_beats_max];

    this->Reset();
}

// destructor

template<typename BEAT_PRIMITIVE>
CDMANodePacket<BEAT_PRIMITIVE>::~CDMANodePacket() {
    delete[] m_data;
}

/* PRIVATE */

// member functions

template<typename BEAT_PRIMITIVE>
typename CDMANodePacket<BEAT_PRIMITIVE>::PACKET_TYPE
CDMANodePacket<BEAT_PRIMITIVE>::GetPacketType() const {
    return m_packet_type;
}

template<typename BEAT_PRIMITIVE>
uint32_t CDMANodePacket<BEAT_PRIMITIVE>::GetNumBeatsMax() const {
    return m_num_beats_max;
}

// CDMANode

template<typename BEAT_PRIMITIVE>
uint32_t CDMANodePacket<BEAT_PRIMITIVE>::GetAddress_impl() const {
    return reinterpret_cast<uint32_t>(m_data);
}

template<typename BEAT_PRIMITIVE>
IDMAEntity::RESULT CDMANodePacket<BEAT_PRIMITIVE>::Reset_impl() {
    m_packet_type = PACKET_TYPE::UNDEFINED;
    m_num_beats   = 0;

    return IDMAEntity::RESULT::SUCCESS;
}

template<typename BEAT_PRIMITIVE>
IDMAEntity::RESULT CDMANodePacket<BEAT_PRIMITIVE>::RecordWrite_impl(const BEAT_PRIMITIVE data) {
    IDMAEntity::RESULT result = IDMAEntity::RESULT::UNDEFINED;
    if (GetPacketType() == PACKET_TYPE::WRITE && m_num_beats < GetNumBeatsMax()) {
        m_data[m_num_beats] = data;
        m_num_beats++;
        result = IDMAEntity::RESULT::SUCCESS;
    } else {
        result = IDMAEntity::RESULT::FAIL_ERROR;
    }

    return result;
}

template<typename BEAT_PRIMITIVE>
IDMAEntity::RESULT CDMANodePacket<BEAT_PRIMITIVE>::RecordRead_impl(const uint32_t numBeats) {
    IDMAEntity::RESULT result = IDMAEntity::RESULT::UNDEFINED;
    if (GetPacketType() == PACKET_TYPE::READ && (m_num_beats + numBeats) < GetNumBeatsMax()) {
        m_num_beats += numBeats;
        result = IDMAEntity::RESULT::SUCCESS;
    } else {
        result = IDMAEntity::RESULT::FAIL_ERROR;
    }

    return result;
}