# The spawn action

This action is used to notify clients about the spawning of new entities in the game world.

Entity values:

| Entity         | Value | Description                  |
|----------------|-------|------------------------------|
| Player Missile | 0x00  | A missile fired by a player  |
| Enemy Missile  | 0x01  | A missile fired by an enemy  |
| Force Missile  | 0x02  | A missile with special force |
| Boss Missile   | 0x03  | A missile fired by a boss    |
| Generic Enemy  | 0x04  | The default enemy type       |
| Walking Enemy  | 0x05  | An enemy that walks          |
| Flying Enemy   | 0x06  | An enemy that flies          |
| Boss Enemy     | 0x07  | A boss enemy                 |
| Shield         | 0x08  | A shield item                |
| Force          | 0x09  | A force item                 |

Message structure:

| Code   | Size    | Entity                                        | Identifier                                              | Position                                                                         |
|--------|---------|-----------------------------------------------|---------------------------------------------------------|----------------------------------------------------------------------------------|
| 1 byte | 4 bytes | 1 byte — The type of entity (see table above) | 4 bytes — Identifier of the entity as unsigned integer  | 4 bytes — Two 2-byte unsigned integers representing the entity's position (x, y) |
