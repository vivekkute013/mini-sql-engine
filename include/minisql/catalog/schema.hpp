#pragma once

#include <string>
#include <vector>
#include <cstdint>

namespace minisql
{
  // type of data column can hold.

  enum class ColumnType
  {
    // -- Integer has different sizes = different storage cost
    SMALLINT, // 2 bytes,  range approx -32,768 to 32,767
    INTEGER,  // 4 bytes,  range approx -2 billion to 2 billion
    BIGINT,   // 8 bytes,  range approx -9 quintillion to 9 quintillion

    // -- Floating point / decimal types ------
    FLOAT,   // 4 bytes,  approximate decimal (fast, imprecise)
    DOUBLE,  // 8 bytes,  approximate decimal (more precision than FLOAT)
    DECIMAL, // fixed-point exact decimal — we store as scaled integer

    // -- Text Types
    CHAR,    // fixed-length text, padded with spaces if shorter
    VARCHAR, // variable-length text, up to a max length
    TEXT,    // large variable-length text, no practical max

    // -- Boolean  ---
    BOOLEAN, // 1 byte, true/false

    // -- Date/time types ---
    DATE,     // stored as days since epoch (4 bytes)
    TIMESTAMP // stored as seconds since epoch (8 bytes)
  };

  class Column
  {
  public:
    Column(const std::string &name, ColumnType type, size_t length = 0, size_t precision = 0, size_t scale = 0) : name_(name), type_(type), length_(length), precision_(precision), scale_(scale) {}

    const std::string &GetName() const { return name_; }
    ColumnType GetType() const { return type_; }
    size_t GetLength() const { return length_; }
    size_t GetPrecision() const { return precision_; }
    size_t GetScale() const { return scale_; }

    // How many bytes this column occupies inside a serialized row.
    // here we return their max reserved size for our fixed slot
    size_t GetFixedSize() const
    {
      switch (type_)
      {
      case ColumnType::SMALLINT:
        return 2;
      case ColumnType::INTEGER:
        return 4;
      case ColumnType::BIGINT:
        return 8;
      case ColumnType::FLOAT:
        return 4;
      case ColumnType::DOUBLE:
        return 8;
      case ColumnType::DECIMAL:
        return 8;
      case ColumnType::CHAR:
        return length_;
      case ColumnType::VARCHAR:
        return length_;
      case ColumnType::TEXT:
        return length_ > 0 ? length_ : 256;
      case ColumnType::BOOLEAN:
        return 1;
      case ColumnType::DATE:
        return 4;
      case ColumnType::TIMESTAMP:
        return 8;
      }
      return 0;
    }

  private:
    std::string name_;
    ColumnType type_;
    size_t length_;    // for CHAR/VARCHAR/TEXT : max character length
    size_t precision_; // for Decimal : total significant digits
    size_t scale_;     // for Decimal : digit after decimal point
  };

  class Schema
  {
  public:
    explicit Schema(const std::vector<Column> &columns) : columns_(columns) {}

    const std::vector<Column> &GetColumns() const { return columns_; }
    size_t GetColumnCount() const { return columns_.size(); }

    size_t GetRowSize() const
    {
      size_t total = 0;
      for (const auto &col : columns_)
      {
        total += col.GetFixedSize();
      }
      return total;
    }

  private:
    std ::vector<Column> columns_;
  };

}