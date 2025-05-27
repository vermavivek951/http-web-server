#pragma once
#include <string>
#include <iostream>
#include <filesystem>
#include <fstream>
#include <sstream>


//helper function
void logRequest(const std::string& ip , const std::string& path , const std::string& status) {
    std::cout << "[LOG] IP: " << ip << " | Path: " << path << " | Status: " << status << "\n";
}

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