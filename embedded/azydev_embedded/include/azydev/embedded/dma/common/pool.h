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

#include <azydev/embedded/dma/common/entity.h>

#include <stdint.h>

template<typename BEAT_PRIMITIVE>
class CDMAPool final : public IDMAEntity
{
public:
    struct DESC
    {
        uint32_t num_beats_max      = 0;
        uint8_t num_allocations_max = 0;
    };

    // constructor
    CDMAPool(const DESC&);

    // destructor
    virtual ~CDMAPool() override final;

    // NVI
    virtual RESULT PushAllocation() final;
    virtual RESULT PopAllocation() final;
    virtual RESULT RecordWrite(const BEAT_PRIMITIVE) final;
    virtual RESULT RecordRead(const uint32_t count = 1) final;

private:
    enum class PACKET_TYPE
    {
        WRITE,
        READ,
        UNDEFINED = 255
    };

    struct ALLOCATION
    {
        uint32_t beat_index = 0;
        uint32_t num_beats  = 0;
    };

    // rule of three
    CDMAPool(const CDMAPool&);
    CDMAPool& operator=(const CDMAPool&);

    // member variables
    const uint32_t m_num_beats_max;
    uint32_t m_num_beats;
    BEAT_PRIMITIVE* m_data;
    uint32_t m_beat_tail_index;

    const uint8_t m_num_allocations_max;
    uint8_t m_num_allocations;
    ALLOCATION* m_allocations;
    ALLOCATION* m_allocation_active;
    uint8_t m_allocation_tail_index;

    // member functions
    bool IsBufferFull() const;
    ALLOCATION& GetAllocation(const uint8_t) const;
    ALLOCATION* const GetAllocationActive() const;
    RESULT RebaseAllocationIfNeeded(const uint32_t beatsToAdd);
};

/* FORWARD DECLARED TEMPLATES */
template class CDMAPool<uint8_t>;
template class CDMAPool<uint16_t>;