#include "minisql/parser/parser.hpp"
#include <stdexcept>

namespace minisql{
  const Token& Parser::Current() const {
    return token_[pos_];
  }

  void Parser::Advance(){
    if(pos_ < token_.size() - 1){
      pos_++;
    }
  }

  void Parser::Expect(TokenType type){
    if(Current().type != type){
      throw std::runtime_error("Parser error : unpected token " + Current().text + " ");
    }
    Advance();
  }

  SelectStatement Parser::ParseSelect() {
    SelectStatement stmt;

    Expect(TokenType::KEYWORD_SELECT);
    if(Current().type == TokenType::SYMBOL_STAR){
      stmt.columns.push_back("*");
      Advance();
    }else{
      stmt.columns.push_back(Current().text);
      Expect(TokenType::IDENTIFIER);

      while(Current().type == TokenType::SYMBOL_COMMA){
        Advance();
        stmt.columns.push_back(Current().text);
        Expect(TokenType::IDENTIFIER);
      }
    }

    Expect(TokenType::KEYWORD_FROM);

    stmt.table_name = Current().text;
    Expect(TokenType::IDENTIFIER);

    if(Current().type == TokenType::KEYWORD_WHERE){
      Advance();
      stmt.has_where = true;

      stmt.where.column = Current().text;
      Expect(TokenType::IDENTIFIER);

      stmt.where.op = Current().text;
      Advance();   // accept =, > or < without strict Expect, since any is allowed here

      stmt.where.value = Current().text;
      Advance();
    }
    return stmt;
  }

  InsertStatement Parser::ParseInsert() {
    InsertStatement stmt;

    Expect(TokenType::KEYWORD_INSERT);
    Expect(TokenType::KEYWORD_INTO);

    stmt.table_name = Current().text;
    Expect(TokenType::IDENTIFIER);

    Expect(TokenType::KEYWORD_VALUES);
    Expect(TokenType::SYMBOL_LPAREN);

    stmt.values.push_back(Current().text);
    Advance();

    while(Current().type == TokenType::SYMBOL_COMMA){
      Advance();
      stmt.values.push_back(Current().text);
      Advance();
    }
    Expect(TokenType :: SYMBOL_RPAREN);

    return stmt;
  }

}