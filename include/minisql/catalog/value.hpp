#pragma once

#include <string>
#include <variant>
#include <cstdint>
#include "minisql/catalog/schema.hpp"

namespace minisql
{
  using ValueData = std::variant<int64_t, double, std::string, bool>;

  class Value
  {
  public:
    Value() : type_(ColumnType::INTEGER), data_(int64_t(0)) {} // default constructor
    Value(ColumnType type, ValueData data) : type_(type), data_(data) {}

    ColumnType GetType() const { return type_; } // get the actual type first
    int64_t GetInteger() const { return std::get<int64_t>(data_); }
    double GetDouble() const { return std::get<double>(data_); }
    const std::string &GetString() const { return std::get<std::string>(data_); }
    bool GetBoolean() const { return std::get<bool>(data_); }

  private:
    ColumnType type_;
    ValueData data_;
  };

}