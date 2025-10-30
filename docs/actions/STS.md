# The statistics action

This action is used to tell all the players of the game that someone has changed one of its statistics.

Status values:

| Status        | Value | Description                           |
|---------------|-------|---------------------------------------|
| Statistic off | 0x00  | The player's statistic is deactivated |
| Statistic on  | 0x01  | The player's statistic is activated   |

Statistics values:

| Statistic      | Value | Description                          |
|----------------|-------|--------------------------------------|
| Shield         | 0x00  | The player's shield statistic        |
| Force          | 0x01  | The player's force statistic         |

Message structure:

| Code   | Size    | Identifier                                              | Statistic                                        | Status                                      |
|--------|---------|---------------------------------------------------------|--------------------------------------------------|---------------------------------------------|
| 1 byte | 4 bytes | 4 bytes — Identifier of the entity as unsigned integer  | 1 byte — Statistic value (see Statistics values) | | 1 byte - Status value (see Status values) |
