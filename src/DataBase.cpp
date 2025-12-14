#include"../include/DataBase.hpp"
#include<sstream>
#include<fstream>

DataBase& DataBase::getinstance(){
    static DataBase instance;
    return instance;
}

//dump() = writing to file
//load() = reading file
bool DataBase::dump(const std::string& file){
    std::lock_guard<std::mutex> lockguard(dbmutex);
    std::ofstream ofs(file, std::ios::binary);
    if(!ofs){
        return false;
    }
/*
 kv_storage
K username someone
K email someone@example.com

 list_storage 
L fruits apple banana cherry          
L colors red green blue

 hash_storage 
H user1 name:p1 age:25 city:Paris
H user2 name:p2 age:30 city:London
*/
    for(const auto& kv:kv_storage){
        ofs<<"K "<< kv.first<<" "<< kv.second<<"\n";
    }                                           
    for(const auto& key:list_storage){
        ofs<<"L "<< key.first;
        for(const auto& values:key.second){
            ofs<<" "<<values;
        }
        ofs<<"\n";
    }
    for(const auto& key:hash_storage){
        ofs<<"H "<< key.first;
        for(const auto& kv:key.second){
            ofs<<" "<<kv.first<<":"<<kv.second;
        }
        ofs<<"\n";
    }
    return true;
    }


bool DataBase::load(const std::string& file){           //loading data from file to different structures
    std::lock_guard<std::mutex> lock(dbmutex);
    std::ifstream ifs(file, std::ios::binary);
    if(!ifs) return false;
    kv_storage.clear();
    list_storage.clear();
    hash_storage.clear();
    
    std::string line;
    while(std::getline(ifs,line)){
        std::istringstream iss(line);
        char type;
        iss>>type;
        //K will seperate key/value simple data kv_storage
        if(type == 'K'){
            std::string key,value;
            iss>>key>>value;
            kv_storage[key] = value;
        }
        //L will seperate List data to list_storage
        else if(type == 'L'){
            std::string key;
            iss>>key;
            std::string item;
            std::deque<std::string> list;
            while(iss>>item){
                list.push_back(item);
            }
            list_storage[key] = list;
        }
        //H will seperate Hash data to hash_storage
        else if(type == 'H'){
            std::string key;
            iss>>key;
            std::unordered_map<std::string, std::string> hash;
            std::string pair;
            while(iss>>pair){
                auto pos = pair.find(':');
                if(pos != std::string::npos){
                    std::string k = pair.substr(0, pos);    
                    std::string v = pair.substr(pos+1);
                    hash[k] = v;
                }
            }
            hash_storage[key] = hash;
        }
    }

    return true;
}

bool DataBase::FLUSHALL(){
    std::lock_guard<std::mutex> lock(dbmutex);
    kv_storage.clear();
    list_storage.clear();
    hash_storage.clear();
    return true;
}

//String commands====================

void DataBase::SET(const std::string& key, const std::string& value){
    std::lock_guard<std::mutex> lock(dbmutex);
    kv_storage[key] = value;
}

bool DataBase::GET(const std::string& key, std::string& value){     //updating parameter value itself
    std::lock_guard<std::mutex> lock(dbmutex);
    if(kv_storage.find(key) != kv_storage.end()){
        value = kv_storage[key];  
        return true;
    } else {
        return false;
    }
}
    
std::vector<std::string> DataBase::KEYS(){
    std::vector<std::string> keys;
    std::lock_guard<std::mutex> lock(dbmutex);
    for(const auto& key:kv_storage){
        keys.push_back(key.first);
    }
    for(const auto& key:list_storage){
        keys.push_back(key.first);
    }
    for(const auto& key:hash_storage){
        keys.push_back(key.first);
    }
    return keys;
}

bool DataBase::DEL(const std::string& key){
    std::lock_guard<std::mutex> lock(dbmutex);
    if(kv_storage.erase(key) || list_storage.erase(key) || hash_storage.erase(key)) return true;
   
    else return false;
}

//List commands=========================

int DataBase::LLEN(const std::string& key){
    std::lock_guard<std::mutex> lock(dbmutex);
    if(list_storage.find(key) != list_storage.end()){
        return list_storage[key].size();
    } else return 0;
}

void DataBase::LPUSH(const std::string& key, const std::vector<std::string> elements){
    std::lock_guard<std::mutex> lock(dbmutex);
    for(int i = 2; i<elements.size();i++){
    list_storage[key].push_front(elements[i]);
    }
}
void DataBase::RPUSH(const std::string& key, const std::vector<std::string> elements){
    std::lock_guard<std::mutex> lock(dbmutex);
    for(int i = 2; i<elements.size();i++){
    list_storage[key].push_back(elements[i]);
    }
}

bool DataBase::LPOP(const std::string& key){
    std::lock_guard<std::mutex> lock(dbmutex);
    if(list_storage.find(key) == list_storage.end()) return false;
    list_storage[key].pop_front();
     if(list_storage[key].size()==0){       //removing whole key if list empty
        list_storage.erase(key);
    }
    return true;
}
bool DataBase::RPOP(const std::string& key){
    std::lock_guard<std::mutex> lock(dbmutex);
    if(list_storage.find(key) == list_storage.end()) return false;
    list_storage[key].pop_back();
    if(list_storage[key].size()==0){
        list_storage.erase(key);
    }
    return true;
}
void DataBase::HSET(const std::string& key, const std::vector<std::string> elements){
    std::lock_guard<std::mutex> lock(dbmutex);
    auto& field = hash_storage[key];
    for(int i = 2 ; i<elements.size() ; i+=2){
        field[elements[i]] = elements[i+1];
    }
}
bool DataBase::HGET(const std::string& key, const std::string& field, std::string value){
    auto& fi= hash_storage[key];
    if(hash_storage.find(key) == hash_storage.end() || fi.find(field) == fi.end()) return false;
    value = fi[field];
    return true;
}

std::unordered_map<std::string, std::string> DataBase::HGETALL(const std::string& key){
    return hash_storage[key];
}

DataBase::DataBase() {}
DataBase::~DataBase() {}