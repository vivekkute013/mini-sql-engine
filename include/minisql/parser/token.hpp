#pragma once 
#include <string>

namespace minisql{
  enum class TokenType{
    KEYWORD_SELECT, KEYWORD_FROM, KEYWORD_WHERE, KEYWORD_INSERT, KEYWORD_INTO, KEYWORD_VALUES, KEYWORD_AND, KEYWORD_OR,

    IDENTIFIER,   // table/column names
    NUMBER,    // numeric literals like 5, 6.00, 3.14
    STRING_LITERAL,   // 'text in quotes'

    SYMBOL_STAR, SYMBOL_COMMA, SYMBOL_SEMICOLON, SYMBOL_LPAREN, SYMBOL_RPAREN, SYMBOL_EQUALS, SYMBOL_GT, SYMBOL_LT,

    END_OF_INPUT
  };

  struct Token{
    TokenType type;
    std::string text;
  };

}