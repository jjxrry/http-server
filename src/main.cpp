#include <iostream>
#include <string>
#include <sstream>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    int serverFileDescriptor, clientSocket;
    struct sockaddr_in serverAddress;
    int option = 1;
    int addressLength = sizeof(serverAddress);

    const int PORT = 3500;
    char buffer[1024] = {0};

    if ((serverFileDescriptor = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket Creation Failed");
        exit(EXIT_FAILURE);
    }

    // forcefully attach to port
    if (setsockopt(serverFileDescriptor, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option))) {
        perror("Setsockopt failed");
        close(serverFileDescriptor);
        exit(EXIT_FAILURE);
    }

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(PORT);

        // Binding the socket to the port
    if (bind(serverFileDescriptor, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0) {
        perror("Bind failed");
        close(serverFileDescriptor);
        exit(EXIT_FAILURE);
    }

    // Listening for incoming connections
    if (listen(serverFileDescriptor, 3) < 0) {
        perror("Listen failed");
        close(serverFileDescriptor);
        exit(EXIT_FAILURE);
    }

    std::cout << "Server is listening on port " << PORT << std::endl;

    while (true) {
        if ((clientSocket = accept(serverFileDescriptor, (struct sockaddr *)&serverAddress, (socklen_t*)&addressLength)) < 0) {
            perror("Accept failed");
            close(serverFileDescriptor);
            exit(EXIT_FAILURE);
        }

        read(clientSocket, buffer, 1024);
        
        // Simple HTTP response
        std::ostringstream responseStream;
        std::string body = "The HTTP Server is alive!!\r\nHello, world!";
        responseStream << "HTTP/1.1 200 OK\r\n";
        responseStream << "Content-Type: text/html\r\n";
        responseStream << "Content-Length: " << body.length() << "\r\n";
        responseStream << "\r\n";
        responseStream << body;

        std::string response = responseStream.str();
        send(clientSocket, response.c_str(), response.length(), 0);

        close(clientSocket);
    }

    close(serverFileDescriptor);
    return 0;
}
