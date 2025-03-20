/*
 * @file    ForthInterpreter.h
 * @author  Sebastien LEGRAND
 *
 * @brief   Interface / Forth Interpreter
 */

// ----- header guards
#ifndef FORTH_INTERPRETER_H_
#define FORTH_INTERPRETER_H_

// ----- includes
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <functional>

// ----- class
class ForthInterpreter
{
public:     // public methods
    ForthInterpreter();
    virtual ~ForthInterpreter();

    virtual void run(const std::string&);

    // no copy or move semantics
    ForthInterpreter(const ForthInterpreter&) = delete;
    ForthInterpreter& operator=(const ForthInterpreter&) = delete;
    ForthInterpreter(ForthInterpreter&&) = delete;
    ForthInterpreter& operator=(ForthInterpreter&&) = delete;

private:    // private methods
    void dup();
    void swap();
    void drop();
    bool isNumber(std::string_view);

    template<typename T> void binaryOperator(T);

private:    // private members
    std::vector<int> stack_;
    std::unordered_map<std::string, std::function<void()>> functions_;
};

// ----- template
template<typename T>
void ForthInterpreter::binaryOperator(T op)
{
    // ensure the stack has at least 2 values
    if (stack_.size() < 2) {
        std::cerr << "Error: not enough values on the stack!\n";
        return;
    }

    // retrieve both values
    int b = stack_.back(); stack_.pop_back();
    int a = stack_.back(); stack_.pop_back();

    stack_.push_back(op(a, b));
}

#endif // FORTH_INTERPRETER_H_
