#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <utility>
#include <vector>
#include "defines.h"


using namespace std;


//Stack Machine
//

class StackMachine{
    private:
        unsigned int instruction_pointer = 0;
        unsigned int stack_pointer = 0;
        short int reg; //R register
        short int stack[128]; //Stack

        vector<pair<int, int>> instructions; //Instructions loaded from the current file

        int error_code = -1; //Current error code
        int error_line = -1;

        bool _add();
        bool _sub();
        bool _mul();
        bool _div();
        bool _mod();
        bool _not();
        bool _or();
        bool _and();
        bool _mir();
        bool _push();
        bool _push_r();
        bool _pop();
        bool _out();


    public:
        bool load_instructions(const char* filename);
        bool execute_instructions();
        string get_error_message();

};

// Arithmetic operations

bool StackMachine::_add(){
    if (stack_pointer >= 2){
        reg = stack[stack_pointer-1] + stack[stack_pointer-2];
        return true;
    }
    else{
        error_code = ERROR_NOT_ENOUGH_PARAMETERS;
        error_line = instruction_pointer + 1;
        return false;
    }
}

bool StackMachine::_sub(){
    if (stack_pointer >= 2){
        reg = stack[stack_pointer-1] - stack[stack_pointer-2];
        return true;
    }
    else{
        error_code = ERROR_NOT_ENOUGH_PARAMETERS;
        error_line = instruction_pointer + 1;
        return false;
    }
}

bool StackMachine::_mul(){
    if (stack_pointer >= 2){
        reg = stack[stack_pointer-1] * stack[stack_pointer-2];
        return true;
    }
    else{
        error_code = ERROR_NOT_ENOUGH_PARAMETERS;
        error_line = instruction_pointer + 1;
        return false;
    }
}

bool StackMachine::_div(){
    if (stack_pointer < 2){
        error_code = ERROR_NOT_ENOUGH_PARAMETERS;
        error_line = instruction_pointer + 1;
        return false;
    }
    else if (stack[stack_pointer-1] == 0){ // Denominator is 0
        error_code = ERROR_INVALID_ARGUMENT;
        error_line = instruction_pointer + 1;
        return false;
    }
    else{
        reg = stack[stack_pointer-1] / stack[stack_pointer-2];
        return true;
    }
}

bool StackMachine::_mod(){
    if (stack_pointer < 2){
        error_code = ERROR_NOT_ENOUGH_PARAMETERS;
        error_line = instruction_pointer + 1;
        return false;
    }
    else if (stack[stack_pointer-1] == 0){ // Denominator is 0
        error_code = ERROR_INVALID_ARGUMENT;
        error_line = instruction_pointer + 1;
        return false;
    }
    else{
        reg = stack[stack_pointer-1] % stack[stack_pointer-2];
        return true;
    }
}

bool StackMachine::_out(){
    if (stack_pointer < 1){
        error_code = ERROR_NOT_ENOUGH_PARAMETERS;
        error_line = instruction_pointer + 1;
        return false;
    }
    else{
        cout << "Top of the stack: " << stack[stack_pointer-1] << endl;
        return true;
    }

}

// Logic operations

bool StackMachine::_not(){
    if (stack_pointer < 1){
        error_code = ERROR_NOT_ENOUGH_PARAMETERS;
        error_line = instruction_pointer + 1;
        return false;
    }
    else{
        reg = ~stack[stack_pointer-1];
        return true;
    }
}

bool StackMachine::_and(){
    if (stack_pointer < 2){
        error_code = ERROR_NOT_ENOUGH_PARAMETERS;
        error_line = instruction_pointer + 1;
        return false;
    }
    else{
        reg = stack[stack_pointer-1] & stack[stack_pointer-2];
        return true;
    }
}

bool StackMachine::_or(){
    if (stack_pointer < 2){
        error_code = ERROR_NOT_ENOUGH_PARAMETERS;
        error_line = instruction_pointer + 1;
        return false;
    }
    else{
        reg = stack[stack_pointer-1] | stack[stack_pointer-2];
        return true;
    }
}

bool StackMachine::_mir(){
    if (stack_pointer < 1){
        error_code = ERROR_NOT_ENOUGH_PARAMETERS;
        error_line = instruction_pointer + 1;
        return false;
    }
    else{
        unsigned int rev = 0;
        unsigned int temp = stack[stack_pointer-1];
        while (temp > 0){
            // bitwise left shift
            rev <<= 1;
            // if current bit is '1'
            if (temp & 1 == 1)
                rev ^= 1;
            // bitwise right shift
            temp >>= 1;
            }
        reg = rev;
        return true;
    }
}
// Execute the instructions of the instruction vector
bool StackMachine::execute_instructions(){
    bool success;
    for (instruction_pointer; instruction_pointer < instructions.size(); instruction_pointer++){
        switch(instructions[instruction_pointer].first){
            case ADD:
                success = _add();
                break;
            case SUB:
                success = _sub();
                break;
            case MUL:
                success = _mul();
                break;
            case DIV:
                success = _div();
                break;
            case MOD:
                success = _mod();
                break;
            case NOT:
                success = _not();
                break;
            case OR:
                success = _or();
                break;
            case AND:
                success = _and();
                break;
            case MIR:
                success = _mir();
                break;
            case OUT:
                success = _out();
                break;
//            case POP:
//                success = _pop();
//                break;
//            case PUSH:
//                success = _push();
//                break;
//            case PUSH_R:
//                success = _push_r();
//                break;
        }
        if (!success)
            return false;
    }
    return true;
}

//Returns a string containg the latest error message
string StackMachine::get_error_message(){
    stringstream ss;
    ss << "At line " << error_line << ": ";

    switch(error_code){
        case ERROR_SYNTAX:
            ss << "Invalid syntax.";
            break;
        case ERROR_INVALID_INSTRUCTION:
            ss << "Invalid instruction.";
            break;
        case ERROR_INVALID_ARGUMENT:
            ss << "Invalid argument.";
            break;
        case ERROR_POP_EMPTY_STACK:
            ss << "Tried to pop an empty stack.";
            break;
        case ERROR_PUSH_FULL_STACK:
            ss << "Tried to push to a full stack.";
            break;
       case ERROR_NOT_ENOUGH_PARAMETERS:
            ss << "Not enough input parameters.";
            break;
    }

    return ss.str();
}

//Loads instructions from a file
bool StackMachine::load_instructions(const char* filename){
    ifstream file;
    file.open(filename);
    string line;
    string s_inst, s_param; //Substrings for lines with an instruction that has a parameter
    int inst, param;
    int line_counter = 1;
    int param_separator; //Does this instruction come with a parameter?
    bool is_only_digits; //Does the parameter only contain digits

    while(getline(file, line)){
        param_separator = line.find(' ');

        if(param_separator >= line.length()){ //No parameter
            param = 0;
            if(line == "ADD"){
                inst = ADD;
            }
            else if(line == "SUB"){
                inst = SUB;
            }
            else if(line == "MUL"){
                inst = MUL;
            }
            else if(line == "DIV"){
                inst = DIV;
            }
            else if(line == "MOD"){
                inst = MOD;
            }
            else if(line == "NOT"){
                inst = NOT;
            }
            else if(line == "OR"){
                inst = OR;
            }
            else if(line == "AND"){
                inst = AND;
            }
            else if(line == "MIR"){
                inst = MIR;
            }
            else if(line == "POP"){
                inst = POP;
            }
            else if(line == "OUT"){
                inst = OUT;
            }
            else if(line == "PUSH"){
                error_code = ERROR_SYNTAX;
                error_line = line_counter;
                return false;
            }
            else{
                error_code = ERROR_INVALID_INSTRUCTION;
                error_line = line_counter;
                return false;
            }
        }
        else{ //Has parameter (argument)
            s_inst = line.substr(0, param_separator);
            s_param = line.substr(param_separator+1);

            //Check for syntax errors
            if(s_inst == "ADD" || s_inst == "SUB" || s_inst == "MUL" || s_inst == "DIV" || s_inst == "MOD" || s_inst == "NOT" ||\
            s_inst == "OR" || s_inst == "AND" || s_inst == "MIR" || s_inst == "POP" || s_inst == "OUT"){
                error_code = ERROR_SYNTAX;
                error_line = line_counter;
                return false;
            }

            if(s_inst == "PUSH"){
                if(s_param == "$R"){
                    param = 0;
                    inst = PUSH_R;
                }
                else{
                    //Check if the string only has digits
                    is_only_digits = (s_param.find_first_not_of( "0123456789" ) == string::npos);

                    if(!is_only_digits){
                        error_code = ERROR_INVALID_ARGUMENT;
                        error_line = line_counter;
                        return false;
                    }
                    inst = PUSH;
                    param = stol(s_param);
                }
            }
            else{
                error_code = ERROR_INVALID_ARGUMENT;
                error_line = line_counter;
                return false;
            }

        }

        //Add the new instruction
        instructions.push_back(make_pair(inst, param));
        cout << inst << " " << param << endl;

        line_counter++;
    }

    file.close();

    return true;
}

int main(){
    StackMachine SM;

    bool result = SM.load_instructions("test2.txt");
    if (!result){
        cout << SM.get_error_message() << endl;
    }
    bool success = SM.execute_instructions();
    if (!success)
        cout << SM.get_error_message() << endl;
    return 0;
}
