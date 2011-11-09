/*
 CPU.h

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
#ifndef CPU_H
#define CPU_H

using namespace std;

#include "Memory.h"
#include "CPU.h"

#define F_RESET  0x1
#define F_IRQ    0x2
#define F_NMI    0x4
#define IRQ_VECTOR (address)0xfffe
#define NMI_VECTOR (address)0xfffa
#define RES_VECTOR (address)0xfffc

class CPU: public Memory
{
public:
    CPU(Memory *theRam):mem(theRam),regA(0),regX(0),regY(0),regSP(0xFF),
        pc(RES_VECTOR),ps(0x34),frequency(1000000),cycle_clock(0),
        num_instructions(0),interrupt_flags(0) {};
    CPU(const CPU&);
    CPU& operator=(const CPU&);
    virtual ~CPU() {};

    void singleStep();
    void run();

    byte getA()
    {
        return regA;
    };
    byte getX()
    {
        return regX;
    };
    byte getY()
    {
        return regY;
    };
    address getPC()
    {
        return pc;
    };
protected:
private:
    Memory *mem;

    byte regA; // Accumulator
    byte regX; // X register
    byte regY; // Y register
    stack regSP; // Stack Pointer
    address pc; // Program counter
    unsigned char ps;
    unsigned long frequency;
    unsigned long long cycle_clock;
    unsigned long long num_instructions;
    int interrupt_flags;
    // Register operations
    void A(byte newA);

    // Processor Status flags
    void setFlagsNZ(byte reg);
    bool N();
    bool V();
    bool B();
    bool D();
    bool I();
    bool Z();
    bool C();
    unsigned char getPS(void)
    {
        return ps;
    }

    void N(bool nN);
    void V(bool nN);
    void B(bool nN);
    void D(bool nN);
    void I(bool nN);
    void Z(bool nN);
    void C(bool nN);

    void interrupt(address vector,byte flag);

    // Addressing modes
    address indirect_x();
    byte zp();
    byte zp_x();
    byte zp_y();
    address zp_indirect();
    address immediate();
    address indirect_y();
    address absolute();
    address absolute_x();
    address absolute_y();
    void branch();

    // Main instructions
    void ADC(byte op_byte);
    void ADC(address op_addr);
    void AND(byte op_byte);
    void AND(address op_addr);
    void ASL(address op_addr);
    void BIT(byte op_byte);
    void BIT(address op_addr);
    void CMP(address op_addr,byte op_byte);
    void DEC(address op_addr);
    void EOR(address op_addr);
    void EOR(byte op_byte);
    void INC(address op_addr);
    void LSR(address op_addr);
    void SBC(address op_addr);
    void SBC(byte op_byte);
    void ORA(address op_addr);
    void ORA(byte op_byte);
    void ROL(address op_addr);
    void ROR(address op_addr);

    // Addressing modes
    void push(byte b);
    byte pull();

};

#endif // CPU_H
