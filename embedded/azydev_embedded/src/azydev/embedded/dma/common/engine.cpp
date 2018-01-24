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

#include <azydev/embedded/dma/common/engine.h>

/* PUBLIC */

// destructor

CDMAEngine::~CDMAEngine() {
}

// NVI

void CDMAEngine::SetConfig(const CONFIG_DESC& config) {
    SetConfig_impl(config);
}

void CDMAEngine::SetEnabled(const bool enabled) {
    SetEnabled_impl(enabled);
}

CDMAEngine::RESULT CDMAEngine::StartTransfer(const CDMAChannel::TRANSFER_DESC& transfer) {
    RESULT result = RESULT::UNDEFINED;

    CDMAChannel* channel = AcquireFreeChannel_impl();

    if (channel != nullptr) {
        channel->StartTransfer(transfer);
        result = RESULT::SUCCESS;
    } else {
        result = RESULT::FAIL_BUSY;
    }

    return result;
}

/* PROTECTED */

// constructor

CDMAEngine::CDMAEngine(const DESC&) {
}