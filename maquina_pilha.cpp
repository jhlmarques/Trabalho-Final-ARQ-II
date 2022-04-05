#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <utility>
#include <map>
#include <vector>
#include "defines.h"

#define DEBUG


using namespace std;

//Dataclass containing info for the instruction parser
struct InstructionData{
    int code;               
    int n_params;           //How many parameters does it have
    bool accept_regs;       //Does it accept registers as a parameter?
    bool accept_values;     //Does it accept values as a parameter

    InstructionData(int code, int n_params, bool accept_regs, bool accept_values)
        : code{code}
        , n_params{n_params}
        , accept_regs{accept_regs}
        , accept_values{accept_values}
        {}

};


//Stack Machine
//

class StackMachine{
    private:
        unsigned int instruction_pointer = 0;
        unsigned int stack_pointer = 0;
        short int reg; //R register
        short int stack[128]; //Stack

        map<string, InstructionData> map_instructions; //The machine's allowed instructions


        vector<pair<int, vector<int>>> instructions; //Instructions loaded from the current file

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
        bool _top();

    public:
        StackMachine(){
            //Fills the instructions map
            map_instructions.insert(pair<string, InstructionData>("ADD",    InstructionData(ADD, 0, false, false)));
            map_instructions.insert(pair<string, InstructionData>("SUB",    InstructionData(SUB, 0, false, false)));
            map_instructions.insert(pair<string, InstructionData>("MUL",    InstructionData(MUL, 0, false, false)));
            map_instructions.insert(pair<string, InstructionData>("DIV",    InstructionData(DIV, 0, false, false)));
            map_instructions.insert(pair<string, InstructionData>("MOD",    InstructionData(MOD, 0, false, false)));
            map_instructions.insert(pair<string, InstructionData>("NOT",    InstructionData(NOT, 0, false, false)));
            map_instructions.insert(pair<string, InstructionData>("OR",     InstructionData(OR, 0, false, false)));
            map_instructions.insert(pair<string, InstructionData>("AND",    InstructionData(AND, 0, false, false)));
            map_instructions.insert(pair<string, InstructionData>("MIR",    InstructionData(MIR, 0, false, false)));
            map_instructions.insert(pair<string, InstructionData>("POP",    InstructionData(POP, 0, false, false)));
            map_instructions.insert(pair<string, InstructionData>("OUT",    InstructionData(OUT, 0, false, false)));
            map_instructions.insert(pair<string, InstructionData>("PUSH",   InstructionData(PUSH, 1, true, true)));
            map_instructions.insert(pair<string, InstructionData>("TOP",   InstructionData(PUSH, 0, false, false)));
        }


        bool load_instructions(const char* filename);
        bool execute_instructions();
        string get_error_message();
};

// Control operations

bool StackMachine::_pop()
{
    if (stack_pointer >= 1)
    {
        stack_pointer--;
        stack[stack_pointer] = 0;
        return true;
    }
    else
    {
        error_code = ERROR_POP_EMPTY_STACK;
        error_line = instruction_pointer + 1;
        return false;
    }
}

bool StackMachine::_push()
{
    if (stack_pointer <= 127)
    {
        stack[stack_pointer] = instructions[instruction_pointer].second[0];
        stack_pointer++;
        return true;
    }
    else
    {
        error_code = ERROR_PUSH_FULL_STACK;
        error_line = instruction_pointer + 1;
        return false;
    }
}

bool StackMachine::_push_r()
{
    if (stack_pointer <= 127)
    {
        stack[stack_pointer] = reg;
        stack_pointer++;
        return true;
    }
    else
    {
        error_code = ERROR_PUSH_FULL_STACK;
        error_line = instruction_pointer + 1;
        return false;
    }
}

bool StackMachine::_top()
{
    if (stack_pointer >= 1)
    {
        reg = stack[stack_pointer-1];
        return true;
    }
    else
    {
        error_code = ERROR_NOT_ENOUGH_PARAMETERS;
        error_line = instruction_pointer + 1;
        return false;
    }
}

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
            case POP:
                success = _pop();
                break;
            case PUSH:
                success = _push();
                break;
            case PUSH_R:
                success = _push_r();
                break;
            case TOP:
                success = _top();
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
        case ERROR_INCORRECT_PARAMETER_TYPE:
            ss << "One or more parameters have conflicting types.";
            break;

    }

    return ss.str();
}

//Loads instructions from a file
bool StackMachine::load_instructions(const char* filename){
    ifstream file;
    file.open(filename);
    string line;
    string line_substr;
    
    string parsed_instruction; //Instruction found in a line
    vector<string> parsed_parameters; //Parameters found in a line

    int line_counter = 1;
    int param_separator; //Ending position of a parameter
    bool param_is_reg; //The parameter is a register

    while(getline(file, line)){
        parsed_parameters.clear();
        
        param_separator = line.find(' ');
        if(param_separator < line.length()){
            parsed_instruction = line.substr(0, param_separator); //Get the instruction
            line_substr = line.substr(param_separator+1); //Work with the rest of the string
            
            while((param_separator = line_substr.find(' ')) < line_substr.length()){
                parsed_parameters.push_back(line_substr.substr(0, param_separator)); //Get the parameters
                line_substr = line_substr.substr(param_separator+1);
            }
            parsed_parameters.push_back(line_substr);

        }
        else{
            parsed_instruction = line;
        }

        //Try to get this instruction's data
        try{
            InstructionData& cur_instruction = map_instructions.at(parsed_instruction);
            if(parsed_parameters.size() != cur_instruction.n_params){ //Incorrect parameter amount
                error_code = ERROR_SYNTAX;
                error_line = line_counter;
                return false;
            }

            vector<int> instruction_parameters; //Parameters to be passed to the instruction

            if(parsed_parameters.size()){
                param_is_reg = (parsed_parameters[0][0] == '$'); //First parameter determines the type of all parameters in the line
            
                //The instruction doesn't allow this type of parameter
                if(!cur_instruction.accept_regs && param_is_reg || !cur_instruction.accept_values && !param_is_reg){
                    error_code = ERROR_INVALID_ARGUMENT;
                    error_line = line_counter;
                    return false;
                }

                //Convert the parameter strings to integers
                for(string x : parsed_parameters){
                    if(param_is_reg){
                        if(x[0] != '$'){
                            error_code = ERROR_INCORRECT_PARAMETER_TYPE;
                            error_line = line_counter;
                            return false;
                        }
                        if(x.length() > 2){
                            error_code = ERROR_INVALID_ARGUMENT;
                            error_line = line_counter;
                            return false;
                        }
                        instruction_parameters.push_back(static_cast<int>(x[1])); //Converts the register's character to int
                    }
                    else{
                        if(x.find_first_not_of( "0123456789" ) != string::npos){
                            error_code = ERROR_INCORRECT_PARAMETER_TYPE;
                            error_line = line_counter;
                            return false;
                        }
                        instruction_parameters.push_back(stoi(x)); //Converts the parameter to a number
                    }
                }
            }

            //Finally, add the new instruction
            //If the instruction accepts both values and registers, the "register version"'s code is equal to the
            //instruction's code plus one.n
            int instruction_code = param_is_reg && cur_instruction.accept_regs && \
                                    cur_instruction.accept_values ? cur_instruction.code + 1 : cur_instruction.code;
            
            instructions.push_back(make_pair(instruction_code, instruction_parameters));
    
            #ifdef DEBUG
            cout << instruction_code << " ";
            for (auto x : instruction_parameters){
                cout << x << " ";
            }
            cout << endl;
            #endif

        }
        catch(out_of_range){
            error_code = ERROR_INVALID_INSTRUCTION;
            error_line = line_counter;
            return false;
        }

        line_counter++;
    }

    file.close();

    return true;
}

int main(){
    StackMachine SM;

    bool result = SM.load_instructions("test.txt");
    if (!result){
        cout << SM.get_error_message() << endl;
    }
    bool success = SM.execute_instructions();
    if (!success)
        cout << SM.get_error_message() << endl;
    return 0;
}
