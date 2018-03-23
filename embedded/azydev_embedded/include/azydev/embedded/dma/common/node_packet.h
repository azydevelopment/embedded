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

#pragma once

#include <azydev/embedded/dma/common/node.h>

#include <stdint.h>

template<typename BEAT_PRIMITIVE>
class CDMANodePacket final : public CDMANode<BEAT_PRIMITIVE>
{
public:
    enum class PACKET_TYPE
    {
        WRITE,
        READ
    };

    struct CONFIG_DESC
    {
        PACKET_TYPE packet_type = PACKET_TYPE::WRITE;
    };

    struct DESC : CDMANode<BEAT_PRIMITIVE>::DESC
    {
        uint32_t num_beats_max = 0;
    };

    // constructor
    CDMANodePacket(const DESC&);

    // destructor
    virtual ~CDMANodePacket() override final;

    // NVI
    virtual void Reset(const CONFIG_DESC&) final;
    virtual PACKET_TYPE GetPacketType() const final;
    virtual uint32_t GetNumBeatsMax() const final;
    virtual void Write(const BEAT_PRIMITIVE) final;
    virtual void PrepareForRead(const uint32_t numBeats) final;

private:
    // rule of three
    CDMANodePacket(const CDMANodePacket&);
    CDMANodePacket& operator=(const CDMANodePacket&);

    // member variables
    PACKET_TYPE m_packet_type;
    uint32_t m_num_beats_max;
    uint32_t m_num_beats;
    BEAT_PRIMITIVE* m_data;

    // CDMANode
    virtual uint32_t GetAddress_impl() const override final;
};

///* FORWARD DECLARED TEMPLATES */
template class CDMANodePacket<uint8_t>;
template class CDMANodePacket<uint16_t>;