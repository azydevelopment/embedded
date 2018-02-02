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

class IDMAPacket : public IDMAEntity
{
public:
	struct DESC
	{
		uint16_t max_size = 0;
	};

	// constructor
	IDMAPacket(const DESC&);

	// destructor
	~IDMAPacket() final;
	
	// NVI
	virtual void Reset() final;
	virtual void Write(const uint8_t) final;
	virtual const uint8_t* const GetData() const final;
	virtual uint16_t GetNumBytes() const final;

private:
	// rule of three
	IDMAPacket(const IDMAPacket&);
	IDMAPacket& operator=(const IDMAPacket&);
	
	// member variables
	uint16_t m_max_size;
	uint16_t m_num_current_bytes;
	uint8_t* m_data;
};
