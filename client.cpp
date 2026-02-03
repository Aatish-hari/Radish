#include <iostream>
#include <string>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include<sstream>
#include<vector>
int main() {
    std::string ip = "127.0.0.1";
    int port = 6379;

    // Create socket
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        std::cerr << "Error: Cannot create socket\n";
        return 1;
    }

    //Server address
    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);

    if (inet_pton(AF_INET, ip.c_str(), &serverAddr.sin_addr) <= 0) {
        std::cerr << "Error: Invalid address\n";
        return 1;
    }

    //Connect to server
    if (connect(sockfd, (sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        std::cerr << "Error: Cannot connect to server\n";
        return 1;
    }

    std::cout << "Connected to Radish server\n";

    //REPL client loop
    while (true) {
        std::string line;
        std::cout<< ip << ":" << port << "> ";
        std::getline(std::cin, line);

        if (line == "exit") {
            std::cout << "EXITING\n";
            break;
        }

        if (line.empty()) continue;

        // Convert simple text into RESP format
        // Example: PING -> *1\r\n$4\r\nPING\r\n
        // Example: SET key value -> *3\r\n$3\r\nSET\r\n$3\r\nkey\r\n$5\r\nvalue\r\n

        std::istringstream iss(line);
        std::vector<std::string> parts;
        std::string word;

        while (iss >> word){
            parts.push_back(word);
        }

        // Building RESP
        std::string resp;
        resp += "*" + std::to_string(parts.size()) + "\r\n";
        
        for (const auto& p : parts) {
            resp += "$" + std::to_string(p.size()) + "\r\n";
            resp += p + "\r\n";
        }

        // Send command to server
        ssize_t sent = send(sockfd, resp.c_str(), resp.size(), 0);
        if (sent < 0) {
            std::cerr << "Error sending data\n";
            break;
        }
        char buffer[4096] = {0};  // Buffer for response
        int total = 0;

        while (true) {
        int bytes = recv(sockfd, buffer + total, sizeof(buffer) - total - 1, 0);

        if (bytes <= 0){ 
            std::cout<<"Redi++ Server Closed"<<std::endl;
            break;
        }  // Error or closed
        total += bytes;
        buffer[total] = '\0';

       
        std::cout<< buffer << std::endl;
        break;
}

    }
    std::cout<<"Closing client"<<std::endl;
    close(sockfd);
    return 0;
}
