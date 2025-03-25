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
    // arithmetic operators
    functions_["+"] = [this]() { if (shouldExecute()) binaryOperator(std::plus<>()); };
    functions_["-"] = [this]() { if (shouldExecute()) binaryOperator(std::minus<>()); };
    functions_["*"] = [this]() { if (shouldExecute()) binaryOperator(std::multiplies<>()); };
    functions_["/"] = [this]() { if (shouldExecute()) binaryOperator(std::divides<>()); };
    functions_["MOD"] = [this]() { if (shouldExecute()) binaryOperator(std::modulus<>()); };
    functions_["NEGATE"] = [this]() { if (shouldExecute()) unaryOperator(std::negate<>()); };

    // comparison operators
    functions_[">"] = [this]() { if (shouldExecute()) binaryOperator(std::greater<>()); };
    functions_["<"] = [this]() { if (shouldExecute()) binaryOperator(std::less<>()); };
    functions_["="] = [this]() { if (shouldExecute()) binaryOperator(std::equal_to<>()); };
    functions_["<>"] = [this]() { if (shouldExecute()) binaryOperator(std::not_equal_to<>()); };

    functions_["0="] = [this]() { if (shouldExecute()) zeroCompare(ZeroCompFcn::Equal); };
    functions_["0<"] = [this]() { if (shouldExecute()) zeroCompare(ZeroCompFcn::Lesser); };
    functions_["0>"] = [this]() { if (shouldExecute()) zeroCompare(ZeroCompFcn::Greater); };
    functions_["0<>"] = [this]() { if (shouldExecute()) zeroCompare(ZeroCompFcn::Not_Equal); };

    // stack manipulation
    functions_["DUP"] = [this]() { if (shouldExecute()) dup(); };
    functions_["DROP"] = [this]() { if (shouldExecute()) drop(); };
    functions_["SWAP"] = [this]() { if (shouldExecute()) swap(); };

    // bitwise operators
    functions_["AND"] = [this]() { if (shouldExecute()) binaryOperator(std::bit_and<>()); };
    functions_["OR"] = [this]() { if (shouldExecute()) binaryOperator(std::bit_or<>()); };
    functions_["XOR"] = [this]() { if (shouldExecute()) binaryOperator(std::bit_xor<>()); };
    functions_["NOT"] = [this]() { if (shouldExecute()) unaryOperator(std::bit_not<>()); };

    // stack display
    functions_["."] = [this]() { if (shouldExecute()) printTOS(); };

    // control flow
    functions_["IF"] = [this]() { processIf(); };
    functions_["ELSE"] = [this]() { processElse(); };
    functions_["THEN"] = [this]() { processThen(); };

    // word definition
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

// check if the next instruction should be executed
bool ForthVM::shouldExecute()
{
    return (cond_stack_.empty() || (!cond_stack_.top()));
}

// process IF
void ForthVM::processIf()
{
    if (stack_.empty()) {
        std::cerr << "Error: stack is empty!\n";
        return;
    }

    // retrieve the top value on the stack
    int condition = stack_.back(); stack_.pop_back();

    // set the execution and condition stack
    cond_stack_.push(condition == 0);
}

// process Else
void ForthVM::processElse()
{
    if (cond_stack_.empty()) {
        std::cerr << "Error: ELSE without an IF\n";
        return;
    }
    // inverse the condition
    cond_stack_.top() = !cond_stack_.top();
}

// process Then
void ForthVM::processThen()
{
    if (cond_stack_.empty()) {
        std::cerr << "Error: THEN without an IF\n";
        return;
    }
    // remove one level from the stack
    cond_stack_.pop();
}

// print the top of stack
void ForthVM::printTOS()
{
    if (stack_.empty())
        return;

    int top = stack_.back(); stack_.pop_back();
    std::cout << top << "\n";
}

/* compare the top of the stack to 0 (zero)
 * Args:
 *  comp : the test to perform
 */
void ForthVM::zeroCompare(ForthVM::ZeroCompFcn comp)
{
    if (stack_.empty())
        return;

    // retrieve the top of the stack
    int top = stack_.back(); stack_.pop_back();

    switch (comp) {
        case ZeroCompFcn::Equal:     // 0=
            stack_.push_back(top == 0);
            break;
        case ZeroCompFcn::Lesser:     // 0<
            stack_.push_back(top < 0);
            break;
        case ZeroCompFcn::Greater:     // 0>
            stack_.push_back(top > 0);
            break;
        case ZeroCompFcn::Not_Equal:     // 0<>
            stack_.push_back(top != 0);
            break;
    }
}
