/*
* Copyright (C) Andrew Yeung - All Rights Reserved
* Unauthorized copying of this file, via any medium is strictly prohibited
* Proprietary and confidential
* Written by Andrew Yeung <azy.development@gmail.com>, May 2017
*/

#include <azydev/embedded/logging/atmel/samd21/logger.h>

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
