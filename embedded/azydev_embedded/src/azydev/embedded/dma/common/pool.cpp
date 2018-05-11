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

#include <azydev/embedded/dma/common/pool.h>

#include <cstring>

/* CDMAPool */

/* PUBLIC */

// constructor

template<typename BEAT_PRIMITIVE>
CDMAPool<BEAT_PRIMITIVE>::CDMAPool(const DESC& desc)
    : m_num_beats_max(desc.num_beats_max)
    , m_num_beats(0)
    , m_data(nullptr)
    , m_beat_tail_index(0)
    , m_num_allocations_max(desc.num_allocations_max)
    , m_num_allocations(0)
    , m_allocations(nullptr)
    , m_allocation_active(nullptr)
    , m_allocation_tail_index(0) {
    m_data        = new BEAT_PRIMITIVE[desc.num_beats_max];
    m_allocations = new Allocation*[desc.num_allocations_max];

    // actually create the allocation objects
    typename Allocation::DESC allocationDesc = {};
    allocationDesc.is_incrementing           = true;
    for (uint8_t i = 0; i < desc.num_allocations_max; i++) {
        m_allocations[i] = new Allocation(allocationDesc);
    }
}

// destructor

template<typename BEAT_PRIMITIVE>
CDMAPool<BEAT_PRIMITIVE>::~CDMAPool() {
    for (uint8_t i = 0; i < m_num_allocations_max; i++) {
        delete m_allocations[i];
    }

    delete[] m_allocations;
    delete[] m_data;
}

// NVI

template<typename BEAT_PRIMITIVE>
IDMAEntity::RESULT CDMAPool<BEAT_PRIMITIVE>::PushAllocation() {
    RESULT result = RESULT::UNDEFINED;

    // check if we have space for another allocation
    if (IsPoolFull()) {
        result = RESULT::FAIL_ERROR;
    } else {
        // populate new allocation
        uint8_t allocationNewIndex =
            (m_allocation_tail_index + m_num_allocations) % m_num_allocations_max;
        Allocation& allocationNew = GetAllocation(allocationNewIndex);
        {
            // if this is the first allocation
            if (m_num_allocations == 0) {
                allocationNew.m_beat_base_address = &m_data[m_beat_tail_index];
            } else {
                // otherwise, continue from the end of the last allocation
                Allocation& allocationActive = *GetAllocationActive();
                allocationNew.m_beat_base_address =
                    allocationActive.m_beat_base_address + allocationActive.m_num_beats;
            }

            allocationNew.m_num_beats = 0;
        }

        // update tracking state
        m_num_allocations++;
        m_allocation_active = &allocationNew;

        result = RESULT::SUCCESS;
    }

    return result;
}

template<typename BEAT_PRIMITIVE>
IDMAEntity::RESULT CDMAPool<BEAT_PRIMITIVE>::PopAllocation() {
    RESULT result = RESULT::UNDEFINED;

    // check if we've already popped all allocations
    if (m_num_allocations == 0) {
        result = RESULT::FAIL_ERROR;
    } else {
        Allocation& allocationTail = GetAllocation(m_allocation_tail_index);

        // mark beats of the tail allocation as free
        m_num_beats -= allocationTail.m_num_beats;

        // remove an allocation
        m_num_allocations--;

        // update some state depending on if there are any remaining allocations
        if (m_num_allocations > 0) {
            m_allocation_tail_index = (m_allocation_tail_index + 1) % m_num_allocations_max;

            Allocation& allocationTailNew = GetAllocation(m_allocation_tail_index);
            m_beat_tail_index             = allocationTailNew.m_beat_base_address - m_data;
        }

        result = RESULT::SUCCESS;
    }

    return result;
}

template<typename BEAT_PRIMITIVE>
IDMAEntity::RESULT CDMAPool<BEAT_PRIMITIVE>::RecordWrite(const BEAT_PRIMITIVE data) {
    RESULT result = RESULT::UNDEFINED;

    if (IsPoolFull()) {
        result = RESULT::FAIL_ERROR;
    } else {
        // if we're about to rollover, try to rebase to the start of the data buffer
        result = RebaseAllocationIfNeeded(1);

        if (result == RESULT::SUCCESS) {
            // get currently active allocation
            Allocation& allocationActive = *GetAllocationActive();

            // write the beat to the data buffer
            *(allocationActive.m_beat_base_address + allocationActive.m_num_beats) = data;

            // track the new beat
            allocationActive.m_num_beats++;
            m_num_beats++;
        }
    }

    return result;
}

template<typename BEAT_PRIMITIVE>
IDMAEntity::RESULT CDMAPool<BEAT_PRIMITIVE>::RecordRead(const uint32_t numBeats) {
    RESULT result = IDMAEntity::RESULT::UNDEFINED;

    if (IsPoolFull()) {
        result = RESULT::FAIL_ERROR;
    } else {
        // if we're about to rollover, try to rebase to the start of the data buffer
        result = RebaseAllocationIfNeeded(numBeats);

        if (result == RESULT::SUCCESS) {
            // get currently active allocation
            Allocation& allocationActive = *GetAllocationActive();

            // track new beat
            allocationActive.m_num_beats += numBeats;
            m_num_beats++;
        }
    }
    return result;
}

/* PRIVATE */

// member functions

template<typename BEAT_PRIMITIVE>
bool CDMAPool<BEAT_PRIMITIVE>::IsPoolFull() const {
    bool isAllocationAvailable = !(m_num_allocations == m_num_allocations_max);

    uint32_t beatHeadIndex =
        ((m_allocation_active->m_beat_base_address + m_allocation_active->m_num_beats) - m_data)
        % m_num_beats_max;

    bool isBeatAvailable = (m_num_beats == 0) || (beatHeadIndex != m_beat_tail_index);

    if (isAllocationAvailable && isBeatAvailable) {
        return false;
    } else {
        return true;
    }
}

template<typename BEAT_PRIMITIVE>
typename CDMAPool<BEAT_PRIMITIVE>::Allocation&
CDMAPool<BEAT_PRIMITIVE>::GetAllocation(const uint8_t index) const {
    return *m_allocations[index];
}

template<typename BEAT_PRIMITIVE>
typename CDMAPool<BEAT_PRIMITIVE>::Allocation* const
CDMAPool<BEAT_PRIMITIVE>::GetAllocationActive() const {
    return m_allocation_active;
}

template<typename BEAT_PRIMITIVE>
IDMAEntity::RESULT CDMAPool<BEAT_PRIMITIVE>::RebaseAllocationIfNeeded(const uint32_t beatsToAdd) {
    RESULT result = RESULT::UNDEFINED;

    // check if there's enough space at the beginning of the data buffer
    Allocation* allocationActive = GetAllocationActive();

    BEAT_PRIMITIVE* allocationNextBeat =
        allocationActive->m_beat_base_address + allocationActive->m_num_beats;
    BEAT_PRIMITIVE* poolBeatEnd = m_data + m_num_beats_max;

    // if at the end of the buffer
    if (allocationNextBeat >= poolBeatEnd) {
        // if we can fit this allocation at the start of this buffer
        if (allocationActive->m_num_beats + beatsToAdd <= m_beat_tail_index) {
            // copy over this allocation's data to the start of the data buffer
            memcpy(
                &m_data[0], allocationActive->m_beat_base_address, allocationActive->m_num_beats);

            // update the allocation's metadata
            allocationActive->m_beat_base_address = m_data;

            result = RESULT::SUCCESS;
        } else {
            // otherwise fail to say we've just run out of space
            result = RESULT::FAIL_ERROR;
        }
    } else {
        // if we're not at the end of the buffer, don't do anything
        result = RESULT::SUCCESS;
    }

    return result;
}

/* CDMAPool::View */

/* PUBLIC */

// constructor

template<typename BEAT_PRIMITIVE>
CDMAPool<BEAT_PRIMITIVE>::Allocation::Allocation(const DESC& desc)
    : CDMANode<BEAT_PRIMITIVE>(desc)
    , m_beat_base_address(nullptr)
    , m_num_beats(0) {
}

// destructor

template<typename BEAT_PRIMITIVE>
CDMAPool<BEAT_PRIMITIVE>::Allocation::~Allocation() {
}

/* PRIVATE */

template<typename BEAT_PRIMITIVE>
uint32_t CDMAPool<BEAT_PRIMITIVE>::Allocation::GetBaseAddress_impl() const {
    return reinterpret_cast<uint32_t>(m_beat_base_address);
}

template<typename BEAT_PRIMITIVE>
IDMAEntity::RESULT CDMAPool<BEAT_PRIMITIVE>::Allocation::Reset_impl() {
}