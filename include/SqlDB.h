#ifndef SqlDB_H
#define SqlDB_H

#include <string> 
#include <array> 
#include <memory>
#include <unordered_map>

struct DB_Field
{
    std::string name {};
    std::string dtype {}; //Data type of value
    std::array<uint8_t, 16> value {}; //Value restricted to 8 bytes
};

using FieldMap = std::unordered_map<std::string, DB_Field>;

class SqlDB
{
public:
    SqlDB(std::string& db_file)
        : db_file {db_file}
    {}

    //Gets the DB info of the db_file equivalent to running: sqlite3 [db_file] .dbinfo
    std::unique_ptr<FieldMap> get_dbinfo();
private:
    std::string db_file {};
    
    //Field sizes in bytes
    static const int MAGIC_STR_SIZE {16};
    static const int PAGE_SZ_SIZE {2}; 
};

#endif // SqlDB_H