#pragma once

#include <vector>
#include <string>
#include "minisql/parser/token.hpp"

namespace minisql{
  class Tokenizer{
    public : 
    explicit Tokenizer(const std::string& sql) : sql_(sql), pos_(0) {}

    std::vector<Token> Tokenize();
    
    private : 
    void SkipWhitespace();
    Token NextToken();
    Token ReadIdentifierOrKeyword();
    Token ReadNumber();
    Token ReadStringLiteral();

    std::string sql_;
    size_t pos_;

  };


}