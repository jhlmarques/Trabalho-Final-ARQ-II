
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
    PUSH_R,
    TOP,
    MOV,
    JMP,
    JMP_R,
    JZ,
    JZ_R,
    JNZ,
    JNZ_R,
    JN,
    JN_R
};

#define DEFAULT_REG 0

//Flags
#define FLAG_ZERO 1
#define FLAG_NEGATIVE 2


//Error codes
#define ERROR_SYNTAX 0
#define ERROR_INVALID_INSTRUCTION 1
#define ERROR_INVALID_ARGUMENT 2
#define ERROR_POP_EMPTY_STACK 3
#define ERROR_PUSH_FULL_STACK 4
#define ERROR_NOT_ENOUGH_PARAMETERS 5
#define ERROR_INCORRECT_PARAMETER_TYPE 6
#define ERROR_JUMP_OUT_OF_BOUNDS 7
