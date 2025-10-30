# The error action

This action is used to notify a player of an error that has occurred.

Message structure:

| Code   | Size    | Message                                               |
|--------|---------|-------------------------------------------------------|
| 1 byte | 4 bytes | N bytes — UTF-8 string representing the error message |
