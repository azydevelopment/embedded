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

#include <azydev/embedded/clock/common/clock.h>

/* PUBLIC */

CClock::~CClock() {
    // check if every consumer has released this clock
}

uint8_t CClock::GetId() const {
    return m_id;
}

void CClock::SetConfig(const CONFIG_DESC& config) {
    SetConfig_impl(config);
}

void CClock::Acquire() {
    // enable clock if currently disabled
    if (GetState() == STATE::CS0) {
        SetState(STATE::CS1);
    }

    // be sure only to increment after the clock has actually been enabled
    m_num_consumers++;
}

void CClock::Release() {
    // TODO ERROR_HANDLING: over releasing

    m_num_consumers--;

    // if nothing is using this clock, disable it
    if (m_num_consumers == 0) {
        SetState(STATE::CS0);
    }
}

/* PROTECTED */

CClock::CClock(const DESC& desc)
    : m_id(desc.id)
    , m_num_consumers(0)
    , m_state(STATE::CS0) {
}

/* PRIVATE */

// final

CClock::STATE CClock::GetState() const {
    return m_state;
}

void CClock::SetState(const STATE state) {
    SetState_impl(state);
    m_state = state;
}