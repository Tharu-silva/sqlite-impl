#include "SqlDB.h"
#include <iostream>
#include <string> 

int main() 
{
    std::string f_name {"../sample.db"};
    SqlDB sqldb {f_name};

    std::unique_ptr<FieldMap> fieldmap {sqldb.get_dbinfo()};

    DB_Field magic_str {fieldmap->at("magic string")};
    DB_Field db_size {fieldmap->at("page size")};    
    DB_Field num_tables {fieldmap->at("number of tables")};

    auto magic_value {SqlDB::extract_value(magic_str)};
    auto size_value {SqlDB::extract_value(db_size)};
    auto num_tables_val {SqlDB::extract_value(num_tables)}; 

    std::cout << "database page size: " << std::get<int>(size_value) << '\n';
    std::cout << "number of tables: " << std::get<int>(num_tables_val) << '\n';

    return 0;
}