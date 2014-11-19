#ifndef HW9_H
#define HW9_H
#include <stdint.h>
#include <stdlib.h>

// Types used for pretty print
#define LOOK_1_REG 0x0
#define LOOK_2_REG 0x1
#define LOOK_2_REG_IMMED 0x2
#define LOOK_3_REG 0x3
#define LOOK_MEM 0x4
#define LOOK_SYSCALL 0x5
#define LOOK_JUMP 0x6
#define LOOK_1_REG_IMMED 0x7
#define LOOK_SHIFT 0x8
#define LOOK_BRANCH 0x9

struct Instr {
        uint32_t uid; /* Unique identification bits */
        uint32_t pretty; /* The type of pretty print format this Instr is */
        char *mnemonic; /* The human readable mnemonic */
        struct Instr *next; /* Pointer for doubly linked list */
        struct Instr *prev; /* Pointer for doubly linked list */
};

struct InstrType {
        char type; /* Should be R, I, or J */
        size_t count; /* Length of Instr list */
        struct InstrType *next; /* Points to the next InstrType node */
        struct InstrType *prev; /* Points to the next InstrType node */
        struct Instr *head; /* Points to the first Instr node of this type */
};

typedef struct Instr Instr;
typedef struct InstrType InstrType;

#endif
