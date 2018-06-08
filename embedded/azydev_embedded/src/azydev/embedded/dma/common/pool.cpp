/* The MIT License (MIT)
, m_num _beats(0), m_num_beats(0)azy.development@gmail.com>
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
    , m_beat_tail(nullptr)
    , m_num_allocations_max(desc.num_allocations_max)
    , m_num_allocations(0)
    , m_allocations(nullptr)
    , m_allocation_active_index(0)
    , m_allocation_tail(nullptr) {
    m_data      = new BEAT_PRIMITIVE[desc.num_beats_max];
    m_beat_tail = m_data;

    m_allocations = new Allocation*[desc.num_allocations_max];

    // actually create the allocation objects
    typename Allocation::DESC allocationDesc = {};
    for (uint8_t i = 0; i < desc.num_allocations_max; i++) {
        m_allocations[i] = new Allocation(allocationDesc);
    }

    m_allocation_tail = m_allocations;
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
IDMAEntity::RESULT
CDMAPool<BEAT_PRIMITIVE>::PushAllocation(IDMANode<BEAT_PRIMITIVE>** const outNode) {
    RESULT result = RESULT::UNDEFINED;

    // check if we have space for another allocation and more beat space
    if (IsAllocationAvailable() && IsBeatsAvailable(1)) {
        // populate new allocation
        uint8_t allocationNewIndex = (m_allocation_active_index + 1) % m_num_allocations_max;
        Allocation& allocationNew  = *m_allocations[allocationNewIndex];

        // if this is the first allocation
        if (m_num_allocations == 0) {
            allocationNew.m_beat_base_address = m_beat_tail;
            m_allocation_tail                 = &m_allocations[allocationNewIndex];
        } else {
            // otherwise, continue from the end of the last allocation
            Allocation& allocationActive = *m_allocations[m_allocation_active_index];
            allocationNew.m_beat_base_address =
                allocationActive.m_beat_base_address + allocationActive.m_num_beats;
        }

        allocationNew.m_num_beats         = 0;
        allocationNew.m_num_beats_padding = 0;

        // update tracking state
        m_num_allocations++;
        m_allocation_active_index = allocationNewIndex;

        *outNode = &allocationNew;

        result = RESULT::SUCCESS;
    } else {
        *outNode = nullptr;
        result   = RESULT::FAIL_ERROR;
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
        // remove an allocation
        m_num_allocations--;

        // return beats to the pool
        m_num_beats -=
            ((*m_allocation_tail)->m_num_beats + (*m_allocation_tail)->m_num_beats_padding);

        // update some state depending on if there are any remaining allocations
        if (m_num_allocations > 0) {
            uint8_t allocationNewTailIndex =
                ((m_allocation_tail - m_allocations) + 1) % m_num_allocations_max;

            m_allocation_tail = &m_allocations[allocationNewTailIndex];
            m_beat_tail       = (*m_allocation_tail)->m_beat_base_address;
        }

        result = RESULT::SUCCESS;
    }

    return result;
}

template<typename BEAT_PRIMITIVE>
IDMAEntity::RESULT CDMAPool<BEAT_PRIMITIVE>::RecordWrite(const BEAT_PRIMITIVE data) {
    RESULT result = RESULT::UNDEFINED;

    if (IsBeatsAvailable(1)) {
        // if we're about to rollover, try to rebase to the start of the data buffer
        result = RebaseAllocationIfNeeded(1, true);

        if (result == RESULT::SUCCESS) {
            // get currently active allocation
            Allocation& allocationActive = *m_allocations[m_allocation_active_index];

            // write the beat to the data buffer
            *(allocationActive.m_beat_base_address + allocationActive.m_num_beats) = data;

            // track the new beat
            allocationActive.m_num_beats++;
            m_num_beats++;
        }
    } else {
        result = RESULT::FAIL_ERROR;
    }

    return result;
}

template<typename BEAT_PRIMITIVE>
IDMAEntity::RESULT CDMAPool<BEAT_PRIMITIVE>::RecordRead(const uint32_t numBeats) {
    RESULT result = RESULT::SUCCESS;

    // if (IsBeatsAvailable(numBeats)) {
    //// if we're about to rollover, try to rebase to the start of the data buffer
    // result = RebaseAllocationIfNeeded(numBeats, false);
    //
    // if (result == RESULT::SUCCESS) {
    //// get currently active allocation
    // Allocation& allocationActive = *(m_allocations[m_allocation_active_index]);
    //
    //// zero out data
    // memset(
    //(allocationActive.m_beat_base_address + allocationActive.m_num_beats), 0, numBeats);
    //
    //// track new beats
    // allocationActive.m_num_beats += numBeats;
    // m_num_beats += numBeats;
    //}
    //} else {
    // result = RESULT::FAIL_ERROR;
    //}

    return result;
}

/* PRIVATE */

// member functions

template<typename BEAT_PRIMITIVE>
bool CDMAPool<BEAT_PRIMITIVE>::IsAllocationAvailable() const {
    return !(m_num_allocations == m_num_allocations_max);
}

template<typename BEAT_PRIMITIVE>
bool CDMAPool<BEAT_PRIMITIVE>::IsBeatsAvailable(const uint32_t numBeats) const {
    return (m_num_beats_max - m_num_beats) >= numBeats;
}

template<typename BEAT_PRIMITIVE>
bool CDMAPool<BEAT_PRIMITIVE>::WouldNeedRebasing(const uint32_t numBeats) const {
    return true;
}

template<typename BEAT_PRIMITIVE>
IDMAEntity::RESULT
CDMAPool<BEAT_PRIMITIVE>::RebaseAllocationIfNeeded(const uint32_t beatsToAdd, const bool copyData) {
    RESULT result = RESULT::UNDEFINED;

    // check if there's enough space at the beginning of the data buffer
    Allocation& allocationActive = *m_allocations[m_allocation_active_index];

    BEAT_PRIMITIVE* allocationNewLastBeat =
        allocationActive.m_beat_base_address + allocationActive.m_num_beats + beatsToAdd - 1;
    BEAT_PRIMITIVE* poolBeatEnd = m_data + m_num_beats_max;

    // if won't fit at the end of the buffer
    if (allocationNewLastBeat >= poolBeatEnd) {
        // keep track of how many beats will be padding; also used for rebase copying
        uint32_t numBeatsPadding = poolBeatEnd - allocationActive.m_beat_base_address;

        // if we can fit this allocation at the start of this buffer
        if (allocationActive.m_num_beats + beatsToAdd <= (m_beat_tail - m_data)) {
            // copy over this allocation's data to the start of the data buffer
            if (copyData) {
                memcpy(&m_data[0], allocationActive.m_beat_base_address, numBeatsPadding);
            }

            // update the allocation's metadata
            allocationActive.m_beat_base_address = m_data;
            allocationActive.m_num_beats_padding = numBeatsPadding;

            // account for padding in the total number of beats counted
            m_num_beats += numBeatsPadding;

            result = RESULT::SUCCESS;
        } else {
            // otherwise fail to say we've just run out of space
            result = RESULT::FAIL_ERROR;
        }
    } else {
        // if we're not at the end of the buffer, don't need to do anything
        result = RESULT::SUCCESS;
    }

    return result;
}

/* CDMAPool::Allocation */

/* PUBLIC */

// constructor

template<typename BEAT_PRIMITIVE>
CDMAPool<BEAT_PRIMITIVE>::Allocation::Allocation(const DESC& desc)
    : IDMANode<BEAT_PRIMITIVE>(desc)
    , m_beat_base_address(nullptr)
    , m_num_beats(0)
    , m_num_beats_padding(0) {
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
uint32_t CDMAPool<BEAT_PRIMITIVE>::Allocation::GetNumBeats_impl() const {
    return m_num_beats;
}

template<typename BEAT_PRIMITIVE>
bool CDMAPool<BEAT_PRIMITIVE>::Allocation::IsIncrementing_impl() const {
    return m_num_beats > 1;
}

template<typename BEAT_PRIMITIVE>
IDMAEntity::RESULT CDMAPool<BEAT_PRIMITIVE>::Allocation::Reset_impl() {
}