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
		
	m_data = new BEAT_PRIMITIVE[desc.num_beats_max];
	m_allocations = new ALLOCATION[desc.num_allocations_max];
}

// destructor

template<typename BEAT_PRIMITIVE>
CDMAPool<BEAT_PRIMITIVE>::~CDMAPool() {
    delete[] m_data;
    delete[] m_allocations;
}

// NVI

template<typename BEAT_PRIMITIVE>
IDMAEntity::RESULT CDMAPool<BEAT_PRIMITIVE>::PushAllocation() {
    RESULT result = RESULT::UNDEFINED;

    // check if we have space for another allocation
    if (IsBufferFull() || m_num_allocations == m_num_allocations_max) {
        result = RESULT::FAIL_ERROR;
    } else {
        // populate new allocation
        uint8_t allocationNewIndex =
            (m_allocation_tail_index + m_num_allocations) % m_num_allocations_max;
        ALLOCATION& allocationNew = GetAllocation(allocationNewIndex);
        {
            // if this is the first allocation
            if (m_num_allocations == 0) {
                allocationNew.beat_index = m_beat_tail_index;
            } else {
                // otherwise, continue from the end of the last allocation
                ALLOCATION& allocationActive = *GetAllocationActive();
                allocationNew.beat_index = allocationActive.beat_index + allocationActive.num_beats;
            }

            allocationNew.num_beats = 0;
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
		ALLOCATION& allocationTail = GetAllocation(m_allocation_tail_index);
		
		// mark beats of the tail allocation as free
		m_num_beats -= allocationTail.num_beats;
		
        // remove an allocation
        m_num_allocations--;

        // update some state depending on if there are any remaining allocations
        if (m_num_allocations > 0) {
            m_allocation_tail_index = (m_allocation_tail_index + 1) % m_num_allocations_max;
			
			ALLOCATION& allocationTailNew = GetAllocation(m_allocation_tail_index);
            m_beat_tail_index       = allocationTailNew.beat_index;
        }

        result = RESULT::SUCCESS;
    }

    return result;
}

template<typename BEAT_PRIMITIVE>
IDMAEntity::RESULT CDMAPool<BEAT_PRIMITIVE>::RecordWrite(const BEAT_PRIMITIVE data) {
    RESULT result = RESULT::UNDEFINED;

	if(IsBufferFull()) {
		result = RESULT::FAIL_ERROR;
	} else {
		// if we're about to rollover, try to rebase to the start of the data buffer
		result = RebaseAllocationIfNeeded(1);
	
		if (result == RESULT::SUCCESS) {
			// get currently active allocation
			ALLOCATION& allocationActive = *GetAllocationActive();

			// write the beat to the data buffer
			m_data[allocationActive.beat_index + allocationActive.num_beats] = data;

			// track the new beat
			allocationActive.num_beats++;
			m_num_beats++;
		}
	}

    return result;
}

template<typename BEAT_PRIMITIVE>
IDMAEntity::RESULT CDMAPool<BEAT_PRIMITIVE>::RecordRead(const uint32_t count) {
    RESULT result = IDMAEntity::RESULT::UNDEFINED;

	if(IsBufferFull()) {
		result = RESULT::FAIL_ERROR;
	} else {
		// if we're about to rollover, try to rebase to the start of the data buffer
		result = RebaseAllocationIfNeeded(count);

		if (result == RESULT::SUCCESS) {
			// get currently active allocation
			ALLOCATION& allocationActive = *GetAllocationActive();

			// track new beat
			allocationActive.num_beats += count;
			m_num_beats++;
		}
	}
    return result;
}

/* PRIVATE */

// member functions

template<typename BEAT_PRIMITIVE>
bool CDMAPool<BEAT_PRIMITIVE>::IsBufferFull() const {
	return m_num_beats == m_num_beats_max;
}

template<typename BEAT_PRIMITIVE>
typename CDMAPool<BEAT_PRIMITIVE>::ALLOCATION&
CDMAPool<BEAT_PRIMITIVE>::GetAllocation(const uint8_t index) const {
    return m_allocations[index];
}

template<typename BEAT_PRIMITIVE>
typename CDMAPool<BEAT_PRIMITIVE>::ALLOCATION* const
CDMAPool<BEAT_PRIMITIVE>::GetAllocationActive() const {
    return m_allocation_active;
}

template<typename BEAT_PRIMITIVE>
IDMAEntity::RESULT CDMAPool<BEAT_PRIMITIVE>::RebaseAllocationIfNeeded(const uint32_t beatsToAdd) {
    RESULT result = RESULT::UNDEFINED;

    // check if there's enough space at the beginning of the data buffer
    ALLOCATION* allocationActive = GetAllocationActive();
	
	// if at the end of the buffer
	if((allocationActive->beat_index + allocationActive->num_beats) >= m_num_beats_max) {
		// if we can fit this allocation at the start of this buffer
		if(allocationActive->num_beats + beatsToAdd <= m_beat_tail_index) {
			// copy over this allocation's data to the start of the data buffer
			memcpy(&m_data[0], &m_data[allocationActive->beat_index], allocationActive->num_beats);

			// update the allocation's metadata
			allocationActive->beat_index = 0;

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