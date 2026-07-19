#pragma once

#include <string>
#include <vector>
#include <memory>

namespace minisql{
  
  // A Single where condition, eg, "id" = 5
  struct WhereCondition{
    std::string column;
    std::string op;   // "=", ">", "<"
    std::string value;   // right-hand side, as raw text for me now
  };

  // Represent a parsed SELECT statement
  struct SelectStatement{
    std::vector<std::string> columns;
    std::string table_name;
    bool has_where = false;
    WhereCondition where;
  };

  // Represent parsed INSERT Statement
  struct InsertStatement{
    std::string table_name;
    std::vector<std::string> values;  // raw text values, in order
  };


}