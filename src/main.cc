/*
 * @file    main.cc
 * @author  Sebastien LEGRAND
 *
 * @brief   Main entry point
 */

// ----- includes
#include "forth_vm.h"

#include <iostream>


// ----- main
int main(int argc, char* argv[]) {
    ForthVM forth;
    std::string input;

    // look for a file
    if (argc == 2) {
        forth.load(argv[1]);
        return 0;
    }

    // launch the interpreter instead
    std::cout << "Forth Interpreter. Enter 'exit' to quit.\n";

    // mainloop
    while (true) {
        std::cout << "> ";
        std::getline(std::cin, input);

        if (input == "exit")
            break;

        forth.run(input);
        // forth.printStack();
        std::cout << "OK\n";
    }

    return 0;
}
