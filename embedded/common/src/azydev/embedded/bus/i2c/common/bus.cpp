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

#include <azydev/embedded/bus/i2c/common/bus.h>

/* PUBLIC */

CI2CBus::~CI2CBus() {
}

// NVI

void CI2CBus::SetConfig(const CONFIG_DESC& config) {
    SetConfig_impl(config);
}

CI2CEntity::STATUS CI2CBus::SetEnabled(const bool enabled) {
    STATUS status = SetEnabled_impl(enabled);
    m_enabled     = enabled;
    return status;
}

CI2CEntity::STATUS CI2CBus::Start(uint8_t address, const TRANSFER_DIRECTION direction) {
    // TODO ERROR_HANDLING: Check for transfer already in progress
    STATUS status          = Start_impl(address, direction);
    m_transfer_in_progress = true;
    return status;
}

CI2CEntity::STATUS CI2CBus::Write(uint8_t data) {
    return Write_impl(data);
}

CI2CEntity::STATUS CI2CBus::Read(uint8_t& outData) {
    return Read_impl(outData);
}

CI2CEntity::STATUS CI2CBus::Stop() {
    STATUS status          = Stop_impl();
    m_transfer_in_progress = false;
    return status;
}

/* PROTECTED */

CI2CBus::CI2CBus(const DESC& desc)
    : CI2CEntity(desc)
    , m_enabled(false)
    , m_transfer_in_progress(false) {
}
