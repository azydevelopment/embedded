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

#include <azydev/embedded/logging/atmel/samd21/logger.h>

#include <string.h>

/* PUBLIC */

// constructor
CLoggerAtmelSAMD21::CLoggerAtmelSAMD21()
    : CLogger() {
}

// destructor
CLoggerAtmelSAMD21::~CLoggerAtmelSAMD21() {
}

// copy constructor
CLoggerAtmelSAMD21::CLoggerAtmelSAMD21(const CLoggerAtmelSAMD21&) {
}

/* PRIVATE */

// CLogger

void CLoggerAtmelSAMD21::Init_impl() {
    // TODO IMPLEMENT: Other logger output possibilities

    struct usart_config config;
    usart_get_config_defaults(&config);
    config.pinmux_pad0 = PINMUX_PA22C_SERCOM3_PAD0;
    config.pinmux_pad1 = PINMUX_PA23C_SERCOM3_PAD1;
    config.pinmux_pad2 = PINMUX_UNUSED;
    config.pinmux_pad3 = PINMUX_UNUSED;
    config.mux_setting = USART_RX_1_TX_0_XCK_1;
    config.baudrate    = 115200;

    // wait until the usart has been initialized
    while (usart_init(&m_usart, SERCOM3, &config) != STATUS_OK) {
    }
}

void CLoggerAtmelSAMD21::SetEnabled_impl(const bool) {
    usart_enable(&m_usart);
}

void CLoggerAtmelSAMD21::Log_impl(const char msg[], const bool eol) {
    // TODO HACK: C-style cast
    usart_write_buffer_wait(&m_usart, (const uint8_t*)msg, strlen(msg));
    if (eol) {
        LogEol();
    }
}
