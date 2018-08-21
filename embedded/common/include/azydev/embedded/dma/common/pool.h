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
#include <azydev/embedded/dma/common/node.h>

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
    virtual bool IsAllocationAvailable() const final;
    virtual const IDMANode<BEAT_PRIMITIVE>* const
    GetAllocationDMANode(uint8_t const allocationId) const final;
    virtual RESULT PushAllocation(uint8_t& allocationId) final;
    virtual RESULT PopAllocation() final;
    virtual RESULT RecordWrite(const BEAT_PRIMITIVE) final;
    virtual RESULT RecordRead(const uint32_t numBeats = 1) final;

private:
    class Allocation final : public IDMANode<BEAT_PRIMITIVE>
    {
    public:
        struct DESC : IDMANode<BEAT_PRIMITIVE>::DESC
        {};

        // constructor
        Allocation(const DESC&);

        // destructor
        virtual ~Allocation() override final;

        // member variables
        BEAT_PRIMITIVE* m_beat_base_address;
        uint32_t m_num_beats;
        uint32_t m_num_beats_padding;

    private:
        // rule of three
        Allocation(const Allocation&);
        Allocation& operator=(const Allocation&);

        // IDMANode
        virtual uintptr_t GetBaseAddress_impl() const override final;
        virtual uint32_t GetNumBeats_impl() const override final;
        virtual bool IsIncrementing_impl() const override final;
        virtual RESULT Reset_impl() override final;
    };

    // rule of three
    CDMAPool(const CDMAPool&);
    CDMAPool& operator=(const CDMAPool&);

    // member variables
    const uint32_t m_num_beats_max;
    uint32_t m_num_beats;
    BEAT_PRIMITIVE* m_data;
    BEAT_PRIMITIVE* m_beat_tail;

    const uint8_t m_num_allocations_max;
    uint8_t m_num_allocations;
    Allocation** m_allocations;
    uint8_t m_allocation_active_index;
    Allocation** m_allocation_tail;

    // member functions
    bool IsBeatsAvailable(const uint32_t numBeats = 1) const;
    bool WouldNeedRebasing(const uint32_t numBeats) const;
    RESULT RebaseAllocationIfNeeded(const uint32_t beatsToAdd, const bool copyData);
};

/* FORWARD DECLARED TEMPLATES */
template class CDMAPool<uint8_t>;
template class CDMAPool<uint16_t>;