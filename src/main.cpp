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

    auto magic_value {SqlDB::get_field(magic_str)};
    auto size_value {SqlDB::get_field(db_size)};

    std::cout << "database page size: " << std::get<int>(size_value) << '\n';
    
    return 0;
}