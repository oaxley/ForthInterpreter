/*
 * @file    forth_vm.h
 * @author  Sebastien LEGRAND
 *
 * @brief   Interface / Forth Virtual Machine
 */

// ----- header guards
#ifndef FORTH_VM_H_
#define FORTH_VM_H_

// ----- includes
#include <deque>
#include <functional>
#include <iostream>
#include <stack>
#include <string>
#include <unordered_map>
#include <vector>


// ----- class
class ForthVM
{
public:     // public methods
    ForthVM();
    virtual ~ForthVM();

    virtual void run(const std::string&);
    virtual void printStack();

    // no copy or move semantics
    ForthVM(const ForthVM&) = delete;
    ForthVM& operator=(const ForthVM&) = delete;
    ForthVM(ForthVM&&) = delete;
    ForthVM& operator=(ForthVM&&) = delete;

private:    // private methods
    void dup();
    void swap();
    void drop();
    bool isNumber(std::string_view);

    template<typename T> void binaryOperator(T);

    void beginDefinition();
    void endDefinition();
    void runDefinition(const std::string&);

    bool shouldExecute();
    void processIf();
    void processElse();
    void processThen();

private:    // private members
    std::vector<int> stack_;
    std::unordered_map<std::string, std::function<void()>> functions_;

    // user defined functions
    std::unordered_map<std::string, std::deque<std::string>> userfn_;
    std::string fnname_;
    bool definefn_ {false};

    // conditions stack (if..else..then)
    std::stack<bool> cond_stack_;
};

// ----- template
template<typename T>
void ForthVM::binaryOperator(T op)
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

#endif // FORTH_VM_H_
