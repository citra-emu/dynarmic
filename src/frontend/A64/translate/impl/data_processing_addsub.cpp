/* This file is part of the dynarmic project.
 * Copyright (c) 2018 MerryMage
 * This software may be used and distributed according to the terms of the GNU
 * General Public License version 2 or any later version.
 */

#include "frontend/A64/translate/impl/impl.h"

namespace Dynarmic {
namespace A64 {

bool TranslatorVisitor::ADD_imm(bool sf, Imm<2> shift, Imm<12> imm12, Reg Rn, Reg Rd) {
    size_t datasize = sf ? 64 : 32;

    u64 imm;
    switch (shift.ZeroExtend()) {
    case 0b00:
        imm = imm12.ZeroExtend<u64>();
        break;
    case 0b01:
        imm = imm12.ZeroExtend<u64>() << 12;
        break;
    default:
        return ReservedValue();
    }

    auto operand1 = Rn == Reg::SP ? SP(datasize) : X(datasize, Rn);

    auto result = ir.Add(operand1, I(datasize, imm));

    if (Rd == Reg::SP) {
        SP(datasize, result);
    } else {
        X(datasize, Rd, result);
    }

    return true;
}

bool TranslatorVisitor::ADDS_imm(bool sf, Imm<2> shift, Imm<12> imm12, Reg Rn, Reg Rd) {
    size_t datasize = sf ? 64 : 32;

    u64 imm;
    switch (shift.ZeroExtend()) {
    case 0b00:
        imm = imm12.ZeroExtend<u64>();
        break;
    case 0b01:
        imm = imm12.ZeroExtend<u64>() << 12;
        break;
    default:
        return ReservedValue();
    }

    auto operand1 = Rn == Reg::SP ? SP(datasize) : X(datasize, Rn);

    auto result = ir.Add(operand1, I(datasize, imm));

    ir.SetNZCV(ir.NZCVFrom(result));

    X(datasize, Rd, result);

    return true;
}

bool TranslatorVisitor::SUB_imm(bool sf, Imm<2> shift, Imm<12> imm12, Reg Rn, Reg Rd) {
    size_t datasize = sf ? 64 : 32;

    u64 imm;
    switch (shift.ZeroExtend()) {
    case 0b00:
        imm = imm12.ZeroExtend<u64>();
        break;
    case 0b01:
        imm = imm12.ZeroExtend<u64>() << 12;
        break;
    default:
        return ReservedValue();
    }

    auto operand1 = Rn == Reg::SP ? SP(datasize) : X(datasize, Rn);

    auto result = ir.Sub(operand1, I(datasize, imm));

    if (Rd == Reg::SP) {
        SP(datasize, result);
    } else {
        X(datasize, Rd, result);
    }

    return true;
}

bool TranslatorVisitor::SUBS_imm(bool sf, Imm<2> shift, Imm<12> imm12, Reg Rn, Reg Rd) {
    size_t datasize = sf ? 64 : 32;

    u64 imm;
    switch (shift.ZeroExtend()) {
    case 0b00:
        imm = imm12.ZeroExtend<u64>();
        break;
    case 0b01:
        imm = imm12.ZeroExtend<u64>() << 12;
        break;
    default:
        return ReservedValue();
    }

    auto operand1 = Rn == Reg::SP ? SP(datasize) : X(datasize, Rn);

    auto result = ir.Sub(operand1, I(datasize, imm));

    ir.SetNZCV(ir.NZCVFrom(result));

    X(datasize, Rd, result);

    return true;
}

bool TranslatorVisitor::ADD_shift(bool sf, Imm<2> shift, Reg Rm, Imm<6> imm6, Reg Rn, Reg Rd) {
    size_t datasize = sf ? 64 : 32;

    if (shift == 0b11) return ReservedValue();
    if (!sf && imm6.Bit<5>()) return ReservedValue();

    u8 shift_amount = imm6.ZeroExtend<u8>();

    auto operand1 = X(datasize, Rn);
    auto operand2 = ShiftReg(datasize, Rm, shift, ir.Imm8(shift_amount));

    auto result = ir.Add(operand1, operand2);

    X(datasize, Rd, result);

    return true;
}

bool TranslatorVisitor::ADDS_shift(bool sf, Imm<2> shift, Reg Rm, Imm<6> imm6, Reg Rn, Reg Rd) {
    size_t datasize = sf ? 64 : 32;

    if (shift == 0b11) return ReservedValue();
    if (!sf && imm6.Bit<5>()) return ReservedValue();

    u8 shift_amount = imm6.ZeroExtend<u8>();

    auto operand1 = X(datasize, Rn);
    auto operand2 = ShiftReg(datasize, Rm, shift, ir.Imm8(shift_amount));

    auto result = ir.Add(operand1, operand2);

    ir.SetNZCV(ir.NZCVFrom(result));

    X(datasize, Rd, result);

    return true;
}

bool TranslatorVisitor::SUB_shift(bool sf, Imm<2> shift, Reg Rm, Imm<6> imm6, Reg Rn, Reg Rd) {
    size_t datasize = sf ? 64 : 32;

    if (shift == 0b11) return ReservedValue();
    if (!sf && imm6.Bit<5>()) return ReservedValue();

    u8 shift_amount = imm6.ZeroExtend<u8>();

    auto operand1 = X(datasize, Rn);
    auto operand2 = ShiftReg(datasize, Rm, shift, ir.Imm8(shift_amount));

    auto result = ir.Sub(operand1, operand2);

    X(datasize, Rd, result);

    return true;
}

bool TranslatorVisitor::SUBS_shift(bool sf, Imm<2> shift, Reg Rm, Imm<6> imm6, Reg Rn, Reg Rd) {
    size_t datasize = sf ? 64 : 32;

    if (shift == 0b11) return ReservedValue();
    if (!sf && imm6.Bit<5>()) return ReservedValue();

    u8 shift_amount = imm6.ZeroExtend<u8>();

    auto operand1 = X(datasize, Rn);
    auto operand2 = ShiftReg(datasize, Rm, shift, ir.Imm8(shift_amount));

    auto result = ir.Sub(operand1, operand2);

    ir.SetNZCV(ir.NZCVFrom(result));

    X(datasize, Rd, result);

    return true;
}

bool TranslatorVisitor::ADD_ext(bool sf, Reg Rm, Imm<3> option, Imm<3> imm3, Reg Rn, Reg Rd) {
    size_t datasize = sf ? 64 : 32;
    u8 shift = imm3.ZeroExtend<u8>();
    if (shift > 4) return ReservedValue();

    auto operand1 = Rn == Reg::SP ? SP(datasize) : X(datasize, Rn);
    auto operand2 = ExtendReg(datasize, Rm, option, shift);

    auto result = ir.Add(operand1, operand2);

    if (Rd == Reg::SP) {
        SP(datasize, result);
    } else {
        X(datasize, Rd, result);
    }

    return true;
}

bool TranslatorVisitor::ADDS_ext(bool sf, Reg Rm, Imm<3> option, Imm<3> imm3, Reg Rn, Reg Rd) {
    size_t datasize = sf ? 64 : 32;
    u8 shift = imm3.ZeroExtend<u8>();
    if (shift > 4) return ReservedValue();

    auto operand1 = Rn == Reg::SP ? SP(datasize) : X(datasize, Rn);
    auto operand2 = ExtendReg(datasize, Rm, option, shift);

    auto result = ir.Add(operand1, operand2);

    ir.SetNZCV(ir.NZCVFrom(result));

    if (Rd == Reg::SP) {
        SP(datasize, result);
    } else {
        X(datasize, Rd, result);
    }

    return true;
}

bool TranslatorVisitor::SUB_ext(bool sf, Reg Rm, Imm<3> option, Imm<3> imm3, Reg Rn, Reg Rd) {
    size_t datasize = sf ? 64 : 32;
    u8 shift = imm3.ZeroExtend<u8>();
    if (shift > 4) return ReservedValue();

    auto operand1 = Rn == Reg::SP ? SP(datasize) : X(datasize, Rn);
    auto operand2 = ExtendReg(datasize, Rm, option, shift);

    auto result = ir.Sub(operand1, operand2);

    if (Rd == Reg::SP) {
        SP(datasize, result);
    } else {
        X(datasize, Rd, result);
    }

    return true;
}

bool TranslatorVisitor::SUBS_ext(bool sf, Reg Rm, Imm<3> option, Imm<3> imm3, Reg Rn, Reg Rd) {
    size_t datasize = sf ? 64 : 32;
    u8 shift = imm3.ZeroExtend<u8>();
    if (shift > 4) return ReservedValue();

    auto operand1 = Rn == Reg::SP ? SP(datasize) : X(datasize, Rn);
    auto operand2 = ExtendReg(datasize, Rm, option, shift);

    auto result = ir.Sub(operand1, operand2);

    ir.SetNZCV(ir.NZCVFrom(result));

    if (Rd == Reg::SP) {
        SP(datasize, result);
    } else {
        X(datasize, Rd, result);
    }

    return true;
}

bool TranslatorVisitor::ADC(bool sf, Reg Rm, Reg Rn, Reg Rd) {
    size_t datasize = sf ? 64 : 32;

    auto operand1 = X(datasize, Rn);
    auto operand2 = X(datasize, Rm);

    auto result = ir.AddWithCarry(operand1, operand2, ir.GetCFlag());

    X(datasize, Rd, result);

    return true;
}

bool TranslatorVisitor::ADCS(bool sf, Reg Rm, Reg Rn, Reg Rd) {
    size_t datasize = sf ? 64 : 32;

    auto operand1 = X(datasize, Rn);
    auto operand2 = X(datasize, Rm);

    auto result = ir.AddWithCarry(operand1, operand2, ir.GetCFlag());

    ir.SetNZCV(ir.NZCVFrom(result));

    X(datasize, Rd, result);

    return true;
}

bool TranslatorVisitor::SBC(bool sf, Reg Rm, Reg Rn, Reg Rd) {
    size_t datasize = sf ? 64 : 32;

    auto operand1 = X(datasize, Rn);
    auto operand2 = X(datasize, Rm);

    auto result = ir.SubWithCarry(operand1, operand2, ir.GetCFlag());

    X(datasize, Rd, result);

    return true;
}

bool TranslatorVisitor::SBCS(bool sf, Reg Rm, Reg Rn, Reg Rd) {
    size_t datasize = sf ? 64 : 32;

    auto operand1 = X(datasize, Rn);
    auto operand2 = X(datasize, Rm);

    auto result = ir.SubWithCarry(operand1, operand2, ir.GetCFlag());

    ir.SetNZCV(ir.NZCVFrom(result));

    X(datasize, Rd, result);

    return true;
}

} // namespace A64
} // namespace Dynarmic
