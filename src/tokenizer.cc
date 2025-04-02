/*
 * @file    tokenizer.cc
 * @author  Sebastien LEGRAND
 *
 * @brief   Implementation / Forth interpreter tokenizer
 */

// ----- includes
#include "tokenizer.h"

// ----- public implementation

// constructor
Tokenizer::Tokenizer()
{
}

// destructor
/*virtual*/ Tokenizer::~Tokenizer()
{
}

/* Initialize the underlying istream with the user input
 * Args:
 *      line (std::string) : the input line from the user
 */
void Tokenizer::parse(const std::string& line)
{
    // clear the current istream
    clear();

    // set the istream to the new line
    stream_.str(line);
}

// clear the current istream
void Tokenizer::clear()
{
    stream_.str("");
    stream_.seekg(0L);
    next_token_.clear();
}

/* return the next token in the string
 * Returns:
 *      The next token if any, std::nullopt if none is available
 */
std::optional<std::string> Tokenizer::next()
{
    std::string token;

    // lookup in the list if we already have a token from a previous peek
    if (next_token_.size() > 0) {
        // retrieve the token and send it back
        token = next_token_.front(); next_token_.pop_front();
        return token;
    }

    // end of the stream
    if (stream_.eof())
        return std::nullopt;

    // retrieve a new token
    stream_ >> token;
    return token;
}

/* peek at the next token, without removing it from the queue
 * Returns:
 *      The next token if any, std::nullopt if none is available
 */
std::optional<std::string> Tokenizer::peek()
{
    // no more token to read
    if (stream_.eof())
        return std::nullopt;

    // grab the next token and add it to the list
    std::string token;
    stream_ >> token;
    next_token_.push_back(token);

    return token;
}
