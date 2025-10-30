# The join action

This action is triggered when a player deals with joining a game. It has multiple purposes.

1. Sent by the client to the server to request joining a game.

Message structure:

| Code   | Size    | Game identifier                                             |
|--------|---------|-------------------------------------------------------------|
| 1 byte | 4 bytes | 4 bytes — Unique identifier of the game as unsigned integer |

2. Sent by the server to each player in the game to notify them that a new player has joined and to the new player to inform them of their position.

Message structure:

| Code   | Size    | Player identifier                                                        |
|--------|---------|--------------------------------------------------------------------------|
| 1 byte | 4 bytes | 4 bytes — Unique identifier of the player who joined as unsigned integer |
