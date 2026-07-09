#include <cstring>
#include <cmath>
#include "minisql/catalog/row.hpp"

namespace minisql{
  void Row::SerializeTo(char*storage, const Schema& schema) const {
    size_t offset = 0;
    const auto& columns = schema.GetColumns();

    for(size_t i = 0; i< columns.size(); i++){
      const Column& col = columns[i];
      const Value& val = values_[i];

      switch(col.GetType()){
        case ColumnType::SMALLINT:{
          int16_t v = static_cast<int16_t>(val.GetInteger());
          std::memcpy(storage + offset, &v, sizeof(int16_t));
          break;
        }
        case ColumnType::INTEGER:{
          int32_t v = static_cast<int32_t>(val.GetInteger());
          std::memcpy(storage + offset, &v, sizeof(int32_t));
          break;
        }
        case ColumnType::BIGINT:{
          int64_t v = val.GetInteger();
          std::memcpy(storage + offset, &v, sizeof(int64_t));
          break;
        }
        case ColumnType::FLOAT:{
          float v = static_cast<float>(val.GetDouble());
          std::memcpy(storage + offset, &v, sizeof(float));
          break;
        }
        case ColumnType::DOUBLE:{
          double v = val.GetDouble();
          std::memcpy(storage + offset, &v, sizeof(double));
          break;
        }
        case ColumnType::DECIMAL:{
          //store as a scaled integer, eg - 19.99 with scales 2 -> 1999
          int16_t scaled = static_cast<int64_t>(
            val.GetDouble() * std::pow(10, col.GetScale())
          );
          std::memcpy(storage + offset, &scaled, sizeof(int16_t));
          break;
        }
        case ColumnType::BOOLEAN:{
          bool v = val.GetBoolean();
          std::memcpy(storage + offset, &v, sizeof(bool));
          break;
        }
        case ColumnType::DATE:{
          int32_t v = static_cast<int32_t>(val.GetInteger());
          std::memcpy(storage + offset, &v, sizeof(int32_t));
          break;
        }
        case ColumnType::TIMESTAMP:{
          int64_t v = val.GetInteger();
          std::memcpy(storage + offset, &v, sizeof(int64_t));
          break;
        }
        case ColumnType::CHAR:
        case ColumnType::VARCHAR:
        case ColumnType::TEXT:{
          const std::string& s = val.GetString();
          std::memset(storage + offset, 0, col.GetFixedSize());
          std::memcpy(storage + offset, s.c_str(), std::min(s.size(), col.GetFixedSize()));
          break;
        }
      }
      offset += col.GetFixedSize();
    }

  }

  Row Row::DeserializeFrom(const char* storage, const Schema& schema){
    size_t offset = 0;
    std::vector<Value> values;
    const auto& columns = schema.GetColumns();

    for(const auto& col : columns){
      switch(col.GetType()){
        case ColumnType::SMALLINT : {
          int16_t v;
          std::memcpy(&v, storage + offset, sizeof(int16_t));
          values.push_back(Value(col.GetType(), int64_t(v)));
          break;
        }
        case ColumnType::INTEGER : {
          int32_t v;
          std::memcpy(&v, storage + offset, sizeof(int32_t));
          values.push_back(Value(col.GetType(), int64_t(v)));
          break;
        }
        case ColumnType::BIGINT :
        case ColumnType::TIMESTAMP : {
          int64_t v;
          std::memcpy(&v, storage + offset, sizeof(int64_t));
          values.push_back(Value(col.GetType(), v));
          break;
        }
        case ColumnType::FLOAT : {
          float v;
          std::memcpy(&v, storage + offset, sizeof(float));
          values.push_back(Value(col.GetType(), double(v)));
          break;
        }
        case ColumnType::DOUBLE : {
          double v;
          std::memcpy(&v, storage + offset, sizeof(double));
          values.push_back(Value(col.GetType(), v));
          break;
        }
        case ColumnType::DECIMAL : {
          int64_t scaled;
          std::memcpy(&scaled, storage + offset, sizeof(int64_t));
          double v = scaled / std::pow(10, col.GetScale());
          values.push_back(Value(col.GetType(), v));
          break;
        }
        case ColumnType::BOOLEAN : {
          bool v;
          std::memcpy(&v, storage + offset, sizeof(bool));
          values.push_back(Value(col.GetType(), v));
          break;
        }
        case ColumnType::DATE : {
          int32_t v;
          std::memcpy(&v, storage + offset, sizeof(int32_t));
          values.push_back(Value(col.GetType(), int64_t(v)));
          break;
        }
        case ColumnType::CHAR :
        case ColumnType::VARCHAR :
        case ColumnType::TEXT :{
          std::string s(storage + offset, col.GetFixedSize());
          // trim trailing zero padding bytes
          size_t end = s.find('\0');
          if(end != std::string::npos) s.resize(end);
          values.push_back(Value(col.GetType(), s));
          break;
        }
      }
      offset += col.GetFixedSize();
    }
    return Row(values);
  }

}