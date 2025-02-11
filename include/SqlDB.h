#ifndef SqlDB_H
#define SqlDB_H

#include <string> 
#include <array> 
#include <memory>
#include <unordered_map>
#include <variant>

enum class Field_type: uint8_t
{
    STRING = 0,
    INTEGER = 1,
    DOUBLE = 2
};

struct DB_Field
{
    std::string name {};
    Field_type dtype {}; //Data type of value
    std::array<uint8_t, 16> value {}; //Value restricted to 16 bytes

    DB_Field(const std::string& n, Field_type t) : name(n), dtype(t) {}

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
    //Converts the bytes stored in DB field to the corresponding dtype
    static std::variant<std::string, int, double> extract_value(const DB_Field& db_field);

private:
    std::string db_file {};
    
    //Field sizes in bytes
    static const int MAGIC_STR_SIZE {16};
    static const int PAGE_SZ_SIZE {2}; 

    //Gets corresponding fields + values, assumes db_file offset is set to correct locations. 
    std::unique_ptr<DB_Field> get_magic_str(std::ifstream& db_file);
    std::unique_ptr<DB_Field> get_db_size(std::ifstream& db_file);
    std::unique_ptr<DB_Field> get_num_tables(std::ifstream& db_file);
};

#endif // SqlDB_H