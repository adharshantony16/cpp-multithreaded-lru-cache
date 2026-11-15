# Event-Driven HTTP Server with Linux epoll

A lightweight, high-performance concurrent HTTP server implemented in **C**, utilizing the Linux `epoll` event notification facility, non-blocking sockets, and an event-driven loop to handle multiple simultaneous client connections efficiently.

## 🚀 Features
* **Event-Driven Architecture:** Uses `epoll` (`epoll_create1`, `epoll_ctl`, `epoll_wait`) instead of traditional thread-per-connection models, enabling high concurrency with minimal CPU overhead.
* **Non-Blocking I/O:** Configures socket descriptors with `O_NONBLOCK` flags to prevent blocking during socket reads and writes.
* **Resource Safe:** Clean socket lifecycle management, file descriptor recycling, and robust error handling to prevent resource leaks under heavy stress testing.

## 🛠️ Tech Stack & Concepts
* **Language:** C (C11 standard)
* **System Calls / APIs:** Linux `epoll`, POSIX Sockets (`socket`, `bind`, `listen`, `accept`), `fcntl`
* **Networking Protocol:** HTTP/1.1 response framing (`200 OK`, headers, content length)

## ⚙️ How to Compile & Run
*Note: Because `epoll` is a native Linux system call, compile and run this on a Linux environment (or WSL on Windows).*

```bash
# Compile the server
gcc server.c -o server

# Run the server on port 8080
./server
