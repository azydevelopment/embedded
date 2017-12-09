// TODO HACK: Didn't keep this up to date

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
//#pragma once
//
//#include <azydev/embedded/bus/i2c/atmel/avr8/bus.h>
//
//#include <azydev/embedded/bus/i2c/common/fragment.h>
//#include <azydev/embedded/bus/i2c/common/fragment_data.h>
//#include <azydev/embedded/bus/i2c/common/fragment_start.h>
//#include <azydev/embedded/bus/i2c/common/message.h>
//#include <azydev/embedded/pins/common/pins.h>
//
///* STATICS */
//
//// TODO HACK: How do I not restrict everything to literally one available bus
//volatile CI2CBusAtmelAVR8* CI2CBusAtmelAVR8::g_i2c_bus_avr8;
//
//void CI2CBusAtmelAVR8::I2CBusAVR8Init(
//    const CI2CBusAtmelAVR8::DESC& desc,
//    CPins<uint8_t>& servicePins) {
//    if (g_i2c_bus_avr8 != nullptr) {
//        delete g_i2c_bus_avr8;
//    }
//    g_i2c_bus_avr8 = new CI2CBusAtmelAVR8(desc, servicePins);
//}
//
///* PUBLIC */
//
//CI2CBusAtmelAVR8::CI2CBusAtmelAVR8(const DESC& desc, CPins<uint8_t>& servicePins)
//    : m_service_pins(servicePins)
//    , m_bus_config({})
//    , m_pin_config(desc.pin_config)
//    , m_register_config(desc.register_config)
//    , m_enabled_acknowledge(false) {
//}
//
//CI2CBusAtmelAVR8::~CI2CBusAtmelAVR8() {
//}
//
//void CI2CBusAtmelAVR8::UpdateStateMachine() volatile {
//    // TODO ERROR_HANDLING: When should we check for write collisions
//
//    CI2CMessage* message = GetActiveMessage();
//
//    CI2CFragment& fragment                   = message->GetCurrentFragment();
//    CI2CFragment::FRAGMENT_TYPE fragmentType = fragment.GetType();
//
//    STATUS status = GetStatus();
//
//    switch (status) {
//    case CI2CBusAtmelAVR8::STATUS::MX_START:
//    case CI2CBusAtmelAVR8::STATUS::MX_START_REPEATED: {
//        const CI2CFragmentStart& fragmentStart = static_cast<const CI2CFragmentStart&>(fragment);
//
//        // write slarw to assert the worker device
//        WriteData(fragmentStart.GetSlarw());
//
//        message->Advance();
//
//        // if this is a repeated start, advance again since there was no initial advance from
//        // message process kickoff
//        if (status == CI2CBusAtmelAVR8::STATUS::MX_START_REPEATED) {
//            message->Advance();
//        }
//
//        OnStart();
//
//        EnableAcknowledgements(true);
//        UpdateConfigReg(true, false, false, false);
//    } break;
//
//    case CI2CBusAtmelAVR8::STATUS::MX_ARB_LOST: {
//        OnError(ARBITRATION_LOST);
//        // TODO IMPLEMENT: Auto-retry; (ie. reset counters, send another start condition
//        // automatically, and do not release the message objects).
//        UpdateConfigReg(true, false, false, false);
//    } break;
//
//    case CI2CBusAtmelAVR8::STATUS::MT_SLAW_NACK: {
//        // if we get an SLAW NACK, notify a device was not found
//        OnError(DEVICE_NOT_FOUND);
//    }
//    case CI2CBusAtmelAVR8::STATUS::MT_DATA_NACK: {
//        // TODO IMPLEMENT: Technically, continuing to send data after a NACK is within spec
//        message->SkipToNextStartOrStop();
//    }
//    case CI2CBusAtmelAVR8::STATUS::MT_SLAW_ACK:
//    case CI2CBusAtmelAVR8::STATUS::MT_DATA_ACK: {
//        // must assume fragment has changed if we've skipped fragments in the above state
//        CI2CFragment& fragmentNew                   = message->GetCurrentFragment();
//        CI2CFragment::FRAGMENT_TYPE fragmentTypeNew = fragmentNew.GetType();
//
//        bool start = false;
//        bool stop  = false;
//
//        switch (fragmentTypeNew) {
//        case CI2CFragment::FRAGMENT_TYPE::START: {
//            // if we're here, TX fragment transmit must be done
//            OnFragmentTransmit();
//
//            start = true;
//            message->Advance();
//        } break;
//
//        case CI2CFragment::FRAGMENT_TYPE::DATA: {
//            CI2CFragmentData& fragmentDataTx = static_cast<CI2CFragmentData&>(fragmentNew);
//
//            if (fragmentDataTx.IsBytesAvailableToRead()) {
//                WriteData(fragmentDataTx.ReadByte());
//            } else {
//                // go to the next start or stop
//                message->SkipToNextStartOrStop();
//
//                // set the start/stop command flags accordingly
//                CI2CFragment::FRAGMENT_TYPE fragType = message->GetCurrentFragment().GetType();
//                start = fragType == CI2CFragment::FRAGMENT_TYPE::START;
//                stop  = fragType == CI2CFragment::FRAGMENT_TYPE::STOP;
//            }
//        } break;
//
//        case CI2CFragment::FRAGMENT_TYPE::STOP: {
//            // if we're here, TX fragment transmit must be done
//            OnFragmentTransmit();
//
//            // potential to intentionally lose control of the bus then try again after (different
//            // than repeat start)
//            start = (message->PeekNextFragmentType() == CI2CFragment::FRAGMENT_TYPE::START);
//            stop  = true;
//            message->Advance();
//        } break;
//
//        default: {
//            OnError(INVALID_STATE_CHANGE);
//            message->Advance();
//        } break;
//        }
//
//        UpdateConfigReg(true, start, stop, false);
//
//        if (stop) {
//            WaitForStop();
//            OnStop();
//            if (message->IsDone()) {
//                OnMessageDone();
//            }
//        }
//
//    } break;
//
//    case CI2CBusAtmelAVR8::STATUS::MR_DATA_NACK: {
//        // read data
//        CI2CFragmentData& fragmentDataRx         = static_cast<CI2CFragmentData&>(fragment);
//        CI2CFragment::FRAGMENT_TYPE fragmentType = fragment.GetType();
//        fragmentDataRx.WriteByte(ReadData());
//    }
//    case CI2CBusAtmelAVR8::STATUS::MR_SLAR_NACK: {
//        // if we get an SLAW NACK, notify a device was not found
//        // Note: need an extra status check here since only a received NACK is invalid
//        // If the state is MR_DATA_NACK, that means we *returned*, not received, a NACK which is
//        // valid
//        if (status == CI2CBusAtmelAVR8::STATUS::MR_SLAR_NACK) {
//            OnError(DEVICE_NOT_FOUND);
//        }
//
//        // NACKs not allowed in this state whether it's data or an slarw; must restart or stop
//        // Even if there's space to receive, we're already received everything we've expected so
//        // move on
//        message->SkipToNextStartOrStop();
//
//        // must assume fragment has changed if we've skipped fragments in the above state
//        CI2CFragment& fragmentNew                   = message->GetCurrentFragment();
//        CI2CFragment::FRAGMENT_TYPE fragmentTypeNew = fragmentNew.GetType();
//
//        bool start = false;
//        bool stop  = false;
//
//        switch (fragmentTypeNew) {
//        case CI2CFragment::FRAGMENT_TYPE::START:
//            start = true;
//            break;
//        case CI2CFragment::FRAGMENT_TYPE::STOP:
//            // potential to intentionally lose control of the bus then try again after (different
//            // than repeat start)
//            start = (message->PeekNextFragmentType() == CI2CFragment::FRAGMENT_TYPE::START);
//            stop  = true;
//            break;
//        default:
//            OnError(INVALID_STATE_CHANGE);
//            break;
//        }
//
//        message->Advance();
//
//        UpdateConfigReg(true, start, stop, false);
//
//        OnFragmentReceive(static_cast<const CI2CFragmentData&>(fragment));
//
//        if (stop) {
//            WaitForStop();
//            OnStop();
//            if (message->IsDone()) {
//                OnMessageDone();
//            }
//        }
//    } break;
//
//    case CI2CBusAtmelAVR8::STATUS::MR_DATA_ACK: {
//        // read data
//        CI2CFragmentData& fragmentDataRx         = static_cast<CI2CFragmentData&>(fragment);
//        CI2CFragment::FRAGMENT_TYPE fragmentType = fragment.GetType();
//        fragmentDataRx.WriteByte(ReadData());
//    }
//    case CI2CBusAtmelAVR8::STATUS::MR_SLAR_ACK: {
//        CI2CFragmentData& fragmentDataRx = static_cast<CI2CFragmentData&>(fragment);
//
//        switch (fragmentType) {
//        case CI2CFragment::FRAGMENT_TYPE::DATA: // only choice is to received data
//
//            // note: interestingly enough, we don't advance the message here
//            // we only ever potentially send a NACK which then puts us in a different state that'll
//            // subsequently advance the message
//            EnableAcknowledgements(fragmentDataRx.GetNumExpectedBytesRemaining() > 1);
//
//            UpdateConfigReg(true, false, false, false);
//            break;
//        default:
//            OnError(INVALID_STATE_CHANGE);
//            break;
//        }
//    } break;
//
//    case CI2CBusAtmelAVR8::STATUS::SR_DATA_ACK:
//    case CI2CBusAtmelAVR8::STATUS::SR_GENERAL_CALL_DATA_ACK: {
//        // read data
//        // TODO ERROR_HANDLING: Assumed data fragment
//        CI2CFragmentData& fragmentDataRx         = static_cast<CI2CFragmentData&>(fragment);
//        CI2CFragment::FRAGMENT_TYPE fragmentType = fragment.GetType();
//
//        fragmentDataRx.WriteByte(ReadData());
//
//        bool isWriteSpaceAvailable = fragmentDataRx.IsWriteSpaceAvailable();
//        EnableAcknowledgements(isWriteSpaceAvailable);
//
//        // if no more space is available, go to the next fragment
//        if (!isWriteSpaceAvailable) {
//            message->AdvanceFragment();
//        }
//
//        switch (fragmentType) {
//        case CI2CFragment::FRAGMENT_TYPE::DATA: // only choice is to received data
//            UpdateConfigReg(true, false, false, false);
//            break;
//        default:
//            OnError(INVALID_STATE_CHANGE);
//            break;
//        }
//    } break;
//
//    case CI2CBusAtmelAVR8::STATUS::SR_SLAW_ACK:
//    case CI2CBusAtmelAVR8::STATUS::SR_ARBITRATION_LOST_SLAW_ACK:
//    case CI2CBusAtmelAVR8::STATUS::SR_GENERAL_CALL_ACK:
//    case CI2CBusAtmelAVR8::STATUS::SR_ARBITRATION_LOST_GENERAL_CALL_ACK: {
//        OnAssert();
//
//        switch (fragmentType) {
//        case CI2CFragment::FRAGMENT_TYPE::DATA: // only choice is to received data
//            UpdateConfigReg(true, false, false, false);
//            break;
//        default:
//            OnError(INVALID_STATE_CHANGE);
//            break;
//        }
//
//        // note: no fragment advancement required
//    } break;
//
//    case CI2CBusAtmelAVR8::STATUS::SR_DATA_NACK:
//    case CI2CBusAtmelAVR8::STATUS::SR_GENERAL_CALL_DATA_NACK: {
//        // read data
//        CI2CFragmentData& fragmentDataRx         = static_cast<CI2CFragmentData&>(fragment);
//        CI2CFragment::FRAGMENT_TYPE fragmentType = fragment.GetType();
//        fragmentDataRx.WriteByte(ReadData());
//    }
//    case CI2CBusAtmelAVR8::STATUS::SR_STOP_OR_START_REPEATED: {
//        // TODO IMPLEMENT: The case where this device immediately wants to try to become a manager
//        // device
//
//        // only enable acknwoledgements if we are expecting more
//        bool done = message->IsDone();
//        EnableAcknowledgements(!done);
//
//        // go to the next fragment if not done
//        if (!done) {
//            message->AdvanceFragment();
//        }
//
//        // tell the client we've finished receiving a fragment and will be deasserted
//        OnFragmentReceive(static_cast<const CI2CFragmentData&>(fragment));
//        OnDeassert();
//
//        // if we're done the whole message, indicate so
//        if (done) {
//            OnMessageDone();
//        }
//
//        UpdateConfigReg(true, false, false, false);
//    } break;
//
//    case CI2CBusAtmelAVR8::STATUS::ST_SLAR_ACK:
//    case CI2CBusAtmelAVR8::STATUS::ST_ARBITRATION_LOST_SLAR_ACK: {
//        OnAssert();
//    }
//    case CI2CBusAtmelAVR8::STATUS::ST_DATA_ACK: {
//        switch (fragmentType) {
//        case CI2CFragment::FRAGMENT_TYPE::DATA: // only choice is to transmit data and continue
//        {
//            // TODO BUG: This code assumes that there must always be at least one byte to send.
//
//            CI2CFragmentData& fragmentDataTx = static_cast<CI2CFragmentData&>(fragment);
//
//            // TODO ERROR_HANDLING: Does this get triggered at the right time? Does it prevent
//            // overruns in all cases?
//            bool bytesAvailableToRead = fragmentDataTx.IsBytesAvailableToRead();
//            WriteData(fragmentDataTx.ReadByte());
//
//            // NACK when it's the last byte
//            EnableAcknowledgements(bytesAvailableToRead);
//
//            // go to the next fragment when done transmitting this fragment
//            if (!bytesAvailableToRead) {
//                message->AdvanceFragment();
//            }
//
//            UpdateConfigReg(true, false, false, false);
//        } break;
//
//        default: { OnError(INVALID_STATE_CHANGE); } break;
//        }
//    } break;
//
//    case CI2CBusAtmelAVR8::STATUS::ST_DATA_NACK: {
//        // manager understands the last byte was sent by sending NACK
//    }
//    case CI2CBusAtmelAVR8::STATUS::ST_LAST_DATA_ACK: {
//        // note: if we get an ack instead from the manager, it is demanding more bytes
//
//        // advance the fragment since this was the last byte in the current fragment
//        message->AdvanceFragment();
//
//        // TODO IMPLEMENT: Leaving acknowledgments on is within spec too
//        // leaving it off means this device must explicitly process a new message before it can be
//        // addressed
//        EnableAcknowledgements(false);
//
//        OnFragmentTransmit();
//        OnDeassert();
//
//        bool start = false;
//        if (message->IsDone()) {
//            OnMessageDone();
//        } else {
//            if (message->GetCurrentFragment().GetType() == CI2CFragment::FRAGMENT_TYPE::START) {
//                start = true;
//            }
//        }
//
//        UpdateConfigReg(true, start, false, false);
//    } break;
//    }
//}
//
//// CI2CBus
//
//void CI2CBusAtmelAVR8::SetEnabled(bool enabled) volatile {
//    // call parent method
//    CI2CBus::SetEnabled(enabled);
//
//    // TODO ERROR_HANDLING: Handle if already enabled
//    if (enabled) {
//        // configure pins for I2C
//        m_service_pins.PinWriteDigital(
//            m_pin_config.scl, CPins<uint8_t>::DIGITAL_STATE::DIGITAL_HIGH);
//        m_service_pins.PinWriteDigital(
//            m_pin_config.sda, CPins<uint8_t>::DIGITAL_STATE::DIGITAL_HIGH);
//
//        CPins<uint8_t>::CONFIG_DESC config = {};
//        config.direction                   = CPins<uint8_t>::DIRECTION::IN;
//        config.pull                        = CPins<uint8_t>::PULL::UP;
//
//        m_service_pins.SetPinConfig(m_pin_config.scl, config);
//        m_service_pins.SetPinConfig(m_pin_config.sda, config);
//
//        // init bitrate
//        *m_register_config.bitrate_register = m_bus_config.bit_rate;
//
//        // init prescalar
//        Binary::BSR<volatile uint8_t>(
//            *m_register_config.prescalar.address,
//            m_register_config.prescalar.lsb_offset,
//            m_register_config.prescalar.length,
//            m_bus_config.prescalar);
//
//        // init address mask
//        Binary::BSR<volatile uint8_t>(
//            *m_register_config.address_mask.address,
//            m_register_config.address_mask.lsb_offset,
//            m_register_config.address_mask.length,
//            m_bus_config.address_mask);
//
//        // init address
//        Binary::BSR<volatile uint8_t>(
//            *m_register_config.address.address,
//            m_register_config.address.lsb_offset,
//            m_register_config.address.length,
//            m_bus_config.address);
//
//        // init general call recognition
//        Binary::BS<volatile uint8_t>(
//            *m_register_config.enable_general_call_recognition.address,
//            m_register_config.enable_general_call_recognition.offset,
//            m_bus_config.enable_general_call_recognition);
//    }
//
//    UpdateConfigReg(false, false, false, false);
//}
//
//void CI2CBusAtmelAVR8::ProcessMessage(CI2CMessage& message) volatile {
//    // call parent method
//    CI2CBus::ProcessMessage(message);
//
//    EnableAcknowledgements(true);
//
//    const CI2CFragment& firstFragment        = GetActiveMessage()->GetCurrentFragment();
//    CI2CFragment::FRAGMENT_TYPE fragmentType = firstFragment.GetType();
//
//    // only start, general call, and data fragments can be first
//    // TODO IMPLEMENT: General call
//    switch (fragmentType) {
//    case CI2CFragment::FRAGMENT_TYPE::START:
//        UpdateConfigReg(true, true, false, false);
//        GetActiveMessage()->Advance();
//        break;
//    case CI2CFragment::FRAGMENT_TYPE::DATA:
//        UpdateConfigReg(true, false, false, false);
//        break;
//    default:
//        OnError(INVALID_STATE_CHANGE);
//        break;
//    }
//}
//
//void CI2CBusAtmelAVR8::SetConfig(const CI2CBus::CONFIG_DESC& busConfig) volatile {
//    // TODO ERROR_HANDLING: Dangerous cast.  Can use GUID for struct type for safety?
//    m_bus_config = static_cast<const CONFIG_DESC&>(busConfig);
//}
//
///* PROTECTED */
//
//// CI2CBusAtmel
//
//void CI2CBusAtmelAVR8::WaitForStop() volatile {
//    while (Binary::BC<uint8_t>(
//        *m_register_config.stop_condition.address, m_register_config.stop_condition.offset)) {
//    }
//}
//
//uint8_t CI2CBusAtmelAVR8::ReadData() volatile {
//    return *m_register_config.data_register;
//}
//
//void CI2CBusAtmelAVR8::WriteData(uint8_t data) volatile {
//    *m_register_config.data_register = data;
//}
//
//void CI2CBusAtmelAVR8::ForceStop() volatile {
//    UpdateConfigReg(true, false, true, false);
//    WaitForStop();
//}
//
///* PRIVATE */
//
//CI2CBusAtmelAVR8::STATUS CI2CBusAtmelAVR8::GetStatus() volatile {
//    // TODO HACK:: Kind of a weird one.  Status is calculated without shifting status bits down to
//    // start at bit 0
//    // Only need to mask out the prescalar bits
//    STATUS status =
//        static_cast<STATUS>(*m_register_config.status.address & Binary::BM<volatile uint8_t>(3, 5));
//    return status;
//}
//
//void CI2CBusAtmelAVR8::UpdateConfigReg(bool twint, bool twsta, bool twsto, bool twwc) volatile {
//    *m_register_config.control_register =
//        twint << m_register_config.interrupt_flag.offset
//        | m_enabled_acknowledge << m_register_config.enable_acknowledge.offset
//        | twsta << m_register_config.start_condition.offset
//        | twsto << m_register_config.stop_condition.offset
//        | twwc << m_register_config.write_collision_flag.offset
//        | IsEnabled() << m_register_config.enable.offset
//        | m_bus_config.enable_interrupt << m_register_config.enable_interrupt.offset;
//}
//
//void CI2CBusAtmelAVR8::EnableAcknowledgements(bool enable) volatile {
//    m_enabled_acknowledge = enable;
//}
