# The spawn action

This action is used to notify clients about the spawning of new entities in the game world.

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
| Player         | 0x08  | A player character           |

Message structure:

| Code   | Size    | Entity                                        | Identifier                                              | Position                                                                         |
|--------|---------|-----------------------------------------------|---------------------------------------------------------|----------------------------------------------------------------------------------|
| 1 byte | 4 bytes | 1 byte — The type of entity (see table above) | 4 bytes — Identifier of the entity as unsigned integer  | 4 bytes — Two 2-byte unsigned integers representing the entity's position (x, y) |
