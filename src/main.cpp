#include <iostream>
#include "minisql/catalog/row.hpp"
#include "minisql/catalog/schema.hpp"

int main()
{
    // Define a schema : a 'users' table with 3 Columns
    std::vector<minisql::Column> columns = {
        minisql::Column("id", minisql::ColumnType::INTEGER),
        minisql::Column("name", minisql::ColumnType::VARCHAR, 50),
        minisql::Column("is_active", minisql::ColumnType::BOOLEAN)};
    minisql::Schema schema(columns);

    std::cout << "Row size for this Schema : " << schema.GetRowSize() << " bytes\n";

    // Build one real row of data matching that schema.
    std::vector<minisql::Value> values = {
        minisql::Value(minisql::ColumnType::INTEGER, int64_t(101)),
        minisql::Value(minisql::ColumnType::VARCHAR, std::string("Vivek")),
        minisql::Value(minisql::ColumnType::BOOLEAN, true)};
    minisql::Row row(values);

    // Serialize it into raw buffer.
    char buffer[256]; // storage
    row.SerializeTo(buffer, schema);
    std::cout << " Row serialized successfully. \n";

    // Deserialized it back into a new row.
    minisql::Row restored = minisql::Row::DeserializeFrom(buffer, schema);

    // Print and Verify each value.
    const auto &restored_values = restored.GetValues();
    std::cout << "id : " << restored_values[0].GetInteger() << "\n";
    std::cout << "name : " << restored_values[1].GetString() << "\n";
    std::cout << "is_active " << (restored_values[2].GetBoolean() ? "true" : "false") << "\n";

    bool ok = restored_values[0].GetInteger() == 101 && restored_values[1].GetString() == "Vivek" && restored_values[2].GetBoolean() == true;

    std::cout << (ok ? "SUCCESS : row round-tripped correctly. \n" : "FAILURE : mismatch after deserialization. \n");

    return 0;
}