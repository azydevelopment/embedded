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

#include <azydev/embedded/dma/common/channel.h>

#include <azydev/embedded/dma/common/transfer.h>

/* PUBLIC */

// destructor

CDMAChannel::~CDMAChannel() {
}

// NVI

uint8_t CDMAChannel::GetId() volatile const {
    return m_id;
}

void CDMAChannel::SetConfig(const CONFIG_DESC& config) {
    SetConfig_impl(config);
}

IDMAEntity::RESULT CDMAChannel::StartTransfer(
    CDMATransfer& transfer,
    const CDMATransfer::CONFIG_DESC& transferConfig,
    ITransferControl** transferControl) {
    if (!IsTransferInProgress()) {
        m_transfer_current = &transfer;
        // m_callback_transfer_ended = transfer.callback_transfer_ended;
        StartTransfer_impl(transfer, transferConfig, transferControl);
        return RESULT::SUCCESS;
    } else {
        return RESULT::FAIL_BUSY;
    }
}

bool CDMAChannel::IsTransferInProgress() volatile const {
    return m_transfer_current != nullptr;
}

/* PROTECTED */

// constructor

CDMAChannel::CDMAChannel(const DESC& desc)
    : IDMAEntity()
    , m_id(desc.id)
    , m_transfer_current(nullptr)
    , m_callback_transfer_ended(nullptr) {
}

// member functions

void CDMAChannel::MarkTransferEnded(const RESULT result) {
    MarkTransferEnded_impl(result);
    if (m_callback_transfer_ended != nullptr) {
        uint8_t id = m_transfer_current->GetId();
        m_callback_transfer_ended(id, result);
    }
    m_transfer_current = nullptr;
}