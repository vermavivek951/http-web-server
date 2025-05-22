#include<iostream>
#include<winsock2.h> //provides socket APIs.
#include<ws2tcpip.h> //provides some sort of extended functionality like inet_pton.

int main() {
    std::cout << "running program\n";
    WSADATA wsaData;
    SOCKET serverSocket;

    //Before using socket; we need to initialize winsock;
    int wsaResult = WSAStartup(MAKEWORD(2,2) , &wsaData);
    if(wsaResult != 0) {
        std::cerr << "WSAStartup failed. Error: " << wsaResult << "\n";
        return 1;
    }
    
    //Now we create a socket using the socket() API.
    serverSocket = socket(AF_INET , SOCK_STREAM , 0);
    if(serverSocket == INVALID_SOCKET) {
        std::cerr << "Socket creation failed. Error: " << WSAGetLastError() << "\n";
        WSACleanup();
        return 1;
    }

    
    // Then we bind the socket to an IP/Port
    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY; //Accept connections from any address;
    serverAddr.sin_port = htons(8080); // PORT 8080


    if(bind(serverSocket , (sockaddr*)&serverAddr , sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Bind failed. Error: " << WSAGetLastError() << "\n";
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    if(listen(serverSocket , SOMAXCONN) == SOCKET_ERROR) {
        std::cerr << "Listen failed. Error: " << WSAGetLastError() << "\n";
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    std::cout << "Server listening on port 8080..." << "\n";

    while(true) {
        //Accept a client connection
        sockaddr_in clientAddr{};
        int clientSize = sizeof(clientAddr);
        SOCKET clientSocket = accept(serverSocket , (sockaddr*)&clientAddr , &clientSize);

        if(clientSocket == INVALID_SOCKET) {
            std::cerr << "Accept failed. Error: " << WSAGetLastError() << "\n";
            continue;
        }

        std::cout << "Accepted a new connection." << "\n";

        //Read a raw HTTP request
        char buffer[1024];
        int bytesReceived = recv(clientSocket , buffer , sizeof(buffer) -1 , 0);
        if(bytesReceived > 0) {
            buffer[bytesReceived] = '\0';
            std::cout << "Request:\n" << buffer << "\n";

            //Send a basic HTTP response
            const char* httpResponse = 
                "HTTP/1.1 200 OK\r\n"
                "Content-Type: text/plain\r\n"
                "Content-Length: 11\r\n"
                "\r\n"
                "Hello World";

            send(clientSocket , httpResponse , strlen(httpResponse) , 0);
        }
        closesocket(clientSocket);
    }
    


    closesocket(serverSocket);
    WSACleanup();

    return 0;
}