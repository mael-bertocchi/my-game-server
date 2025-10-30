# The stop action

This action is used to notify the player that the game has been finished and they should stop playing.

We have two possible states for this action:

| State        | Value | Description                        |
|--------------|-------|------------------------------------|
| Lose         | 0x00  | All the players have been defeated |
| Win          | 0x01  | There is at least one player alive |

Message structure:

| Code   | Size    |
|--------|---------|
