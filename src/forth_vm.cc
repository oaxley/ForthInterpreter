/*
 * @file    forth_vm.cc
 * @author  Sebastien LEGRAND
 *
 * @brief   Implementation / Forth Virtual Machine
 */

// ----- includes
#include "forth_vm.h"

#include <sstream>


// ----- public implementation

// constructor
ForthVM::ForthVM()
{
    // use lambdas to set the functions_ dictionary
    functions_["+"] = [this]() { if (shouldExecute()) binaryOperator(std::plus<>()); };
    functions_["-"] = [this]() { if (shouldExecute()) binaryOperator(std::minus<>()); };
    functions_["*"] = [this]() { if (shouldExecute()) binaryOperator(std::multiplies<>()); };
    functions_["/"] = [this]() { if (shouldExecute()) binaryOperator(std::divides<>()); };

    functions_["DUP"] = [this]() { if (shouldExecute()) dup(); };
    functions_["DROP"] = [this]() { if (shouldExecute()) drop(); };
    functions_["SWAP"] = [this]() { if (shouldExecute()) swap(); };

    functions_["IF"] = [this]() { processIf(); };
    functions_["ELSE"] = [this]() { processElse(); };
    functions_["THEN"] = [this]() { processThen(); };

    functions_[":"] = [this]() { beginDefinition(); };
    functions_[";"] = [this]() { endDefinition(); };
}

// destructor
/*virtual*/ ForthVM::~ForthVM()
{
}

/* Execute the interpreter on the input line
 * Args:
 *  input (std::string) : the input from the user
 */
void ForthVM::run(const std::string& input)
{
    std::istringstream stream(input);
    std::string token;

    while( stream >> token) {
        if ((definefn_) && (token != ";")) {            // user defined function definition
            if (fnname_.length() == 0) {
                fnname_ = {token};
            } else {
                userfn_[fnname_].push_back(token);
            }
        } else if (functions_.contains(token)) {        // reserved keyword
                functions_[token]();
        } else if (isNumber(token) && shouldExecute()) {                   // token is a number
            stack_.push_back(std::stoi(token));
        } else if (userfn_.contains(token) && shouldExecute()) {           // user defined function
            runDefinition(token);
        } else if (shouldExecute()) {
            std::cerr << "Unknown word [" << token << "]!\n";
        }
    }
}

// print the stack (for debug mode)
void ForthVM::printStack() {
    std::cout << "Stack : ";
    for (auto i : stack_) {
        std::cout << i << " ";
    }
    std::cout << "\n";
}

// ----- private implementation

// duplicate the top of the stack
void ForthVM::dup()
{
    if (!stack_.empty())
        stack_.push_back(stack_.back());
}

// drop the top of the stack
void ForthVM::drop()
{
    if (!stack_.empty())
        stack_.pop_back();
}

// swap the top and the level below
void ForthVM::swap()
{
    std::size_t s = stack_.size();
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
bool ForthVM::isNumber(std::string_view token)
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

// begin a user defined function definition
void ForthVM::beginDefinition()
{
    definefn_ = true;
    fnname_ = { };
}

// end a user defined function definition
void ForthVM::endDefinition()
{
    definefn_ = false;
    fnname_ = { };
}

/* execute a user defined functions
 * Args:
 *  token: the user defined token/function name
 */
void ForthVM::runDefinition(const std::string& fnname)
{
    // go through all the words in the definition and execute them
    for(const auto& cmd : userfn_[fnname]) {
        run(cmd);
    }
}
