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

#include <stdint.h>

class CSPIEntity
{
public:
    enum class STATUS : uint8_t
    {
        OK,
        ERROR_UNKNOWN,
        ERROR_TIMEOUT,
        UNDEFINED = 255
    };

    enum class ROLE : uint8_t
    {
        UNDEFINED = 255,
        WORKER    = 0,
        MANAGER   = 1
    };

    struct DESC
    {
        uint8_t id = 255;
    };

    // destructor
    virtual ~CSPIEntity();

    // NVI
    virtual uint8_t GetId() const final;
    virtual STATUS SetRole(const ROLE) final;

protected:
    // constructor
    CSPIEntity(const DESC&);

private:
    // rule of three
    CSPIEntity(const CSPIEntity&);
    CSPIEntity& operator=(const CSPIEntity&);

    // member variables
    uint8_t m_id;
    ROLE m_role;

    // abstract
    virtual STATUS SetRole_impl(const ROLE) = 0;
};