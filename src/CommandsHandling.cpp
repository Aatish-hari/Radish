#include "../include/CommandsHandling.hpp"
#include<iostream>
#include<vector>
#include<sstream>
#include <algorithm>
#include"../include/DataBase.hpp"
// RESP parser:
// *2\r\n$4\r\n\PING\r\n$4\r\nTEST\r\n
// *2 -> array has 2 elements
// $4 -> next string has 4 characters
// return -{PING, TEST}, {ECHO, hello}

CommandsHandling::CommandsHandling(){};

std::vector<std::string> parsingResponse(const std::string &input){

    std::vector<std::string> tokens;
    if(input.empty()){
        return tokens;
    }
    //if input doesnt start with '*', splitting it by whitespaces(blankspaces)
    if(input[0] != '*'){
        std::istringstream iss(input);
        std::string token;
        while(iss>>token){
            tokens.push_back(token);
        }
        return tokens;
    }

    size_t pos = 0;
    //'*' then number of elements on command

    if(input[pos] != '*'){
        return tokens;
    }
    pos++; //skiping '*'

    //crlf = Carriage return(\r) + line feed(\n)
    size_t  crlf = input.find("\r\n",pos);
    if(crlf == std::string::npos) return tokens;    //npos(no position/not found)

    int numofele = std::stoi(input.substr(pos, crlf-pos));      //

    pos = crlf+2;
    for(int i = 0; i<numofele ; i++){
        if(pos>=input.size() || input[pos] != '$') break;
        
        pos++; //skip '$'

        crlf = input.find("\r\n",pos);
        if(crlf == std::string::npos) break;
        int length = std::stoi(input.substr(pos, crlf-pos));
        pos = crlf+2;
        
        if(pos+length>input.size())break;
        std::string token = input.substr(pos,length);
        tokens.push_back(token);
        pos +=length+2;
    }
    return tokens;          
}



std::string CommandsHandling::ProcessCommand(std::string commandline){
    std::vector<std::string> tokens = parsingResponse(commandline);        
    if(tokens.empty()){
        std::cout<<"Error No Command"<<std::endl;
        return " ";
    }
    std::string command = tokens[0];        //token[0] represents the operation 
    std::transform(command.begin(), command.end(), command.begin(), ::toupper);   //command to uppercase
    std::cout<<commandline<<std::endl;

    std::ostringstream response;

    DataBase& database = DataBase::getinstance();

//===============//
   //COMMANDS
//===============//

    if(command == "SING"){
        response<<"+SONG\r\n";
    }
    else if(command =="PING") {
        response<<"-This is not Redis this is Radish, try 'sing'\r\n";
    }
    else if(command == "ECHO"){             
        if(tokens.size() <2){
            response<<"-Error: Require something to 'ECHO'"<<"\r\n";
        }
        else{
            response<<"+"<<tokens[1]<<"\r\n";
        }
    }
    else if(command == "FLUSHALL"){
        if(database.FLUSHALL()){
        response<<"+OK"<<"\r\n";
        } else{
            response<<"-Error FLUSHALL";
        }
    }


    //String commands===============================//
    else if(command == "SET"){
        if(tokens.size()<3){
            response<<"-Error: Require Key and Value to 'SET'"<<"\r\n";
        }
        else{
            database.SET(tokens[1], tokens[2]);
            response<<"+OK"<<"\r\n";
        }
    }
      else if(command == "GET"){
        if(tokens.size()<2){
            response<<"-Error: Require Key to 'GET'"<<"\r\n";
        }
        else{
            std::string value;
            if(database.GET(tokens[1],value)){
            response<<"$"<<std::to_string(value.size())<<"\r\n"<<value<<"\r\n";
            }
            else{
                response<<"$-1\r\n";
            }
        }
    }
    else if(command == "KEYS"){
        std::vector<std::string> keys = database.KEYS();
        response<<"*"<<keys.size()<<"\r\n";
        for(const auto& key:keys){
            response<<"$"<<key.size()<<"\r\n"<<key<<"\r\n";
        }
    }
    else if(command == "DEL"){
        if(tokens.size()<2){
            response<<"-Error: Require Key to Delete\r\n";
        }
        else{
            if(database.DEL(tokens[1])){
                response<<":1"<<"\r\n";
            }else{
                response<<":0"<<"\\r\n";
            }
        }
    }

    //List commands=================================//
    else if(command == "LLEN"){
        if(tokens.size()<2) response<<"-Error: Require Key to get the length of that Key\r\n";
        response<<":"<<std::to_string(database.LLEN(tokens[1]))<<"\r\n";
    }

    else if(command == "LPUSH"){
        if(tokens.size()<3) response<<"-ERR wrong number of arguments for 'lpush' command\r\n";
        database.LPUSH(tokens[1], tokens);
        response<<"(integer) "<<std::to_string(database.LLEN(tokens[1]))<<"\r\n";
    }

    else if(command == "RPUSH"){
        if(tokens.size()<3) response<<"-ERR wrong number of arguments for 'rpush' command\r\n";
        database.RPUSH(tokens[1], tokens);
        response<<"(integer) "<<std::to_string(database.LLEN(tokens[1]))<<"\r\n";
    }
    else if(command == "LPOP"){
        if(tokens.size()!=2) response<<"-Error: Require Key to 'lpop'" ;
        if(database.LPOP(tokens[1])){
        response<<"(integer) "<<std::to_string(database.LLEN(tokens[1]))<<"\r\n";
        } else response<<"(nil)"<<"\r\n";
    }
     else if(command == "RPOP"){
        if(tokens.size()<2) response<<"-Error: Require Key to 'rpop'" ;
        if(database.RPOP(tokens[1])){
        response<<"(integer) "<<std::to_string(database.LLEN(tokens[1]))<<"\r\n";
        } else response<<"(nil)"<<"\r\n";
    }
    //Hash command================================//

    else if(command == "HSET"){
        if(tokens.size()<4) response<<"-Error: Require Key its field and value to 'hset'\r\n";
        database.HSET(tokens[1], tokens);
        response<<":" <<(tokens.size()-2)/2 << "\r\n";
    }
    else if(command == "HGET"){
        if(tokens.size() < 3) response <<"-Error: Require Key its field and value to 'hget'\r\n";
        std::string value;
        if(database.HGET(tokens[1], tokens[2], value)){
            response<<"\r\n"<<value<<"\r\n";
        }else response<<"(nil)"<<"\r\n";
    }
    else  if(command == "HGETALL"){
        if(tokens.size()<2) response<<"-Error: Require Key to 'hgetall\r\n";
        std::unordered_map<std::string, std::string> h = database.HGETALL(tokens[1]);
        for(auto& kv:h){
            response<<kv.first<<":"<<kv.second<<"\n";
        }
        response<<"\r";
    }
    //Unknown command
    else{
        response<<"-Error, Unknown Command or Not Defined in Redi++\r\n";
    }
    return response.str();
}