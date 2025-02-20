# CPSC328-project3
# Simple Web Client

## Overview
This project implements a **simple HTTP client** that makes **HTTP GET requests** using **sockets**. The client connects to a specified host and port, requests a resource, and prints the response to standard output. The program supports command-line arguments to specify the host, port, file path, and verbosity mode.

## Features
- Connects to a web server via **sockets**.
- Sends **HTTP GET requests**.
- Receives and displays the **HTTP response**.
- Supports **custom host, port, and file path**.
- Verbose mode to **display the full request**.

## Usage
### Compilation
To compile the project, use the provided **Makefile**:
```bash
make
```
This will generate an executable named `webclient`.

### Running the Program
Run the program with the following command-line arguments:
```bash
./webclient -h <host> -p <port> -f <file_path> [-v]
```
#### Example:
```bash
./webclient -h example.com -p 80 -f /index.html -v
```
This sends an HTTP GET request to `example.com` on port `80` for `/index.html`, with verbose mode enabled.

### Cleaning Up
To remove compiled files:
```bash
make clean
```

## Implementation Details
### Steps Performed:
1. **Create a socket** using `getaddrinfo` and `socket()`.
2. **Connect to the server** using `connect()`.
3. **Build the HTTP request**:
   - Uses `GET` method.
   - Includes `Host` header.
   - Ends with a **CRLF sequence** (`\r\n\r\n`).
4. **Send the request** using `send()`.
5. **Receive the response** using `recv()` and print it.
6. **Close the socket**.

### Command-line Arguments:
- `-h <host>`: Specify the host (default: `localhost`).
- `-p <port>`: Specify the port (default: `80`).
- `-f <file_path>`: Specify the requested file (default: `/`).
- `-v`: Enable verbose mode (prints the HTTP request).

## Code Structure
- `webclient.cpp` - Main program handling socket creation, connection, request sending, and response receiving.
- `Makefile` - Compilation and cleanup rules.

## Error Handling
- Ensures **valid command-line arguments**.
- Handles **socket creation and connection failures**.
- Handles **send/receive errors**.

## Author
- **Justin Allen**
