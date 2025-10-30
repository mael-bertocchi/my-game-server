# The die action

This action is sent by the server to notify the client that an entity has died.

Entity values:

| Entity         | Value | Description                  |
|----------------|-------|------------------------------|
| Player Missile | 0x00  | A missile fired by a player  |
| Enemy Missile  | 0x01  | A missile fired by an enemy  |
| Force Missile  | 0x02  | A missile with special force |
| Generic Enemy  | 0x03  | The default enemy type       |
| Walking Enemy  | 0x04  | An enemy that walks          |
| Flying Enemy   | 0x05  | An enemy that flies          |
| Shield         | 0x06  | A shield item                |
| Force          | 0x07  | A force item                 |

Message structure:

| Code   | Size    | Entity                                        | Identifier                                              |
|--------|---------|-----------------------------------------------|---------------------------------------------------------|
| 1 byte | 4 bytes | 1 byte — The type of entity (see table above) | 4 bytes — Identifier of the entity as unsigned integer  |
