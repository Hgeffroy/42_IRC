#include <unistd.h>
#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

int main() {
    // Create a socket
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket < 0) {
        std::cerr << "Error creating socket" << std::endl;
        return 1;
    }

    // Set up the server address structure
    struct sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(80); // HTTPS port
    struct hostent *server = gethostbyname("www.boredapi.com");
    if (server == NULL) {
        std::cerr << "Error resolving hostname" << std::endl;
        return 1;
    }
    memcpy(&serverAddress.sin_addr.s_addr, server->h_addr, server->h_length);

    // Connect to the server
    if (connect(clientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {
        std::cerr << "Error connecting to server" << std::endl;
        return 1;
    }

    // Send HTTP GET request
    const char* getRequest = "GET /documentation HTTP/1.1\r\nHost: www.boredapi.com\r\n\r\n";
    if (send(clientSocket, getRequest, strlen(getRequest), 0) < 0) {
        std::cerr << "Error sending request" << std::endl;
        return 1;
    }

    // Receive and print the response
    char buffer[8192];
    memset(buffer, 0, sizeof(buffer));
    int bytesRead = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
    if (bytesRead < 0) {
        std::cerr << "Error receiving response" << std::endl;
        return 1;
    }

    std::cout << "Response:\n-" << buffer << "-" << std::endl;

    // Close the socket
    close(clientSocket);

    return 0;
}