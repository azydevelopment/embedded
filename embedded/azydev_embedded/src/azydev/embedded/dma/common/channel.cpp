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

void CDMAChannel::StartTransfer(const TRANSFER_DESC& transfer, ITransferControl** transferControl) {
    m_transfer_in_progress = true;
	m_transfer_id_current = transfer.id;
	m_callback_transfer_complete = transfer.callback_transfer_complete;
    StartTransfer_impl(transfer, transferControl);
}

bool CDMAChannel::IsTransferInProgress() volatile const {
    return m_transfer_in_progress;
}

/* PROTECTED */

// constructor

CDMAChannel::CDMAChannel(const DESC& desc)
    : IDMAEntity()
    , m_id(desc.id)
    , m_transfer_in_progress(false)
    , m_transfer_id_current(255)
    , m_callback_transfer_complete(nullptr) {
}

// member functions

void CDMAChannel::MarkTransferComplete() {
	MarkTransferComplete_impl();
	if(m_callback_transfer_complete != nullptr) {
		m_callback_transfer_complete(m_transfer_id_current);
	}
	m_transfer_in_progress = false;
}