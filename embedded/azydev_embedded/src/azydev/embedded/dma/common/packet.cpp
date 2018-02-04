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

#include <azydev/embedded/dma/common/packet.h>

/* PUBLIC */

// constructor

IDMAPacket::IDMAPacket(const DESC& desc)
: m_max_length(desc.max_size)
, m_data_type(desc.data_type)
, m_data_length(0)
, m_data({}) {

	switch(GetDataType()) {
		case DATA_TYPE::UINT8_T:
			m_data.data_8bit = new uint8_t[GetDataLength()];
			break;
		case DATA_TYPE::UINT16_T:
			m_data.data_16bit = new uint16_t[GetDataLength()];
			break;
		case DATA_TYPE::UINT32_T:
			m_data.data_32bit = new uint32_t[GetDataLength()];
			break;
	}

}

// destructor

IDMAPacket::~IDMAPacket() {
	switch(GetDataType()) {
		case DATA_TYPE::UINT8_T:
			delete[] m_data.data_8bit;
			break;
		case DATA_TYPE::UINT16_T:
			delete[] m_data.data_16bit;
			break;
		case DATA_TYPE::UINT32_T:
			delete[] m_data.data_32bit;
			break;
	}
}

// NVI

void IDMAPacket::Reset() {
	m_data_length = 0;
}

void IDMAPacket::Write(const uint8_t data) {
	if(m_data_length < m_max_length) {
		switch(GetDataType()) {
			case DATA_TYPE::UINT8_T:
				m_data.data_8bit[m_data_length] = data;
				break;
			case DATA_TYPE::UINT16_T:
				m_data.data_16bit[m_data_length] = data;
				break;
			case DATA_TYPE::UINT32_T:
				m_data.data_16bit[m_data_length] = data;
				break;
		}
		m_data_length++;
	}
}

IDMAPacket::DATA_TYPE IDMAPacket::GetDataType() const {
	return m_data_type;
}

uint16_t IDMAPacket::GetDataLength() const {
	return m_data_length;
}

const IDMAPacket::DATA* const IDMAPacket::GetData() const {
	return &m_data;
}

