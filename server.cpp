#include<iostream>
#include<winsock2.h> //provides socket APIs.
#include<ws2tcpip.h> //provides some sort of extended functionality like inet_pton.
#include<fstream>
#include<filesystem>
#include<thread>

#include "httprequest.cpp"
#include "routes.hpp"

namespace fs = std::filesystem;

//helper function
bool endsWith(const std::string& str , const std::string& suffix) {
    if(suffix.size() > str.size()) return false;
    return std::equal(suffix.rbegin() , suffix.rend() , str.rbegin());
}

bool startsWith(const std::string &str , const std::string &prefix) {
    if(prefix.size() > str.size()) return false;
    return str.compare(0 , prefix.size() , prefix) == 0;
}

std::string getMimeType(const std::string& path) {
    if(endsWith(path, ".html")) return "text/html";
    if(endsWith(path, ".css")) return "text/css";
    if(endsWith(path, ".js")) return "application/javascript";
    if(endsWith(path, ".png")) return "image/png";
    if(endsWith(path, ".jpg") || endsWith(path , ".jpeg")) return "image/jpeg";

    return "text/plain";
}

std::string readFileContent(const std::string& filePath) {
    std::ifstream file(filePath , std::ios::binary);
    if(!file) return "";

    std::ostringstream ss;
    ss << file.rdbuf();
    return ss.str();
}


void handleClient(SOCKET clientSocket , const std::map<std::string, std::function<std::string(const HttpRequest&)>>& routes) {
    char buffer[1024];
    int bytesReceived = recv(clientSocket , buffer , sizeof(buffer) -1 , 0);
    if(bytesReceived > 0) {
        buffer[bytesReceived] = '\0';
        std::string rawRequest(buffer);

        try {
            HttpRequest req = parseHttpRequest(rawRequest);
            
            std::cout << "req.path: " << req.path << "\n";
            if(startsWith(req.path , "/api/")) {
                if(routes.find(req.path)!= routes.end()) {
                    std::string json = routes.at(req.path)(req);
                    std::string response = 
                    "HTTP/1.1 200 OK\r\n"
                    "Content-Type: application/json\r\n"
                    "Content-Length: " + std::to_string(json.size()) + "\r\n"
                    "\r\n" + json;
                    
                    send(clientSocket , response.c_str(), response.size() , 0);
                }
                else {
                    std::string notFound = R"({"error":"Not Found"})";
                    std::string response = 
                        "HTTP/1.1 404 Not Found\r\n"
                        "Content-Type: application/json\r\n"
                        "Content-Length: " + std::to_string(notFound.size()) + "\r\n"
                        "\r\n" + notFound;
                    
                    send(clientSocket , response.c_str() , response.size() , 0);
                }
            }
            else {
                //serving static file;
                std::string filePath = "." + req.path;
                std::cout << "filePath: " << filePath << "\n";
                if(req.path == "/") {
                    filePath = "./index.html";
                }

                if(fs::exists(filePath) && fs::is_regular_file(filePath)) {
                    std::string content = readFileContent(filePath);
                    std::string contentType = getMimeType(filePath);


                    std::ostringstream response;
                    response << "HTTP/1.1 200 OK\r\n"
                            << "Content-Type: " << contentType << "\r\n"
                            << "Content-Length: " << content.size() << "\r\n"
                            << "\r\n"
                            << content;

                    

                    std::string fullResponse = response.str();
                    send(clientSocket , fullResponse.c_str() , fullResponse.size() , 0);
                }
                else {
                    const char* notFound =
                        "HTTP/1.1 404 Not Found\r\n"
                        "Content-Type: text/plain\r\n"
                        "Content-Length: 13\r\n"
                        "\r\n"
                        "404 Not Found";
                    send(clientSocket, notFound, strlen(notFound), 0);
                }
            
            }
        } catch (const std::exception& e) {
            std::cerr << "Invalid request: " << e.what() << "\n";
            const char* badRequest =
                "HTTP/1.1 400 Bad Request\r\n"
                "Content-Type: text/plain\r\n"
                "Content-Length: 12\r\n"
                "\r\n"
                "Bad Request";
            send(clientSocket , badRequest , strlen(badRequest) , 0);
        }
    }
    closesocket(clientSocket);
}


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

    auto routes = createRoutes();

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
        std::thread clientThread(handleClient , clientSocket , std::cref(routes));
        clientThread.detach();
    }
    
    closesocket(serverSocket);
    WSACleanup();

    return 0;
}


