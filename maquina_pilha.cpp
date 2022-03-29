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
        unsigned int instruction_pointer;
        unsigned int stack_pointer;
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
        bool _pop();
        bool _out();

    public:
        bool load_instructions(const char* filename);
        string get_error_message();

};

//Returns a string containg the latest error message
string StackMachine::get_error_message(){
    stringstream ss;
    ss << "At line " << error_line << " ";
    
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
            ss << "Tried to pop and empty stack.";
            break;
        case ERROR_PUSH_FULL_STACK:
            ss << "Tried to push to a full stack.";
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
            if(line == "ADD" || line == "SUB" || line == "MUL" || line == "DIV" || line == "MOD" || line == "NOT" ||\
            line == "OR" || line == "AND" || line == "MIR" || line == "PUSH" || line == "POP" || line == "OUT"){
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
    /*
    bool result = SM.load_instructions("test.txt");
    if (!result){
        cout << SM.get_error_message() << endl;
    }
    */

}
