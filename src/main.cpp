#include<iostream>
#include<thread>
#include "../include/RedisServer.hpp"
#include"../include/DataBase.hpp"
int main(int argc , char* argv[]){

    int port = 6379;        //default redis port
    if(argc>=2){
        port = std::stoi(argv[1]);              // to activate on different port
    }

    RedisServer server = RedisServer();

    if(DataBase::getinstance().load("redi.db")){        //loading data from database to data structures
        std::cout<<"DataBase loaded into main memory\n";        
    }else std::cout<<"No Database loaded, Starting of with new Database\n";

    server.run(port);
    return 0;
}