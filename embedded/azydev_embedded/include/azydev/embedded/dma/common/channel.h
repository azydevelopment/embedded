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

#pragma once

#include <azydev/embedded/dma/common/entity.h>

#include <stdint.h>

class CDMAChannel : public IDMAEntity
{
public:
    struct CONFIG_DESC
    {};

    struct DESC
    { uint8_t id = 255; };

    // destructor
    virtual ~CDMAChannel() override;

    // NVI
    virtual uint8_t GetId() volatile const final;
    virtual void SetConfig(const CONFIG_DESC&) final;
    virtual void StartTransfer(const TRANSFER_DESC&) final;
    virtual bool IsBusy() volatile const final;

protected:
    // constructor
    CDMAChannel(const DESC&);
	
	// NVI
	virtual void MarkTransferComplete() volatile final;

private:
    // rule of three
    CDMAChannel(const CDMAChannel&);
    CDMAChannel& operator=(const CDMAChannel&);

    // member variables
    uint8_t const m_id;
    volatile bool m_busy;
    volatile uint8_t m_transfer_id_current;
    volatile OnTransferComplete m_callback_transfer_complete;

    // abstract
    virtual void SetConfig_impl(const CONFIG_DESC&)       = 0;
    virtual void StartTransfer_impl(const TRANSFER_DESC&) = 0;
	virtual void MarkTransferComplete_impl() volatile = 0;
};
