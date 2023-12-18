<center>

# ft_irc

</center>

## Table of Contents

- [About](#about)
- [Features](#features)
- [Prerequisites](#prerequisites)
- [Installation](#installation)
- [Usage](#usage)
- [Examples](#examples)
- [Resources](#resources)
- [Authors](#authors)

## About

The ft_irc project at 42 involves implementing an IRC (Internet Relay Chat) server and client using the C language. The goal is to create a real-time communication system, allowing multiple users to connect to discussion channels.

## Features
- [ ] Connect to the IRC server
- [ ] Create and manage discussion channels
- [ ] Send public and private messages
- [ ] Handle IRC commands (JOIN, PART, PRIVMSG, etc.)
- [ ] Implement the IRC protocol

## Prerequisites
- GCC (GNU Compiler Collection)
- ...

## Installation
1. Clone the repository: `git clone https://github.com/hgeffroy/irc.git`
2. Compile the project: `make`

## Usage
1. Start the server: `./server [port]`
2. Launch the client: `./client [IP address] [port]`

## Examples
Example of using the server and client.

```bash
# Terminal 1 - Start the server
$ ./server 6667
IRC server started on port 6667

# Terminal 2 - Connect a client
$ ./client 127.0.0.1 6667
Connected to the IRC server

# Terminal 2 - Join a channel
JOIN #general

# Terminal 1 - Public message on the channel
PRIVMSG #general :Hello, everyone!

# Terminal 2 - Leave the channel
PART #general
```

## Resources

- [Modern IRC Client Protocol](https://modern.ircdocs.horse/)
- [Building a simple server with C++](https://ncona.com/2019/04/building-a-simple-server-with-cpp/)
- [Client Documentation](https://hexchat.readthedocs.io/en/latest/)
- [User Commands](https://docs.oracle.com/cd/E86824_01/html/E54763/netcat-1.html)
- [Understanding sockets concepts](https://www.ibm.com/docs/en/zos/2.2.0?topic=concepts-understanding-sockets)
- [List of all IRC's commands](https://www.techbull.com/techbull/guide/internet/irccommande.html)

## Authors

- [Henri Geffroy](https://github.com/hgeffroy)
- [Arthur Ascedu](https://github.com/aascedu)
- [Thea Wang](https://github.com/Zwhea)
