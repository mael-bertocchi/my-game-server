# Server protocol of R-Type Game

This document describes how the server and the client communicate in the R-Type Game.

## General overview

We use binary data to communicate between the client and the server. Each message sent from the client to the server or from the server to the client starts with one byte that represent the action type. Followed by four bytes that represent the size of the data. Finally, the data itself.

## Actions

All the actions are composed of the header, which is 1 byte for the action type and 4 bytes for the size of the data, followed by the data itself.

Here is the list of actions:

| Action Type |          Code         | Protocol | Authenticated  |
|-------------|-----------------------|----------|----------------|
| 0x01        | [ERR](actions/ERR.md) | TCP      | No             |
| 0x02        | [DFY](actions/DFY.md) | TCP      | No             |
| 0x03        | [OVE](actions/OVE.md) | UDP      | Yes            |
| 0x04        | [POS](actions/POS.md) | UDP      | Yes            |
| 0x05        | [JON](actions/JON.md) | TCP      | Yes            |
| 0x06        | [LVE](actions/LVE.md) | TCP      | Yes            |
| 0x07        | [SPW](actions/SPW.md) | UDP      | Yes            |
| 0x08        | [SHT](actions/SHT.md) | UDP      | Yes            |
| 0x09        | [DIE](actions/DIE.md) | UDP      | Yes            |
| 0x0A        | [CRE](actions/CRE.md) | TCP      | Yes            |
| 0x0B        | [STR](actions/STR.md) | TCP      | Yes            |
| 0x0C        | [STP](actions/STP.md) | TCP      | Yes            |
| 0x0D        | [STS](actions/STS.md) | UDP      | Yes            |
| 0x0E        | [NXT](actions/NXT.md) | TCP      | Yes            |
| 0x0F        | [GOD](actions/GOD.md) | TCP      | Yes            |

## World dimensions

The server assume the world is 900 pixels wide and 600 pixels high. The origin (0, 0) is at the top-left corner of the world.
