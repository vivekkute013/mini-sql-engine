#pragma once

#include <vector>
#include "minisql/catalog/schema.hpp"
#include "minisql/catalog/value.hpp"

namespace minisql{
  // row holds one records worth of Actual Values, in the same order as schema's column. 
  // It knows how to convert itself to/from raw bytes so it can be stored inside a page.

  class Row{
    public : 
    explicit Row(const std::vector<Value>& values) : values_(values) {}

    const std::vector<Value>& GetValues() const { return values_; }

      // write down this row's data in 'storage', according to 'schema'
    void SerializeTo(char* storage, const Schema& schema) const;

    // Reads a row's worth data out of 'storage', according to 'schema'
    // and returns a new Row built from it.
    static Row DeserializeFrom(const char* storage, const Schema& schema);

    private :
      std:: vector<Value> values_;

  };


}