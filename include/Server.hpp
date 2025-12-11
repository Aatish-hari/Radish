#ifndef REDIS_SERVER_HPP
#define REDIS_SERVER __has_cpp_attribute

class RedisServer{
public:

    int port;
    RedisServer();
    void run(int port);


};

#endif