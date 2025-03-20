/*
 * @file    ForthInterpreter.cc
 * @author  Sebastien LEGRAND
 *
 * @brief   Implementation / Forth Interpreter
 */

// ----- includes
#include "ForthInterpreter.h"

// ----- public implementation

// constructor
ForthInterpreter::ForthInterpreter()
{
    // use lambdas to set the functions_ dictionary
    functions_["+"] = [this]() { binaryOperator(std::plus<>()); };
    functions_["-"] = [this]() { binaryOperator(std::minus<>()); };
    functions_["*"] = [this]() { binaryOperator(std::multiplies<>()); };
    functions_["/"] = [this]() { binaryOperator(std::divides<>()); };

    functions_["DUP"] = [this]() { dup(); };
    functions_["DROP"] = [this]() { drop(); };
    functions_["SWAP"] = [this]() { swap(); };
}

// destructor
/*virtual*/ ForthInterpreter::~ForthInterpreter()
{
}

/* Execute the interpreter on the input line
 * Args:
 *  input (std::string_view) : the input from the user
 */
void ForthInterpreter::run(const std::string& input)
{
    std::istringstream stream(input);
    std::string token;

    while( stream >> token) {
        if (isNumber(token))
            stack_.push_back(std::stoi(token));
        else {
            if (functions_.contains(token))
                functions_[token]();
            else
                std::cerr << "Unknown word [" << token << "]!\n";
        }
    }
}

// ----- private implementation

// duplicate the top of the stack
void ForthInterpreter::dup()
{
    if (!stack_.empty())
        stack_.push_back(stack_.back());
}

// drop the top of the stack
void ForthInterpreter::drop()
{
    if (!stack_.empty())
        stack_.pop_back();
}

// swap the top and the level below
void ForthInterpreter::swap()
{
    int s = stack_.size();
    if (s < 2)
        return;
    std::swap(stack_[s - 1], stack_[s - 2]);
}

/* is the string a number
 * Args:
 *  input (str::string_view) : the input token
 * Returns
 *  True if the string is a number
 */
bool ForthInterpreter(std::string_view token)
{
    if (token.empty())
        return false;

    // check only the first digit (lazy evaluation)
    if (std::isdigit(token[0]))
        return true;

    // special case for negative numbers
    if ((token[0] == '-') && (token.size() > 1) && (std::isdigit(token[1])))
        return true;

    return false;
}
