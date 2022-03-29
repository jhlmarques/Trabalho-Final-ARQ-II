#include <iostream>
#include <fstream>
#include <string>
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
        bool load_instructions(char* filename);

};

int main(){
    
}
