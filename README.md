# IRC Server

## Description

A simple IRC server implemented in C++.

## Features

*   **Create and join channels:** Use the `JOIN` command.
*   **Private messaging:** Use the `PRIVMSG` command to send messages to users or channels.
*   **Channel moderation:** `KICK` users and manage channel modes (`MODE`).
*   **Channel topics:** Set and view channel topics with the `TOPIC` command.
*   **Basic user commands:** `NICK`, `USER`.

## Getting Started

### Prerequisites

*   A C++ compiler that supports C++98 (e.g., `g++`).
*   `make`

### Installation

1.  **Clone the repository:**
    ```bash
    git clone <repository-url>
    cd <repository-directory>
    ```

2.  **Compile the server:**
    ```bash
    make
    ```
    This will create the `ircserver` executable in the root directory.

3.  **Run the server:**
    ```bash
    ./ircserver <port> <password>
    ```
    *   `<port>`: The port number the server should listen on (e.g., 6667).
    *   `<password>`: The password required for clients to connect.

## How to Connect

You can connect to the server using any IRC client (e.g., Irssi, HexChat, WeeChat).

1.  **Server Address:** `127.0.0.1` or `localhost`
2.  **Port:** The `<port>` you specified when running the server.
3.  **Password:** The `<password>` you specified.

**Example using Irssi:**

```
/connect 127.0.0.1 <port> <password>
```

## Authors

*   **yoropeza** - [yoropeza](https://github.com/yoropeza)