#pragma once

#include <vector>
#include "minisql/parser/token.hpp"
#include "minisql/parser/ast.hpp"

namespace minisql{
  class Parser{
    public :
    explicit Parser(const std::vector<Token>& tokens) : token_(tokens), pos_(0) {}

    SelectStatement ParseSelect();
    InsertStatement ParseInsert();

    private : 
    const Token& Current() const;
    void Advance();
    void Expect(TokenType type);

    std::vector<Token> token_;
    size_t pos_;
  };


}