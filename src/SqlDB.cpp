#include "SqlDB.h"

#include <string> 
#include <array> 
#include <memory>
#include <unordered_map>
#include <fstream> 
#include <iostream>
#include <cassert>

std::unique_ptr<FieldMap> SqlDB::get_dbinfo()
{
    //Open db file for reading
    std::ifstream file {db_file, std::ios::binary};

    if (!file.is_open())
    {
        throw std::runtime_error("Failed to open database file: " + db_file);
    }

    //Extract the magic string + null term (16 bytes)
    DB_Field magic_str {"magic string", "string"};

    char byte {};
    int str_idx = 0;

    //Read 16 bytes
    while (str_idx < MAGIC_STR_SIZE && file.read(&byte, 1))
    {
        magic_str.value[str_idx] = static_cast<uint8_t>(byte);
        ++str_idx;
    }
    assert(str_idx == MAGIC_STR_SIZE && byte == '\0' && "Magic string read incorrectly");

    //Extract page size (2-byte) (big-endian)
    DB_Field page_sz {"page size", "integer"};
    int sz_idx = 0;
    while (sz_idx < PAGE_SZ_SIZE && file.read(&byte, 1))
    {
        page_sz.value[sz_idx] = static_cast<uint8_t>(byte);
        ++sz_idx; 
    }
    assert(sz_idx == PAGE_SZ_SIZE && "Did not read 2 bytes for page size");

    //Add items to an unordered map
    std::unique_ptr<FieldMap> fieldMap {std::make_unique<FieldMap>()};
    fieldMap->insert({"magic string", magic_str});
    fieldMap->insert({"page size", page_sz});

    return fieldMap;
}