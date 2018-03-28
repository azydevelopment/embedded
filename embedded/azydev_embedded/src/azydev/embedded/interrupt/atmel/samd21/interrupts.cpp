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

#include <azydev/embedded/interrupts/atmel/samd21/interrupts.h>

#include <asf.h>

/* FILE SCOPED STATICS */

static const uint8_t NUM_ISRS = CInterruptsAtmelSAMD21::ISR_ID::COUNT;
static CInterrupts::ISR fs_isrs[NUM_ISRS];

/* PUBLIC */

// destructor

CInterruptsAtmelSAMD21::~CInterruptsAtmelSAMD21() {
}

/* PROTECTED */

// constructor

CInterruptsAtmelSAMD21::CInterruptsAtmelSAMD21()
    : CInterrupts() {
}

/* PRIVATE */

// CInterrupts

void CInterruptsAtmelSAMD21::SetISR_impl(const uint8_t id, const ISR isr) {
    if (id < ISR_ID::COUNT) {
        fs_isrs[id] = isr;
    }
}

/* ISRs */

void DMAC_Handler() {
    CInterrupts::ISR isr = fs_isrs[CInterruptsAtmelSAMD21::ISR_ID::ISR_DMAC];
    if (isr != nullptr) {
        isr();
    }
}