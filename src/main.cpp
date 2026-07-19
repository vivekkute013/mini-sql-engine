#include <iostream>
#include "minisql/parser/parser.hpp"
#include "minisql/parser/tokenizer.hpp"

int main() {
    std::string sql1 = "SELECT id, name FROM users WHERE id = 5;";
    minisql::Tokenizer tokenizer1(sql1);
    auto token1 = tokenizer1.Tokenize();
    minisql::Parser parser1(token1);
    minisql::SelectStatement stmt1 = parser1.ParseSelect();

    std::cout<<"--- Parsed SELECT ---\n";
    std::cout<<"Table: "<<stmt1.table_name<<"\n";
    std::cout<<"Columns: ";
    for(const auto& col : stmt1.columns) std::cout<<col<<" ";
    std::cout<<"\n";
    if(stmt1.has_where){
        std::cout<<"WHERE "<<stmt1.where.column<<" "<<stmt1.where.op<<" "<<stmt1.where.value<<"\n";
    }

    std::string sql2 = "INSERT INTO users VALUES (101, 'Vivek', true);";
    minisql::Tokenizer tokenizer2(sql2);
    auto token2 = tokenizer2.Tokenize();
    minisql::Parser parser2(token2);
    minisql::InsertStatement stmt2 = parser2.ParseInsert();

    std::cout<<"\n--- Parsed INSERT -----\n";
    std::cout<<"Table: "<<stmt2.table_name<<"\n";
    std::cout<<"Values: ";
    for(const auto& val : stmt2.values) std::cout<<val<<" ";
    std::cout<<"\n";

    return 0;
}