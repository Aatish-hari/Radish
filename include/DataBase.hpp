#ifndef DATABASE_HPP
#define DATABASE_HPP
#include<string>
#include<mutex>
#include<unordered_map>
#include <vector>
#include <deque>
class DataBase{
    
private:
    DataBase();
    ~DataBase();
    DataBase(const DataBase& ) = delete;
    DataBase& operator=(const DataBase&) = delete;
    std::mutex dbmutex;
    //data structures==
    std::unordered_map<std::string, std::string> kv_storage;
    std::unordered_map<std::string, std::deque<std::string>> list_storage;
    std::unordered_map<std::string, std::unordered_map<std::string, std::string>> hash_storage;

public:

//singleton instance
    static DataBase& getinstance();
    //dump & load the database from file
    bool dump(const std::string& file);
    bool load(const std::string& file);

    bool FLUSHALL();
    
    //String commands-
    void SET(const std::string& key, const std::string& value);
    bool GET(const std::string& key, std::string& value);
    std::vector<std::string> KEYS();
    bool DEL(const std::string& key);

    //List commands-
    int LLEN(const std::string& key);
    void LPUSH(const std::string& key, const std::vector<std::string> elements);
    void RPUSH(const std::string& key, const std::vector<std::string> elements);
    bool LPOP(const std::string& key);
    bool RPOP(const std::string& key);

    //Hash commands-
    void HSET(const std::string& key, const std::vector<std::string> elements);
    bool HGET(const std::string& key,  const std::string& field, std::string& value);
    std::unordered_map<std::string, std::string> HGETALL(const std::string& key);
};

#endif