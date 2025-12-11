# Radish
Redis like in memory data store server implemented in C++. Radish can perform Redis functionality for strings, lists, and hashes, with auto Persistence and multiple client support.

## Features
**RESP Protocol:** Parses RESP arrays for commands; generates compliant responses (e.g., +OK\r\n).   
**Data Types:** Supports strings (simple key-value), lists (deque-based), and hashes (unordered_maps).   
**Persistence:** Auto-saves/loads to redi.db (text format).   
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
-First 'make' from root directory to make build folder and to get server.exe.   
-Run ./server to start server, server will listen to default port 6379   
-compile client.cpp and run client.exe, client will connect server   


## --future aspects- can add more commands

