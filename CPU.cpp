/*
 CPU.cpp

 Author:
      Neil Stoker <gg6ujj@gmail.com>

 Copyright (c) 2011 Neil Stoker

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU Lesser General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU Lesser General Public License for more details.

 You should have received a copy of the GNU Lesser General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

using namespace std;
#include <cstdio>
#include "CPU.h"
#include "BreakpointManager.h"

void CPU::A(byte newA)
{
    regA=newA;
    setFlagsNZ(newA);
}

bool CPU::N()
{
    return (ps & 0x80)==0x80;
}
bool CPU::V()
{
    return (ps & 0x40)==0x40;
}
bool CPU::B()
{
    return (ps & 0x10)==0x10;
}
bool CPU::D()
{
    return (ps & 0x08)==0x08;
}
bool CPU::I()
{
    return (ps & 0x04)==0x04;
}
bool CPU::Z()
{
    return (ps & 0x02)==0x02;
}
bool CPU::C()
{
    return (ps & 0x01)==0x01;
}

void CPU::N(bool nv)
{
    ps&= (nv | 0x7F);
}
void CPU::V(bool nv)
{
    ps &= (nv | 0xBF);
}
void CPU::B(bool nv)
{
    ps &= (nv | 0xEF);
}
void CPU::D(bool nv)
{
    ps &= (nv | 0xF7);
}
void CPU::I(bool nv)
{
    ps &= (nv | 0xFB);
}
void CPU::Z(bool nv)
{
    ps &= (nv | 0xFD);
}
void CPU::C(bool nv)
{
    ps &= (nv | 0xFE);
}

void CPU::setFlagsNZ(byte op_byte)
{
    N(op_byte);
    Z(op_byte);
}

void CPU::ADC(byte op_byte)
{
    if(D())
    {
        //Decimal mode
        byte temp=0;
        byte result=C()+(regA & 0xF)+(op_byte & 0xF);
        if(result>9)
        {
            temp=1;
            result -=10;
        }
        temp+=((regA>>4)+(op_byte>>4));
        C(false);
        if(temp>9)
        {
            C(true);
            temp-=10;
        }
        V(C());
        A(result+(0x10*temp));
        ++cycle_clock;
    }
    else
    {
        // Binary addition
        byte high_bit=(regA>>7);
        regA+= regA + op_byte + C();
        if(regA & 0x80)
        {
            Z(false);
            N(false);
            if(op_byte & 0x80)
            {
                C(high_bit);
                V(false);
            }
            else
            {
                C(false);
                V(1^high_bit);
            }
        }
        else
        {
            N(false);
            Z(regA?0:1);
            if(op_byte&0x80)
            {
                C(true);
                V(high_bit);
            }
            else
            {
                C(high_bit);
                V(false);
            }
        }
    }
}
void CPU::ADC(address op_addr)
{
    ADC(mem->read(op_addr));
}

void CPU::AND(byte op_byte)
{
    byte aA=regA;
    aA &= op_byte;
    A(aA);
}
void CPU::AND(address op_addr)
{
    AND(mem->read(op_addr));
}
void CPU::ASL(address op_addr)
{
    byte op_byte=mem->read(op_addr);
    C(op_byte);
    setFlagsNZ(op_byte);
    mem->write(op_addr,op_byte);
}
void CPU::BIT(byte op_byte)
{
    V(op_byte);
    N(op_byte);
    Z(op_byte);
}
void CPU::BIT(address op_addr)
{
    BIT(mem->read(op_addr));
}
void CPU::CMP(address op_addr,byte op_register)
{
    byte op_byte=mem->read(op_addr);
    C((op_register>=op_byte)?1:0);
    byte result=op_register - op_byte;
    setFlagsNZ(result);
}
void CPU::DEC(address op_addr)
{
    byte op_byte=mem->read(op_addr)-1;
    setFlagsNZ(op_byte);
    mem->write(op_addr,op_byte);
}
void CPU::EOR(address op_addr)
{
    EOR(mem->read(op_addr));
}
void CPU::EOR(byte op_byte)
{
    A(regA ^ op_byte);
}
void CPU::INC(address op_addr)
{
    byte op_byte=mem->read(op_addr)+1;
    setFlagsNZ(op_byte);
    mem->write(op_addr,op_byte);
}
void CPU::LSR(address op_addr)
{
    byte op_byte=mem->read(op_addr);
    C(op_byte & 0x01);
    op_byte>>=1;
    N(false);
    Z(op_byte ? 0:1);
    mem->write(op_addr,op_byte);
}
void CPU::SBC(byte op_byte)
{
    if(D())
    {
        byte oldA=regA;
        regA-=(op_byte & 0xF)+C()-1;
        if((regA & 0x0F)>(oldA & 0x0F))
            regA-=0x06; // Borrow needed, so change 0x0F to 0x09, 0x0E to 0x08 etc

        regA-=(op_byte & 0xF0);
        if((regA & 0xF0)>(oldA & 0xF0))
            regA-=0x60;

        C((regA>oldA)?0:1);
        V(C());
        setFlagsNZ(regA);
        ++cycle_clock; // Takes longer than binary SBC
    }
    else
    {
        byte high_bit=regA>>7;
        regA-=op_byte + C()-1;
        if(regA & 0x80)
        {
            Z(false);
            N(true);
            if(op_byte & 0x80)
            {
                C(false);
                V(1^high_bit);
            }
            else
            {
                C(high_bit);
                V(false);
            }
        }
        else
        {
            N(false);
            Z(regA?0:1);
            if(op_byte&0x80)
            {
                C(high_bit);
                V(false);
            }
            else
            {
                C(true);
                V(high_bit);
            }
        }
    }
}
void CPU::SBC(address op_addr)
{
    SBC(mem->read(op_addr));
}
void CPU::ORA(byte op_byte)
{
    A(regA | op_byte);
}
void CPU::ORA(address op_addr)
{
    ORA(mem->read(op_addr));
    // TODO (ns#1#): Verify that this is unnecessary
    //setFlagsNZ(regA);
}
void CPU::ROL(address op_addr)
{
    byte op_byte=mem->read(op_addr);
    byte result=(op_byte<<1)|C();
    C((op_byte & 0x80)?1:0);
    setFlagsNZ(result);
    mem->write(op_addr,result);
}
void CPU::ROR(address op_addr)
{
    byte op_byte=mem->read(op_addr);
    byte result=(op_byte>>1);
    result|= (C()?0x80:0x00);
    C(op_byte & 0x01);
    setFlagsNZ(result);
    mem->write(op_addr,result);
}


byte CPU::zp()
{
    byte addr=mem->read(pc++);
    return addr;
}
byte  CPU::zp_x()
{
    /*
    * The address to be accessed by an instruction using indexed zero page addressing is calculated
    * by taking the 8 bit zero page address from the instruction and adding the current value of the
    * X register to it. For example if the X register contains $0F and the instruction LDA $80,X is
    * executed then the ac)cumulator will be loaded from $008F (e.g. $80 + $0F => $8F).
    *
    * NB:
    * The address calculation wraps around if the sum of the base address and the register exceed $FF.
    * If we repeat the last example but with $FF in the X register then the accumulator will be loaded
    * from $007F (e.g. $80 + $FF => $7F) and not $017F.
    */

    zpaddr addr = mem->read(pc) + regX;
//    if(addr>0xFF)
//		addr-= 0x100;
    return addr;
}
byte  CPU::zp_y()
{
    byte addr=mem->read(pc)+regY;
    return addr;
}
address  CPU::zp_indirect()
{
    zpaddr addr=mem->read(pc++);
    address op_addr=(address)mem->read(addr++);
    op_addr+=256*mem->read(addr);
    return op_addr;
}
address  CPU::immediate()
{
    return pc++;
}
address  CPU::indirect_x()
{
    zpaddr addz=mem->read(pc++)+regX;
    address op_addr=mem->read(addz++);
    op_addr+=(256*mem->read(addz));
    return op_addr;
}
address  CPU::indirect_y()
{
    zpaddr addr=mem->read(pc++);
    address op_base=mem->read(addr++);
    address op_addr=mem->read(addr);

    op_addr = op_base + 256*op_addr + regY;

    if((op_base & 0xFF00)!=(op_addr & 0xFF00))
        ++cycle_clock;
    return op_addr;
}
address  CPU::absolute()
{
    address op_addr = mem->read(pc++);
    op_addr+=256*mem->read(pc++);
    return op_addr;
}
address  CPU::absolute_x()
{
    address op_base= absolute();
    address op_addr=op_base+(address)regX;

    if((op_base & 0xFF00)!=(op_addr & 0xFF00))
        ++cycle_clock;

    return op_addr;
}
address  CPU::absolute_y()
{
    address op_base= absolute();
    address op_addr=op_base+(address)regY;

    if((op_base & 0xFF00)!=(op_addr & 0xFF00))
        ++cycle_clock;

    return op_addr;
}
void  CPU::branch()
{
    int branch_offset = (signed char)mem->read(pc++);
    address branch_dest=branch_offset+pc;
    ++cycle_clock;
    if((pc & 0xFF00) !=(branch_dest & 0xFF00))
        ++cycle_clock;
    pc=branch_dest;
}
void CPU::interrupt(address vector,byte flag)
{
    byte s_byte;
    interrupt_flags &=(~flag);

    if(flag==F_RESET)
    {
        B(true);
    }
    else
    {
        s_byte=pc/256;
        push((byte)s_byte);
        s_byte=pc & 0xFF;
        push(s_byte);
        B(false);
        s_byte=getPS();
        push(s_byte);
    }
    I(true);
    D(false);
    pc=mem->read(vector++);
    pc+=(256*mem->read(vector));
    cycle_clock+=7;
}
void CPU::singleStep()
{
    // Perform one action.
    unsigned int op_code;
    if(interrupt_flags!=0)
    {
        if((I()==0)&& (interrupt_flags & F_IRQ))
            interrupt(IRQ_VECTOR,F_IRQ);
        else if(interrupt_flags & F_NMI)
            interrupt(NMI_VECTOR,F_NMI);
        else if(interrupt_flags & F_RESET)
            interrupt(RES_VECTOR, F_RESET);
    }

    // Check for a breakpoint hit

    op_code=mem->read(pc++);

    switch (op_code)
    {
    case 0x00: //BRK
        ++pc;
        push(pc/256);
        push(pc & 0xFF);
        B(true);
        push(regSP);
        I(true);
        D(false);
        pc=mem->read(IRQ_VECTOR);
        pc+=256*mem->read((address)(IRQ_VECTOR +1));
        break;
    case 0x01: // ORA
        ORA(indirect_x());
        break;
    case 0x04: //TSB
    {
        zpaddr addr=zp();
        byte zp_byte=mem->read(addr);
        Z((regA & zp_byte)?0:1);
        zp_byte|=regA;
        mem->write(addr,zp_byte);
        break;
    }
    case 0x05:
        // ORA
    {
        ORA(mem->read(zp()));
        break;

    }
    case 0x06: // ASL
        ASL(mem->read(zp()));
        break;
    case 0x08: // PHP
    {
        push(regSP);
        break;
    }
    case 0x09: // ORA
    {
        ORA(immediate());
        break;
    }
    case 0x0A: // ASL
    {
        ASL((regA & 0x80)?1:0);
        regA<<=1;
        setFlagsNZ(regA);
        break;
    }
    case 0x0C: // TSB
    {
        address op_addr = absolute();
        byte op_byte=mem->read(op_addr);
        Z((regA & op_byte)?0:1);
        op_byte|=regA;
        mem->write(op_addr,op_byte);
        break;
    }
    case 0x0D: // ORA
    {
        ORA(absolute());
        break;
    }
    case 0x0E: // ASL
    {
        ASL(absolute());
        break;
    }
    case 0x10: // BPL
    {
        if(N()) ++ pc;
        else branch();
        break;
    }
    case 0x11: // ORA
    {
        ORA(indirect_y());
        break;
    }
    case 0x12: // ORA
    {
        ORA(zp_indirect());
        break;
    }
    case 0x14: // TRB
    {
        zpaddr addr=zp();
        byte zp_byte=mem->read(addr);
        Z((regA & zp_byte)?0:1);
        zp_byte &= ~regA;
        mem->write(addr,zp_byte);
        break;
    }
    case 0x15: // ORA
    {
        ORA(zp_x());
        break;
    }
    case 0x16: // ASL
    {
        ASL(zp_x());
        break;
    }
    case 0x18: // CLC
    {
        C(0);
        break;
    }
    case 0x19: //ORA
    {
        ORA(absolute_y());
        break;
    }
    case 0x1A: // INC
    {
        ++regA;
        setFlagsNZ(regA);
        break;
    }
    case 0x1C: // TRB
    {
        address op_addr = absolute();
        byte op_byte=mem->read(op_addr);
        Z((regA&op_byte)?0:1);
        op_byte &= ~regA;
        mem->write(op_addr,op_byte);
        break;
    }
    case 0x1D: // ORA
    {
        ORA(absolute_x());
        break;
    }
    case 0x1E: // ASL
    {
        ASL(absolute_x());
        break;
    }
    case 0x20: // JSR
    {
        address addr=pc+1;
        push(addr /256);
        push(addr & 255);
        pc=absolute();
        break;
    }
    case 0x21: //AND
    {
        AND(indirect_x());
        break;
    }
    case 0x24: // BIT
    {
        BIT(zp());
        break;
    }
    case 0x25: // AMD
    {
        AND(zp());
        break;
    }
    case 0x26: // ROL
    {
        ROL(zp());
        break;
    }
    case 0x28: // PLP
    {
        ps=pull();
        break;
    }
    case 0x29: // AND
    {
        AND(immediate());
        break;
    }
    case 0x2A: // ROL
    {
        byte result = (regA<<1)|C();
        C((regA & 0x80)?1:0);
        regA=result;
        setFlagsNZ(regA);
        break;
    }
    case 0x2C: // BIT
    {
        BIT(absolute());
        break;
    }
    case 0x2D: // AND
    {
        AND(absolute());
        break;
    }
    case 0x2E: // ROL
    {
        ROL(absolute());
        break;
    }
    case 0x30: // BMI
    {
        if(N()) branch();
        else pc++;
        break;
    }
    case 0x31: // AND
    {
        AND(indirect_y());
        break;
    }
    case 0x34: // BIT
    {
        BIT(zp_x());
        break;
    }
    case 0x35: // AND
    {
        AND(zp_x());
        break;
    }
    case 0x36: // ROL
    {
        ROL(zp_x());
        break;
    }
    case 0x38: // SEC
    {
        C(true);
        break;
    }
    case 0x39: // AND
    {
        AND(absolute_y());
        break;
    }
    case 0x3A: // DEC
    {
        --regA;
        setFlagsNZ(regA);
        break;
    }
    case 0x3C: // BIT
    {
        BIT(absolute_x());
        break;
    }
    case 0x3D: // AND
    {
        AND(absolute_x());
        break;
    }
    case 0x3E: // ROL
    {
        ROL(absolute_x());
        break;
    }
    case 0x40: // RTI
// TODO (ns#1#): Check this routine - three pulls?
    {
        pc=pull();
        pc=pull();
        pc+=256*pull();
        break;
    }
    case 0x41: // EOR
    {
        EOR(indirect_x());
        break;
    }
    case 0x45: // EOR
    {
        EOR(zp());
        break;
    }
    case 0x46: // LSR
    {
        LSR(zp());
        break;
    }
    case 0x48: // PHA
    {
        push(regA);
        break;
    }
    case 0x49: // EOR
    {
        EOR(immediate());
        break;
    }
    case 0x4A: // LSR
    {
        C(regA & 0x01);
        regA>>=1;
        N(false);
        Z(regA?false:true);
        break;
    }
    case 0x4C: // JMP
    {
        pc=absolute();
        break;
    }
    case 0x4D: // EOR
    {
        EOR(absolute());
        break;
    }
    case 0x4E: // LSR
    {
        LSR(absolute());
        break;
    }
    case 0x50: // BVC
    {
        if(V()) ++pc;
        else branch();
        break;
    }
    case 0x51: // EOR
    {
        EOR(indirect_y());
        break;
    }
    case 0x52: // EOR
    {
        EOR(zp_indirect());
        break;
    }
    case 0x55: // EOR
    {
        EOR(zp_x());
        break;
    }
    case 0x56: // LSR
    {
        LSR(zp_x());
        break;
    }
    case 0x58: // CLI
    {
        I(false);
        break;
    }
    case 0x59: // EOR
    {
        EOR(absolute_y());
        break;
    }
    case 0x5A: // PHY
    {
        push(regY);
        break;
    }
    case 0x5D: // EOR
    {
        EOR(absolute_x());
        break;
    }
    case 0x5E: // LSR
    {
        LSR(absolute_x());
        break;
    }
    case 0x60: // RTS
    {
        pc=pull();
        pc+=256 * pull();
        pc++;
        break;
    }
    case 0x61: // ADC
    {
        ADC(indirect_x());
        break;
    }
    case 0x64: // STZ
    {
        address addr = zp();
        write(addr,0);
        break;
    }
    case 0x65: // ADC
    {
        ADC(zp());
        break;
    }
    case 0x66: // ROR
    {
        ROR(zp());
        break;
    }
    case 0x68: // PLA
    {
        regA=pull();
        break;
    }
    case 0x69: // ADC
    {
        ADC(immediate());
        break;
    }
    case 0x6A: // ROR
    {
        byte result = regA>>1;
        result |= (C() ? 0x80:0x00);
// TODO (ns#1#): Verify - original source had C(cpuA * 0x01);
        C(regA & 0x01);
        A(result);
        break;
    }
    case 0x6C: // JMP
    {
        address addr=absolute();
        pc=mem->read(addr++);
        pc+=256 * mem->read(addr);
        break;
    }
    case 0x6D: // ADC
    {
        ADC(absolute());
        break;
    }
    case 0x6E: // ROR
    {
        ROR(absolute());
        break;
    }
    case 0x70: // BVS
    {
        if(V()) branch();
        else pc++;
        break;
    }
    case 0x71: // ADC
    {
        ADC(indirect_y());
        break;
    }
    case 0x72: // ADC
    {
        ADC(zp_indirect());
        break;
    }
    case 0x74: // STZ
    {
        zpaddr addr=zp_x();
        mem->write(addr,0);
        break;
    }
    case 0x75: // ADC
    {
        ADC(zp_x());
        break;
    }
    case 0x76: // ROR
    {
        ROR(zp_x());
        break;
    }
    case 0x78: // SEI
    {
        I(true);
        break;
    }
    case 0x79: // ADC
    {
        ADC(absolute_y());
        break;
    }
    case 0x7A: // PLY
    {
        regY=pull();
        setFlagsNZ(regY);
        break;
    }
    case 0x7C: // JMP
    {
        address addr=absolute() + (address)regX;
        pc=mem->read(addr++);
        pc+=256*mem->read(addr);
        break;
    }
    case 0x7D: // ADC
    {
        ADC(absolute_x());
        break;
    }
    case 0x7E: // ROR
    {
        ROR(absolute_x());
        break;
    }
    case 0x80: // BRA
    {
        branch();
        break;
    }
    case 0x81: // STA
    {
        address addr=indirect_x();
        mem->write(addr,regA);
        break;
    }
    case 0x84: // STY
    {
        zpaddr addr =zp();
        mem->write(addr,regY);
        break;
    }
    case 0x85: // STA
    {
        zpaddr addr=zp();
        mem->write(addr,regA);
        break;
    }
    case 0x86: // STX
    {
        zpaddr addr=zp();
        mem->write(addr,regX);
        break;
    }
    case 0x88: // DEY
    {
        regY--;
        setFlagsNZ(regY);
        break;
    }
    case 0x89: // BIT nb Different to the normal BIT
    {
        address addr=immediate();
        Z((mem->read(addr) & regA)?0:1);
        break;
    }
    case 0x8A: // TXA
    {
        A(regX);
        break;
    }
    case 0x8C: // STY
    {
        address addr = absolute();
        mem->write(addr,regY);
        break;
    }
    case 0x8D: // STA
    {
        address addr=absolute();
        mem->write(addr,regA);
        break;
    }
    case 0x8E: // STX
    {
        address addr = absolute();
        mem->write(addr,regX);
        break;
    }
    case 0x90: // BCC
    {
        if(C()) pc++;
        else branch();
        break;
    }
    case 0x91: // STA
    {
        address addr = indirect_y();
        mem->write(addr,regA);
        break;
    }
    case 0x92: // STA
    {
        address addr=zp_indirect();
        mem->write(addr,regA);
        break;
    }
    case 0x94: // STY
    {
        zpaddr addr=zp_x();
        mem->write(addr,regY);
        break;
    }
    case 0x95: // STA
    {
        zpaddr addr = zp_x();
        mem->write(addr,regA);
        break;
    }
    case 0x96: // STX
    {
        zpaddr addr=zp_y();
        mem->write(addr,regX);
        break;
    }
    case 0x98: // TYA
    {
        A(regY);
        break;
    }
    case 0x99: // STA
    {
        address addr = absolute_y();
        mem->write(addr,regA);
        break;
    }
    case 0x9A: // TXS
    {
        regSP=regX;
        break;
    }
    case 0x9C: // STZ
    {
        address addr=absolute();
        mem->write(addr,0);
        break;
    }
    case 0x9D: // STA
    {
        address addr=absolute_x();
        mem->write(addr,regA);
        break;
    }
    case 0x9E: // STZ
    {
        mem->write(absolute_x(),0);
        break;
    }
    case 0xA0: // LDY
    {
        regY=mem->read(immediate());
        setFlagsNZ(regY);
        break;
    }
    case 0xA1: // LDA
    {
        A(mem->read(indirect_x()));
        break;
    }
    case 0xA2: // LDX
    {
        regX=mem->read(immediate());
        setFlagsNZ(regX);
        break;
    }
    case 0xA4: // LDY
    {
        regY=mem->read(zp());
        setFlagsNZ(regY);
        break;
    }
    case 0xA5: // LDA
    {
        A(mem->read(zp()));
        break;
    }
    case 0xA6: // LDX
    {
        regX=mem->read(zp());
        setFlagsNZ(regX);
        break;
    }
    case 0xA8: // TAY
    {
        regY=regA;
        setFlagsNZ(regY);
        break;
    }
    case 0xA9: // LDA
    {
        A(mem->read(immediate()));
        break;
    }
    case 0xAA: // TAX
    {
        regX=regA;
        setFlagsNZ(regX);
        break;
    }
    case 0xAC: // LDY
    {
        regY=mem->read(absolute());
        setFlagsNZ(regY);
        break;
    }
    case 0xAD: // LDA
    {
        A(mem->read(absolute()));
        break;
    }
    case 0xAE: // LDX
    {
        regX=mem->read(absolute());
        setFlagsNZ(regX);
        break;
    }
    case 0xB0: // BCS
    {
        if(C()) branch();
        else pc++;
        break;
    }
    case 0xB1: // LDA
    {
        A(mem->read(indirect_y()));
        break;
    }
    case 0xB2: // LDA
    {
        A(mem->read(zp_x()));
        break;
    }
    case 0xB4: // LDY
    {
        regY=mem->read(zp_x());
        break;
    }
    case 0xB5: // LDA
    {
        A(mem->read(zp_x()));
        break;
    }
    case 0xB6: // LDX
    {
        regX=mem->read(zp_x());
        setFlagsNZ(regX);
        break;
    }
    case 0xB8: // CLV
    {
        V(false);
        break;
    }
    case 0xB9: // LDA
    {
        A(mem->read(absolute_y()));
        break;
    }
    case 0xBA: // TSX
    {
        regX=regSP;
        setFlagsNZ(regX);
        break;
    }
    case 0xBC: // LDY
    {
        regY=mem->read(absolute_x());
        setFlagsNZ(regY);
        break;
    }
    case 0xBD: // LDA
    {
        A(mem->read(absolute_x()));
        break;
    }
    case 0xBE: // LDX
    {
        regX=mem->read(absolute_y());
        setFlagsNZ(regX);
        break;
    }
    case 0xC0: // CPY
    {
        CMP(regY,immediate());
        break;
    }
    case 0xC1: // CMP
    {
        CMP(regA,indirect_x());
        break;
    }
    case 0xC4: // CPY
    {
        CMP(regY,zp());
        break;
    }
    case 0xC5: // CMP
    {
        CMP(regA,zp());
        break;
    }
    case 0xC6: // DEC
    {
        DEC(zp());
        break;
    }
    case 0xC8: // INY
    {
        regY++;
        setFlagsNZ(regY);
        break;
    }
    case 0xC9: // CMP
    {
        CMP(regA,immediate());
        break;
    }
    case 0xCA: // DEX
    {
        regX--;
        setFlagsNZ(regX);
        break;
    }
    case 0xCC: // CPY
    {
        CMP(regY,absolute());
        break;
    }
    case 0xCD: // CMP
    {
        CMP(regA,absolute());
        break;
    }
    case 0xCE: // DEC
    {
        DEC(absolute());
        break;
    }
    case 0xD0: // BNE
    {
        if(Z()) ++pc;
        else branch();
        break;
    }
    case 0xD1: // CMP
    {
        CMP(regA,indirect_y());
        break;
    }
    case 0xD2: // CMP
    {
        CMP(regA,zp_indirect());
        break;
    }
    case 0xD5: // CMP
    {
        CMP(regA,zp_x());
        break;
    }
    case 0xD6: // DEC
    {
        DEC(zp_x());
        break;
    }
    case 0xD8: // CLD
    {
        D(false);
        break;
    }
    case 0xD9: // CMP
    {
        CMP(regA,absolute_y());
        break;
    }
    case 0xDA: // PHX
    {
        push(regX);
        break;
    }
    case 0xDD: // CMP
    {
        CMP(regA,absolute_x());
        break;
    }
    case 0xDE: // DEC
    {
        CMP(regA,absolute_x());
        break;
    }
    case 0xE0: // CPX
    {
        CMP(regX,immediate());
        break;
    }
    case 0xE1: // SBC
    {
        SBC(indirect_x());
        break;
    }
    case 0xE4: // CPX
    {
        CMP(regX,zp());
        break;
    }
    case 0xE5: // SBC
    {
        SBC(zp());
        break;
    }
    case 0xE6: // INC
    {
        INC(zp());
        break;
    }
    case 0xE8: // INX
    {
        regX++;
        setFlagsNZ(regX);
        break;
    }
    case 0xE9: //SBC
    {
        SBC(immediate());
        break;
    }
    case 0xEA: //NOP
    {
        // Do nothing. Hard to get this instruction to malfunction...
        break;
    }
    case 0xEC: // CPX
    {
        CMP(regX,absolute());
        break;
    }
    case 0xED: // SBC
    {
        SBC(absolute());
        break;
    }
    case 0xEE: // INC
    {
        INC(absolute());
        break;
    }
    case 0xF0: // BEQ
    {
        if(Z())branch();
        else ++pc;
        break;
    }
    case 0xF1: // SBC
    {
        SBC(indirect_y());
        break;
    }
    case 0xF2: // SBC
    {
        SBC(zp_indirect());
        break;
    }
    case 0xF5: // SBC
    {
        SBC(zp_x());
        break;
    }
    case 0xF6: // INC
    {
        INC(zp_x());
        break;
    }
    case 0xF8: // SED
    {
        D(true);
        break;
    }
    case 0xF9: // SBC
    {
        SBC(absolute_y());
        break;
    }
    case 0xFA: // PLX
    {
        regX=pull();
        setFlagsNZ(regX);
        break;
    }
    case 0xFD: // SBC
    {
        SBC(absolute_x());
        break;
    }
    case 0xFE: // INC
    {
        INC(absolute_x());
        break;
    }
    default:
        // Unknown instruction, going to treat as a NOP, but add a log line.
        fprintf(stderr,"Unexpected instruction at PC 0x%x 0x%x\n",pc,op_code);
        break;
    }

// TODO (ns#1#): Implement the counter for the number of cycles an instruction takes
    //cycle_clock+= instruction_cycles[opcode];
    ++num_instructions;
}

void CPU::run()
{
// TODO (g6ujj#1#): Need to implement breakpoints
    bool breakPointSet=false;
    long stopAt=50000;
    while(!breakPointSet)
    {
        singleStep();
        if(stopAt--==0)
            break;
    }
}

void CPU::push(byte b)
{
    address a=0x100+regSP;
    mem->write(a,b);
    regSP--;
}

byte CPU::pull(void)
{
    regSP++;
    address a=0x100+regSP;
    byte rv=mem->read(a);
    return rv;
}
