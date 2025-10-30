# The movement action

This action is used to handle player movement requests.

Sent by the client to the server to move in a specific direction.

Direction values:

| Direction   | Value | Description                |
|-------------|-------|----------------------------|
| Down Right  | 0x00  | Move down and to the right |
| Down Left   | 0x01  | Move down and to the left  |
| Up Right    | 0x02  | Move up and to the right   |
| Up Left     | 0x03  | Move up and to the left    |
| Right       | 0x04  | Move right                 |
| Down        | 0x05  | Move down                  |
| Left        | 0x06  | Move left                  |
| Up          | 0x07  | Move up                    |

Message structure:

| Code   | Size    | Direction                                  |
|--------|---------|--------------------------------------------|
| 1 byte | 4 bytes | 1 byte — Direction value (see table below) |
