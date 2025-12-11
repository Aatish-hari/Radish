#include"../include/RedisServer.hpp"
#include <iostream>
#include <sys/socket.h>
#include<netinet/in.h>
#include"../include/CommandsHandling.hpp"
#include <thread>
#include<vector>
#include <cstring>
#include <unistd.h>
#include"../include/DataBase.hpp"
RedisServer::RedisServer(){}

void RedisServer::run(int port){
    int serversock = socket(AF_INET, SOCK_STREAM, 0);
    if(serversock <0){
        std::cerr<<"Error Opening Server Socket\n";
        return;
    }
    int opt = 1;
    setsockopt(serversock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));      //Enable(opt=1) SO_REUSEADDR(can re-bind to a port that is in the TIME_WAIT state.) on this serversock

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    serverAddr.sin_addr.s_addr = INADDR_ANY;       //Bind to all available network interfaces.

    int bindfd = bind(serversock, (struct sockaddr *) &serverAddr, sizeof(serverAddr));
    if(bindfd<0){
        std::cerr<<"Error in binding\n";
        return;
    }
    int listenfd = listen(serversock,10);       //10 is number of client waiting
    if(listenfd<0){
        std::cerr<<"Error listening serverSocket\n";
        return;
    }

    std::cout<<"Redi++ Server Started\n"<<"Server listening on port "<<port<<std::endl;
    
    std::vector<std::thread> threads;
    CommandsHandling commandhandler;

    while(true){
        int clientsock = accept(serversock,nullptr, nullptr);       //client connection
        if(clientsock<0) {
            std::cerr<<"Error in client socket"<<std::endl;
            break;
        }
        
    threads.emplace_back([clientsock, &commandhandler](){
            char buffer[1024];
            while(true){
                memset(buffer,0,sizeof(buffer));                //sets all 1024 bytes in buffer to zero:buffer[0] = '\0'
                int bytes = recv(clientsock, buffer, sizeof(buffer)-1,0);   //buffer[bytes]='\0'
                if(bytes<=0) break;
                std::string request(buffer, bytes); //string construct
                std::string response = commandhandler.ProcessCommand(request);          //send out a outputstringstream
                send(clientsock, response.c_str(), response.size(), 0);

                 if (DataBase::getinstance().dump("redi.db")){
                    std::cout << "Data Saved to redi.db\n"; 
                    }
                    else std::cerr << "Error Saving data\n";
            }
            close(clientsock);
        });

       
    }
     for(auto &t: threads){
            if(t.joinable()){
                t.join();
            }
        }

}