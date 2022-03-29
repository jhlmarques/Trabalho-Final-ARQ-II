
enum Instructions{
    ADD,
    SUB,
    MUL,
    DIV,
    MOD,
    NOT,
    OR,
    AND,
    MIR,
    OUT,
    POP,
    PUSH,
    PUSH_R
};


//Error codes
#define ERROR_SYNTAX 0
#define ERROR_INVALID_INSTRUCTION 1
#define ERROR_INVALID_ARGUMENT 2
#define ERROR_POP_EMPTY_STACK 3
#define ERROR_PUSH_FULL_STACK 4