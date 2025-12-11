# Radish
Redis like in memory data store server implemented in C++. Radish can perform Redis functionality for strings, lists, and hashes, with auto Persistence and multiple client support.

## Features
**RESP Protocol:** Implemented to handel RESP (Redis Serialization Protocol) for commands and responses.   

**Data Types:** Supports strings (each key with value), lists (each key with deque), and hashes (each key with unordered_map).   

**Persistence:** Auto-saves/loads to redi.db.   

**Concurrency:** Mutex-protected singleton DataBase, per-client threads for multi-user support.   

## Supported Commands:
| General Commands | String Commands | List Commands | Hash Commands |
|------------------|-----------------|---------------|---------------|
| SING             | SET key value   | LLEN key      | HSET key field value |
| PING             | GET key         | LPUSH key elem | HGET key field |
| ECHO msg         | DEL key         | RPUSH key elem | HGETALL key |
| FLUSHALL         | KEYS            | LPOP key      | HDEL key field |
|                  |                 | RPOP key      |               |

## Installation
This project uses Makefile. Ensure you have a C++17 (or later ones) compiler.   
-First 'make' from project's root directory to make build folder and to get server.exe.   
-Run ./server to start server, server will listen to default port 6379   
-compile client.cpp and run client.exe, client will connect server   


### Future aspects- will be adding more commands

