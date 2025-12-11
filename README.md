# Radish
Redis like in memory data store server implemented in C++. Radish can perform Redis functionality for strings, lists, and hashes, with auto Persistence and multiple client support.


## Features
### Protocol Support: Handles RESP (Redis Serialization Protocol) for commands and responses.
### Data Types: Strings (key-value), Lists (push/pop from ends), Hashes (field-value maps).
### Persistence: Automatic save/load to/from redi.db (plain-text format).
### Concurrency: Thread-safe with mutexes; one thread per client connection.

## Supported Commands
+----------------------------+------------------------------+------------------------------+--------------------------------------+
|      GENERAL COMMANDS      |      STRING COMMANDS         |       LIST COMMANDS          |            HASH COMMANDS             |
+----------------------------+------------------------------+------------------------------+--------------------------------------+
| SING                       | SET <key> <value>            | LLEN <key>                   | HSET <key> <field> <value>...        |
| (returns "SONG")           | (store string value)         | (list size)                  | (set hash fields)                    |
+----------------------------+------------------------------+------------------------------+--------------------------------------+
| PING                       | GET <key>                    | LPUSH <key> <elem...>        | HGET <key> <field>                   |
| (suggests trying sing)     | (get stored value)           | (push to head)               | (get field value)                    |
+----------------------------+------------------------------+------------------------------+--------------------------------------+
| ECHO <msg>                 | DEL <key>                    | RPUSH <key> <elem...>        | HGETALL <key>                         |
| (return message)           | (delete a key)               | (push to tail)               | (all fields+values)                  |
+----------------------------+------------------------------+------------------------------+--------------------------------------+
| FLUSHALL                   | KEYS                         | LPOP <key>                   | HDEL <key> <field>...                |
| (clear all data)           | (list all keys)              | (pop from head)              | (remove fields)                      |
+----------------------------+------------------------------+------------------------------+--------------------------------------+
|                            |                              | RPOP <key>                   |                                      |
|                            |                              | (pop from tail)              |                                      |
+----------------------------+------------------------------+------------------------------+--------------------------------------+

