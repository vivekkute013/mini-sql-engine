#include "minisql/parser/tokenizer.hpp"
#include <cctype>
#include <unordered_map>

namespace minisql
{
  static const std::unordered_map<std::string, TokenType> kKeywords  = {
      {"SELECT", TokenType::KEYWORD_SELECT},
      {"FROM", TokenType::KEYWORD_FROM},
      {"WHERE", TokenType::KEYWORD_WHERE},
      {"INSERT", TokenType::KEYWORD_INSERT},
      {"INTO", TokenType::KEYWORD_INTO},
      {"VALUES", TokenType::KEYWORD_VALUES},
      {"AND", TokenType::KEYWORD_AND},
      {"OR", TokenType::KEYWORD_OR},
  };

  void Tokenizer::SkipWhitespace()
  {
    while (pos_ < sql_.size() && std::isspace(sql_[pos_]))
    {
      pos_++;
    }
  }

  Token Tokenizer::ReadIdentifierOrKeyword()
  {
    size_t start = pos_;
    while (pos_ < sql_.size() && (std::isalnum(sql_[pos_]) || sql_[pos_] == '_'))
    {
      pos_++;
    }
    std::string text = sql_.substr(start, pos_ - start);

    std::string upper_text = text;
    for (auto &c : upper_text)
      c = std::toupper(c);

    auto it = kKeywords.find(upper_text);
    if (it != kKeywords.end())
    {
      return Token{it->second, upper_text};
    }
    return Token{TokenType ::IDENTIFIER, text};
  }

  Token Tokenizer::ReadNumber()
  {
    size_t start = pos_;
    while (pos_ < sql_.size() && (std::isdigit(sql_[pos_]) || sql_[pos_] == '.'))
    {
      pos_++;
    }
    return Token{TokenType::NUMBER, sql_.substr(start, pos_ - start)};
  }

  Token Tokenizer::ReadStringLiteral()
  {
    size_t start = pos_;
    pos_++;
    while (pos_ < sql_.size() && sql_[pos_] != '\'')
    {
      pos_++;
    }
    std::string text = sql_.substr(start, pos_ - start);
    pos_++;
    return Token{TokenType::STRING_LITERAL, text};
  }

  Token Tokenizer::NextToken()
  {
    SkipWhitespace();

    if (pos_ >= sql_.size())
    {
      return Token{TokenType::END_OF_INPUT, ""};
    }

    char c = sql_[pos_];

    if (std::isalpha(c) || c == '_')
      return ReadIdentifierOrKeyword();
    if (std::isdigit(c))
      return ReadNumber();
    if (c == '\'')
      return ReadStringLiteral();

    switch (c)
    {
    case '*':
      pos_++;
      return Token{TokenType::SYMBOL_STAR, "*"};
    case ',':
      pos_++;
      return Token{TokenType::SYMBOL_COMMA, ","};
    case ';':
      pos_++;
      return Token{TokenType::SYMBOL_SEMICOLON, ";"};
    case '(':
      pos_++;
      return Token{TokenType::SYMBOL_LPAREN, "("};
    case ')':
      pos_++;
      return Token{TokenType::SYMBOL_RPAREN, ")"};
    case '=':
      pos_++;
      return Token{TokenType::SYMBOL_EQUALS, "="};
    case '>':
      pos_++;
      return Token{TokenType::SYMBOL_GT, ">"};
    case '<':
      pos_++;
      return Token{TokenType::SYMBOL_LT, "<"};
    default:
      pos_++;
      return Token{TokenType::END_OF_INPUT, ""};
    }
  }

  std::vector<Token> Tokenizer::Tokenize()
  {
    std::vector<Token> tokens;
    Token tok;
    do
    {
      tok = NextToken();
      tokens.push_back(tok);
    } while (tok.type != TokenType::END_OF_INPUT);

    return tokens;
  }

}