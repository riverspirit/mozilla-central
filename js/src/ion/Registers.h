/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 4 -*-
 * vim: set ts=8 sts=4 et sw=4 tw=99:
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef ion_Registers_h
#define ion_Registers_h

#include "mozilla/Array.h"

#include "jsutil.h"

// ARM defines the RegisterID within Architecture-arm.h
#if !defined(JS_CPU_ARM)
#include "assembler/assembler/MacroAssembler.h"
#endif
#include "ion/IonTypes.h"
#if defined(JS_CPU_X86)
# include "ion/x86/Architecture-x86.h"
#elif defined(JS_CPU_X64)
# include "ion/x64/Architecture-x64.h"
#elif defined(JS_CPU_ARM)
# include "ion/arm/Architecture-arm.h"
#endif

namespace js {
namespace ion {

struct Register {
    typedef Registers Codes;
    typedef Codes::Code Code;
    typedef js::ion::Registers::RegisterID RegisterID;
    Code code_;

    static Register FromCode(uint32_t i) {
        JS_ASSERT(i < Registers::Total);
        Register r = { (Registers::Code)i };
        return r;
    }
    Code code() const {
        JS_ASSERT((uint32_t)code_ < Registers::Total);
        return code_;
    }
    const char *name() const {
        return Registers::GetName(code());
    }
    bool operator ==(const Register &other) const {
        return code_ == other.code_;
    }
    bool operator !=(const Register &other) const {
        return code_ != other.code_;
    }
    bool volatile_() const {
        return !!((1 << code()) & Registers::VolatileMask);
    }
};

struct FloatRegister {
    typedef FloatRegisters Codes;
    typedef Codes::Code Code;

    Code code_;

    static FloatRegister FromCode(uint32_t i) {
        JS_ASSERT(i < FloatRegisters::Total);
        FloatRegister r = { (FloatRegisters::Code)i };
        return r;
    }
    Code code() const {
        JS_ASSERT((uint32_t)code_ < FloatRegisters::Total);
        return code_;
    }
    const char *name() const {
        return FloatRegisters::GetName(code());
    }
    bool operator ==(const FloatRegister &other) const {
        return code_ == other.code_;
    }
    bool operator !=(const FloatRegister &other) const {
        return code_ != other.code_;
    }
    bool volatile_() const {
        return !!((1 << code()) & FloatRegisters::VolatileMask);
    }
};

// Information needed to recover machine register state.
class MachineState
{
    mozilla::Array<uintptr_t *, Registers::Total> regs_;
    mozilla::Array<double *, FloatRegisters::Total> fpregs_;

  public:
    static MachineState FromBailout(uintptr_t regs[Registers::Total],
                                    double fpregs[FloatRegisters::Total]);

    void setRegisterLocation(Register reg, uintptr_t *up) {
        regs_[reg.code()] = up;
    }
    void setRegisterLocation(FloatRegister reg, double *dp) {
        fpregs_[reg.code()] = dp;
    }

    bool has(Register reg) const {
        return regs_[reg.code()] != NULL;
    }
    bool has(FloatRegister reg) const {
        return fpregs_[reg.code()] != NULL;
    }
    uintptr_t read(Register reg) const {
        return *regs_[reg.code()];
    }
    double read(FloatRegister reg) const {
        return *fpregs_[reg.code()];
    }
    void write(Register reg, uintptr_t value) const {
        *regs_[reg.code()] = value;
    }
};

} // namespace ion
} // namespace js

#endif /* ion_Registers_h */
