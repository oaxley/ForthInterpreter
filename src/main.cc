/*
 * @file    main.cc
 * @author  Sebastien LEGRAND
 *
 * @brief   Main entry point
 */

// ----- includes
#include "ForthInterpreter.h"

#include <iostream>


// ----- main
int main() {
    ForthInterpreter forth;
    std::string input;

    std::cout << "Forth Interpreter. Enter 'exit' to quit.\n";

    while (true) {
        std::cout << "> ";
        std::getline(std::cin, input);

        if (input == "exit")
            break;

        forth.run(input);
        forth.printStack();
    }

    return 0;
}
