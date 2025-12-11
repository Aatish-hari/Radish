# Radish
Redis like in memory data store server implemented in C++. Radish can perform Redis functionality for strings, lists, and hashes, with auto Persistence and multiple client support.


## Features
### Protocol Support: Handles RESP (Redis Serialization Protocol) for commands and responses.
### Data Types: Strings (key-value), Lists (push/pop from ends), Hashes (field-value maps).
### Persistence: Automatic save/load to/from redi.db (plain-text format).
### Concurrency: Thread-safe with mutexes; one thread per client connection.

## Supported Commands
| General Commands | String Commands | List Commands | Hash Commands |
|------------------|-----------------|---------------|---------------|
| SING             | SET key value   | LLEN key      | HSET key field value |
| PING             | GET key         | LPUSH key elem | HGET key field |
| ECHO msg         | DEL key         | RPUSH key elem | HGETALL key |
| FLUSHALL         | KEYS            | LPOP key      | HDEL key field |
|                  |                 | RPOP key      |               |

