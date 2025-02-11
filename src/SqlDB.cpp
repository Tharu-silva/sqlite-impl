#include "SqlDB.h"

#include <string> 
#include <array> 
#include <memory>
#include <unordered_map>
#include <fstream> 
#include <iostream>
#include <cassert>
#include <exception>

std::unique_ptr<FieldMap> SqlDB::get_dbinfo()
{
    //Open db file for reading
    std::ifstream file {db_file, std::ios::binary};

    if (!file.is_open())
    {
        throw std::runtime_error("Failed to open database file: " + db_file);
    }

    //Extract the magic string
    std::unique_ptr<DB_Field> magic_str {get_magic_str(file)};

    //Extract page size
    std::unique_ptr<DB_Field> db_size {get_db_size(file)};

    //Offset by 100 bytes to get page 1 (schema table) header
    file.seekg(100, std::ios::beg);
    std::unique_ptr<DB_Field> num_tables {get_num_tables(file)};

    //Add items to an unordered map
    std::unique_ptr<FieldMap> fieldMap {std::make_unique<FieldMap>()};
    fieldMap->insert({"magic string", *magic_str});
    fieldMap->insert({"page size", *db_size});
    fieldMap->insert({"number of tables", *num_tables});

    return fieldMap;
}

std::variant<std::string, int, double> SqlDB::extract_value(const DB_Field& db_field)
{
    switch (db_field.dtype)
    {
        case Field_type::STRING:
        {
            std::string value {};
            for (const auto& byte: db_field.value)
                value.push_back(static_cast<uint8_t>(byte));
            return value; 
        }
        case Field_type::INTEGER:
        {
            int value {}; //MSB at lowest idx
            value = db_field.value[0] << 8 | db_field.value[1]; 
            return value; 
        }
        case Field_type::DOUBLE:
        {
            double value {};
            throw std::runtime_error("Unimplemented");
            return value;
        }
    }

    return std::variant<std::string, int, double>();
}

std::unique_ptr<DB_Field> SqlDB::get_magic_str(std::ifstream &db_file)
{
    //Extract the magic string + null term (16 bytes)
    std::unique_ptr<DB_Field> magic_str {
        std::make_unique<DB_Field>("magic string", Field_type::STRING)
        };

    char byte {};
    int str_idx = 0;

    //Read 16 bytes
    while (str_idx < MAGIC_STR_SIZE && db_file.read(&byte, 1))
    {
        magic_str->value[str_idx] = static_cast<uint8_t>(byte);
        ++str_idx;
    }
    assert(str_idx == MAGIC_STR_SIZE && byte == '\0' && "Magic string read incorrectly");

    return magic_str;
}

std::unique_ptr<DB_Field> SqlDB::get_db_size(std::ifstream &db_file)
{
    //Extract page size (2-byte) (big-endian)
    std::unique_ptr<DB_Field> paze_sz {std::make_unique<DB_Field>("page size", Field_type::INTEGER)};

    int sz_idx = 0;
    char byte {};
    while (sz_idx < PAGE_SZ_SIZE && db_file.read(&byte, 1))
    {
        paze_sz->value[sz_idx] = static_cast<uint8_t>(byte);
        ++sz_idx; 
    }
    assert(sz_idx == PAGE_SZ_SIZE && "Did not read 2 bytes for page size");

    return paze_sz;
}

std::unique_ptr<DB_Field> SqlDB::get_num_tables(std::ifstream& db_file)
{
    //At a leaf table b-tree page
    std::streamoff cell_offs {3}; //Offset for cell count 
    db_file.seekg(cell_offs, std::ios::cur);

    //Extract cell count (2-byte) big-endian
    std::unique_ptr<DB_Field> num_cells {std::make_unique<DB_Field>("number of tables", Field_type::INTEGER)};

    int sz_idx = 0;
    char byte {};
    while (sz_idx < PAGE_SZ_SIZE && db_file.read(&byte, 1))
    {
        num_cells->value[sz_idx] = static_cast<uint8_t>(byte);
        ++sz_idx; 
    }
    assert(sz_idx == PAGE_SZ_SIZE && "Did not read 2 bytes for cell count/number of tables");

    return num_cells;
}
