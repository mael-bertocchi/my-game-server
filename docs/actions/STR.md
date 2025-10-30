# The start game action

This action is used to start the game. It has multiple purposes.

1. Sent by the server to notify clients that the game is starting.

Message structure:

| Code   | Size    | Count   | The identifier of the player                                  | The position of the player                                                       |
|--------|---------|---------|--------------------------------------------------------------|----------------------------------------------------------------------------------|
| 1 byte | 4 bytes | 1 byte  | 4 bytes — Unique identifier of the player as unsigned integer | 4 bytes — Two 2-byte unsigned integers representing the player's position (x, y) |

The `Count` field indicates how many players are included in the message. The `Identifier` and `Position` fields are repeated `Count` times. The `Size` field represents the total size of the message body.

2. Sent by clients to the server to indicate they want to start the game.

Message structure:

| Code   | Size    |
|--------|---------|
| 1 byte | 4 bytes |
