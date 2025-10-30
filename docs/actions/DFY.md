# The identification action

This action is used to identify the client to the server. It has multiple purposes.

We have three states of identification:

| State                     | Code | Description                                                                           |
|---------------------------|------|---------------------------------------------------------------------------------------|
| Identification requested  | 0x00 | The server is requesting the client to identify itself.                               |
| Identification failed     | 0x01 | The client sent invalid or incomplete information. The server is requesting it again. |
| Identification successful | 0x02 | The client has been successfully identified.                                          |

1. Sent by the server to request the client to identify itself or denie access if the identification failed.

Message structure:

| Code   | Size    | State                        |
|--------|---------|------------------------------|
| 1 byte | 4 bytes | 1 byte — See the table above |

2. Sent by the server to notify the client that it connected successfully.

Message structure:

| Code   | Size    | State                            | Identifier                                                             |
|--------|---------|----------------------------------|------------------------------------------------------------------------|
| 1 byte | 4 bytes | 1 byte — The success status code | 4 bytes — Unique identifier assigned to the client as unsigned integer |

3. Sent by the client to the server to provide its identification information.

Message structure:

| Code   | Size    | Content                                                                                     |
|--------|---------|---------------------------------------------------------------------------------------------|
| 1 byte | 4 bytes | N bytes — UTF-8 string representing the client's username and password separated by a space |
