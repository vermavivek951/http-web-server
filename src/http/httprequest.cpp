#pragma once
#include<string>
#include<map>
#include <stdexcept>
#include <sstream>
#include <iostream>

struct HttpRequest {
    std::string method;
    std::string path;
    std::string version;
    std::map<std::string, std::string> headers;
    std::string body;
};

 
HttpRequest parseHttpRequest(const std::string& rawRequest) {
    HttpRequest request;
    std::istringstream stream(rawRequest);
    std::string line;

    if(!std::getline(stream , line) || line.empty()) {
        throw std::runtime_error("Malformed request: Empty request line");
    }


    std::istringstream requestLine(line);
    requestLine >> request.method >> request.path >> request.version;

    if(request.method.empty() || request.path.empty() || request.version.empty()) {
        throw std::runtime_error("Malformed request line");
    }


    while(std::getline(stream , line) && line != "\r") {
        size_t colon = line.find(':');
        if(colon != std::string::npos) {
            std::string key = line.substr(0,colon);
            std::string value = line.substr(colon + 1);

            key.erase(key.find_last_not_of(" \t\r\n") + 1);
            value.erase(0 , value.find_first_not_of(" \t\r\n"));
            value.erase(value.find_last_not_of(" \t\r\n") + 1);

            request.headers[key] = value;
        }
    }

    while(std::getline(stream , line)) {
        request.body+=line;
    }
    return request;
}


std::string makeHttpResponse(const std::string& status , const std::string& contentType , const std::string& body) {
    std::ostringstream response;
    response << "HTTP/1.1 " << status << "\r\n"
             << "Content-Length: " << body.size() << "\r\n"
             << "Content-Type: " << contentType << "\r\n"
             << "\r\n"
             << body;

    return response.str();
}