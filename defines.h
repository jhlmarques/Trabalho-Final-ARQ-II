//Instructions with two operands which are in the stack
#define ADD 1
#define SUB 2
#define MUL 3
#define DIV 4
#define MOD 5
//Instructions with one operand which is in the stack
#define NOT 6
#define OR 7
#define AND 8
#define MIR 9
#define OUT 10
#define POP 11
//Instructions with one operand
#define PUSH 12


//Error codes
#define ERROR_SYNTAX 0
#define ERROR_INVALID_INSTRUCTION 1
#define ERROR_INVALID_ARGUMENT 2
#define ERROR_POP_EMPTY_STACK 3
#define ERROR_PUSH_FULL_STACK 4