/*
 * @file    tokenizer.h
 * @author  Sebastien LEGRAND
 *
 * @brief   Interface / Forth interpreter tokenizer
 */

// ----- header guards
#ifndef FORTH_TOKENIZER_H_
#define FORTH_TOKENIZER_H_

// ----- includes
#include <string>
#include <optional>
#include <sstream>
#include <list>

// ----- class
class Tokenizer
{
public:
    Tokenizer();
    virtual ~Tokenizer();

    // no copy or move semantics
    Tokenizer(const Tokenizer&) = delete;
    Tokenizer& operator=(const Tokenizer&) = delete;
    Tokenizer(Tokenizer&&) = delete;
    Tokenizer&& operator=(Tokenizer&&) = delete;

    void parse(const std::string&);
    void clear();
    std::optional<std::string> next();
    std::optional<std::string> peek();

private:
    std::istringstream stream_ {};
    std::list<std::string> next_token_ {};
};

#endif // FORTH_TOKENIZER_H_
