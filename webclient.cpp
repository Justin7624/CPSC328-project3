/***************************************************************
Author:         Justin Allen
Major:          Computer Science
Creation Date:  January 24, 2025
Due Date:       TBA
Course:         CPSC 328
Professor:      Schwesinger
Assignment:     #3
Filename:       webclient.cpp
Purpose:        
    This program implements a simple HTTP client that makes HTTP GET 
    requests using sockets. It connects to a specified host and port, 
    requests a resource, and prints the response to standard output. 
    The program supports command-line arguments to specify the host, 
    port, file path, and verbosity mode.

***************************************************************/

#include <iostream>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <string>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <arpa/inet.h>

/***************************************************************
Function:       build_http_request
Description:    Constructs an HTTP GET request string.

Parameters:     
    const std::string &host - The host to which the request is sent.
    const std::string &file_path - The file path to request.
    bool verbose - If true, prints the constructed HTTP request.
    std::string &request - Reference to store the final request string.

Return Value:   void - The function modifies the request string in place.

Citations:
- HTTP Request Format: https://developer.mozilla.org/en-US/docs/Web/HTTP/Messages
***************************************************************/
void build_http_request(const std::string &host, const std::string &file_path, bool verbose, std::string &request) {
    request = "GET " + file_path + " HTTP/1.1\r\n";
    request += "Host: " + host + "\r\n";
    request += "Connection: close\r\n\r\n";

    if (verbose) {
        std::cout << "HTTP Request:\n" << request << std::endl;
    }
}

/***************************************************************
Function:       main
Description:    Main function to parse command-line arguments, 
                establish a socket connection, send an HTTP request,
                receive a response, and display it.

Parameters:     
    int argc - The number of command-line arguments.
    char* argv[] - An array of C-style strings representing the arguments.

Return Value:   int - Returns 0 upon successful execution, 
                or 1 if an error occurs.

Citations:
- Socket Programming: https://beej.us/guide/bgnet/html/
- getaddrinfo: https://man7.org/linux/man-pages/man3/getaddrinfo.3.html
- send/recv: https://man7.org/linux/man-pages/man2/send.2.html
***************************************************************/
int main(int argc, char *argv[]) {
    // Default arguments
    std::string host = "localhost";
    std::string port = "80";
    std::string file_path = "/";
    bool verbose = false;

    // Parse command-line arguments
    for (int i = 1; i < argc; ++i) {
        if (std::string(argv[i]) == "-h" && i + 1 < argc) {
            host = argv[++i];
        } else if (std::string(argv[i]) == "-p" && i + 1 < argc) {
            port = argv[++i];
        } else if (std::string(argv[i]) == "-f" && i + 1 < argc) {
            file_path = argv[++i];
        } else if (std::string(argv[i]) == "-v") {
            verbose = true;
        } else {
            std::cerr << "Usage: " << argv[0] << " [-h host] [-p port] [-f file] [-v]\n";
            return 1;
        }
    }

    // Get address info
    addrinfo hints{}, *res;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if (getaddrinfo(host.c_str(), port.c_str(), &hints, &res) != 0) {
        perror("Error getting address info");
        return 1;
    }

    // Create and connect socket
    int sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (sockfd == -1) {
        perror("Error creating socket");
        freeaddrinfo(res);
        return 1;
    }

    if (connect(sockfd, res->ai_addr, res->ai_addrlen) == -1) {
        perror("Error connecting socket");
        close(sockfd);
        freeaddrinfo(res);
        return 1;
    }

    freeaddrinfo(res);

    // Build and send HTTP request
    std::string request;
    build_http_request(host, file_path, verbose, request);

    if (send(sockfd, request.c_str(), request.size(), 0) == -1) {
        perror("Error sending request");
        close(sockfd);
        return 1;
    }

    // Receive HTTP response
    char buffer[4096];
    ssize_t bytes_received;
    std::cout << "HTTP Response:\n";
    while ((bytes_received = recv(sockfd, buffer, sizeof(buffer) - 1, 0)) > 0) {
        buffer[bytes_received] = '\0';
        std::cout << buffer;
    }

    if (bytes_received == -1) {
        perror("Error receiving response");
    }

    close(sockfd);
    return 0;
}
