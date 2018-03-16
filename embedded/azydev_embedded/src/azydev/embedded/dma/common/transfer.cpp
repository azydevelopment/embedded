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

#include <azydev/embedded/dma/common/transfer.h>

/* PUBLIC */

// constructor

CDMATransfer::CDMATransfer(const DESC& desc)
    : IDMAEntity()
    , m_id(desc.id_initial)
    , m_num_beats(0) {
}

// destructor

CDMATransfer::~CDMATransfer() {
}

// NVI

uint8_t CDMATransfer::GetId() volatile const {
    return m_id;
}

void CDMATransfer::Reset(const uint8_t id) {
    Reset_impl();
    m_id        = id;
    m_num_beats = 0;
}

IDMAEntity::RESULT CDMATransfer::AddStep(const STEP_DESC& step) {
    m_num_beats += step.num_beats;
    return AddStep_impl(step);
}

uint32_t CDMATransfer::GetNumBeats() const {
    return m_num_beats;
}