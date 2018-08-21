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

#include <azydev/embedded/dma/atmel/samd21/transfer.h>

#include <azydev/embedded/dma/common/node.h>

/* PUBLIC */

// constructor

template<typename BEAT_PRIMITIVE>
CDMATransferAtmelSAMD21<BEAT_PRIMITIVE>::CDMATransferAtmelSAMD21(const DESC& desc)
    : CDMATransfer<BEAT_PRIMITIVE>(desc)
    , m_num_steps_max(desc.num_steps_max)
    , m_current_step(0)
    , m_descriptor_chain(nullptr) {
    m_descriptor_chain = new DESCRIPTOR[GetNumStepsMax()];
}

// destructor

template<typename BEAT_PRIMITIVE>
CDMATransferAtmelSAMD21<BEAT_PRIMITIVE>::~CDMATransferAtmelSAMD21() {
    delete[] m_descriptor_chain;
}

// NVI

template<typename BEAT_PRIMITIVE>
const typename CDMATransferAtmelSAMD21<BEAT_PRIMITIVE>::DESCRIPTOR
CDMATransferAtmelSAMD21<BEAT_PRIMITIVE>::GetBaseDescriptor() const {
    return m_descriptor_chain[0];
}

/* PRIVATE */

// member functions

template<typename BEAT_PRIMITIVE>
uint8_t CDMATransferAtmelSAMD21<BEAT_PRIMITIVE>::GetNumStepsMax() const {
    return m_num_steps_max;
}

template<typename BEAT_PRIMITIVE>
uint8_t CDMATransferAtmelSAMD21<BEAT_PRIMITIVE>::GetCurrentStep() const {
    return m_current_step;
}

// CDMATransfer

template<typename BEAT_PRIMITIVE>
void CDMATransferAtmelSAMD21<BEAT_PRIMITIVE>::Reset_impl() {
    // reset the number of steps to 0
    m_current_step = 0;

    // invalidate the first descriptor
    m_descriptor_chain->btctrl.bits.valid       = 0;
    m_descriptor_chain->next_descriptor_address = 0;
}

template<typename BEAT_PRIMITIVE>
bool CDMATransferAtmelSAMD21<BEAT_PRIMITIVE>::IsStepAvailable_impl(uint8_t const numSteps) const {
    return (GetCurrentStep() + numSteps) < GetNumStepsMax();
}

template<typename BEAT_PRIMITIVE>
IDMAEntity::RESULT CDMATransferAtmelSAMD21<BEAT_PRIMITIVE>::AddStep_impl(
    const typename CDMATransfer<BEAT_PRIMITIVE>::STEP_DESC& step) {
    IDMAEntity::RESULT result = IDMAEntity::RESULT::UNDEFINED;

    if (GetCurrentStep() >= GetNumStepsMax()) {
        result = IDMAEntity::RESULT::FAIL_ERROR;
    } else {
        const IDMANode<BEAT_PRIMITIVE>* nodeSrc = step.node_source;
        const IDMANode<BEAT_PRIMITIVE>* nodeDst = step.node_destination;

        // calculate source data address
        uintptr_t addressSrc = 0;
        {
            uint32_t offset = 0;
            if (nodeSrc->IsIncrementing()) {
                offset = step.num_beats * nodeSrc->GetSizeOfBeatPrimitive();
            }

            addressSrc = nodeSrc->GetBaseAddress() + offset;
        }

        // calculate source data destination
        uint32_t addressDest = 0;
        {
            uint32_t offset = 0;
            if (nodeDst->IsIncrementing()) {
                offset = step.num_beats * nodeDst->GetSizeOfBeatPrimitive();
            }

            addressDest = nodeDst->GetBaseAddress() + offset;
        }

        // configure descriptors
        {
            const CDMATransferAtmelSAMD21::STEP_DESC& stepSAMD21 =
                static_cast<const CDMATransferAtmelSAMD21::STEP_DESC&>(step);

            uint8_t currentStep = GetCurrentStep();

            // populate this step's descriptor
            DESCRIPTOR& descriptor        = m_descriptor_chain[currentStep];
            descriptor.btctrl.bits.valid  = true;
            descriptor.btctrl.bits.evosel = static_cast<uint8_t>(stepSAMD21.event_output_selection);
            descriptor.btctrl.bits.blockact =
                static_cast<uint8_t>(stepSAMD21.block_completed_action);
            descriptor.btctrl.bits.beatsize    = static_cast<uint8_t>(stepSAMD21.beat_size);
            descriptor.btctrl.bits.srcinc      = static_cast<uint8_t>(nodeSrc->IsIncrementing());
            descriptor.btctrl.bits.dstinc      = static_cast<uint8_t>(nodeDst->IsIncrementing());
            descriptor.btctrl.bits.stepsel     = static_cast<uint8_t>(stepSAMD21.step_size_select);
            descriptor.btctrl.bits.stepsize    = static_cast<uint8_t>(stepSAMD21.step_size);
            descriptor.num_beats               = stepSAMD21.num_beats;
            descriptor.source_address          = addressSrc;
            descriptor.destination_address     = addressDest;
            descriptor.next_descriptor_address = 0;

            // link previous descriptor to this one if necessary
            if (currentStep > 0) {
                DESCRIPTOR& descriptorPrevious = m_descriptor_chain[currentStep - 1];
                descriptorPrevious.next_descriptor_address =
                    reinterpret_cast<uint32_t>(&descriptor);
            }
        }

        m_current_step++;

        result = IDMAEntity::RESULT::SUCCESS;
    }

    return result;

    return IDMAEntity::RESULT::SUCCESS;
}
