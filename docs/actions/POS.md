# The position action

This action is used to broadcast entity positions to all connected clients.

Sent by the server to notify all clients of the entity current positions.

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
| Player         | 0x0A  | The player entity            |

Message structure:

| Code   | Size    | Count                                                                 | Entity                                        | Identifier                                             | Position                                                                         |
|--------|---------|-----------------------------------------------------------------------|-----------------------------------------------|-------------------------------------------------------|----------------------------------------------------------------------------------|
| 1 byte | 4 bytes | 2 bytes — The number of positions in the message as unsigned integer  | 1 byte — The type of entity (see table above) | 4 bytes — Identifier of the entity as unsigned integer | 4 bytes — Two 2-byte unsigned integers representing the player's position (x, y) |

The `Count` field indicates how many position updates are included in the message. The `Entity`, `Identifier`, and `Position` fields are repeated `Count` times. The `Size` field represents the total size of the message body.
