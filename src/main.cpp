#include <iostream>
#include <sstream>
#include <string>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstring>

// function declarations
int createServerSocket(int port);
void handleClient(int clientSocket);
void sendResponse(int clientSocket);

int main() {
    int serverSocket = createServerSocket(3050);
    if (serverSocket < 0){ //while exists
        std::cerr << "main server socket creation failed";
        return 1;
    }
    // create client socket
    while (true) {
        struct sockaddr_in clientAddr;
        socklen_t clientLen = sizeof(clientAddr);
        int clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientLen);

        if (clientSocket < 0){
            std::cerr << "failed to accept connetion\n";
            continue;
        }
        handleClient(clientSocket);
        close(clientSocket);
    }

    close(serverSocket);
    return 0;
}


int createServerSocket(int port) {
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0){ //while exists
        std::cerr << "socket creation failed";
        return 1;
    }

    struct sockaddr_in serverAddr;
    std::memset(&serverAddr, 0, sizeof(serverAddr)); //allocate memory
    // what is sin_ and/or sockaddr_in methods
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(port);

    // fill this
    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        std::cerr << "Binding failed\n";
        close(serverSocket);
        return -1;
    }

    if (listen(serverSocket, 5) < 0) {
        std::cerr << "Listening failed\n";
        close(serverSocket);
        return -1;
    }

    return serverSocket;
}

void handleClient(int clientSocket) {
    sendResponse(clientSocket);
}

void sendResponse(int clientSocket) {
    std::ostringstream responseStream;
    responseStream << "HTTP/1.1 200 OK\r\n";
    responseStream << "Content-Type: text/html\r\n";
    responseStream << "Content-Length: 13\r\n";
    responseStream << "\r\n";
    responseStream << "Hello, world!";

    std::string response = responseStream.str();
    send(clientSocket, response.c_str(), response.length(), 0);
}
