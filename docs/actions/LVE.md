# The leave action

This action is triggered when a player leaves a game. It has multiple purposes.

1. Sent by the client to the server to request leaving a game.

Message structure:

| Code   | Size    |
|--------|---------|
| 1 byte | 4 bytes |

2. Sent by the server to each player in the game to notify them that a player has left.

Message structure:

| Code   | Size    | Player identifier                                                      |
|--------|---------|------------------------------------------------------------------------|
| 1 byte | 4 bytes | 4 bytes — Unique identifier of the player who left as unsigned integer |
