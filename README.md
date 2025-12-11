# Radish
Redis like in memory data store server implemented in C++. Radish can perform Redis functionality for strings, lists, and hashes, with auto Persistence and multiple client support.


## Features
### Protocol Support: Handles RESP (Redis Serialization Protocol) for commands and responses.
### Data Types: Strings (key-value), Lists (push/pop from ends), Hashes (field-value maps).
### Persistence: Automatic save/load to/from redi.db (plain-text format).
### Concurrency: Thread-safe with mutexes; one thread per client connection.

## Supported Commands
| General Commands | Description | String Commands | Description | List Commands | Description | Hash Commands | Description |
|------------------|-------------|-----------------|-------------|---------------|-------------|---------------|-------------|
| **SING** | returns "SONG" | **SET key value** | store value | **LLEN key** | list size | **HSET key field value** | set fields |
| **PING** | try 'sing' hint | **GET key** | get value | **LPUSH key elem** | push head | **HGET key field** | get field |
| **ECHO msg** | echo message | **DEL key** | delete key | **RPUSH key elem** | push tail | **HGETALL key** | all fields |
| **FLUSHALL** | clear all data | **KEYS** | list keys | **LPOP key** | pop head | **HDEL key field** | delete fields |
|               |              |                 |            | **RPOP key** | pop tail |               |             |


