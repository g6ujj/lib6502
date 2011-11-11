#ifndef BREAKPOINT_H
#define BREAKPOINT_H
#include "Memory.h"


typedef unsigned int bpFlags_t;
static const bpFlags_t bp_A = 0x1;
static const bpFlags_t bp_X = 0x2;
static const bpFlags_t bp_Y = 0x4;
static const bpFlags_t bp_S = 0x8;
static const bpFlags_t bp_P = 0x10;
static const bpFlags_t bp_PC = 0x20;
static const bpFlags_t bp_mem = 0x40;
static const bpFlags_t bp_operand = 0xff; // To split operand and condition flags
static const bpFlags_t bp_equal = 0x100;
static const bpFlags_t bp_notequal = 0x200;
static const bpFlags_t bp_less = 0x400;
static const bpFlags_t bp_greater = 0x800;
static const bpFlags_t bp_condition = 0xf00; // To split operand and condition flags
static const bpFlags_t bp_automatic = 0x8000;


class Breakpoint
{
    public:
        /** Default constructor */
        Breakpoint(): enabled(false), bVal(0), wVal(0),flags(0) {};
        /** Default destructor */
        virtual ~Breakpoint();
        /** Copy constructor
         *  \param other Object to copy from
         */
        Breakpoint(const Breakpoint& other):enabled(other.enabled),bVal(other.bVal),wVal(other.wVal),flags(other.flags) {};
        /** Assignment operator
         *  \param other Object to assign from
         *  \return A reference to this
         */
        Breakpoint& operator=(const Breakpoint& other);
        bool operator==(const Breakpoint& other) const;
        bool operator!=(const Breakpoint& other) const;

        Breakpoint(address addr):enabled(true),bVal(0),wVal(addr),flags(bp_PC | bp_equal){};
        Breakpoint(address, byte b);
        void enable() {enabled=true;}
        void disable() {enabled=false;}
        bool isEnabled(){return enabled;}
        bool isAutomatic(){return wVal;}
        bpFlags_t getOperandFlags(){return flags & bp_operand;}
        bpFlags_t getConditionFlags(){return flags & bp_condition;}
        bool Equals(bpFlags_t flags, address addr, byte aByte);
    protected:
    private:
    bool enabled;
    unsigned char bVal;
    address wVal;
    bpFlags_t flags;
};

#endif // BREAKPOINT_H
