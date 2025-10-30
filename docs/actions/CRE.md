# The create game action

This action is used to create game. It has multiple purposes.

1. Sent by the client to the server to request the creation of a new game.

Message structure:

| Code   | Size    |
|--------|---------|
| 1 byte | 4 bytes |

2. Sent by the server to notify all the client that the game has been created.

Message structure:

| Code   | Size    | Identifier                                                           |
|--------|---------|---------------------------------------------------------------------|
| 1 byte | 4 bytes | 4 bytes — Unique identifier assigned to the game as unsigned integer |
