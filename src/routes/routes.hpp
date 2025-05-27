#pragma once
#include <string>
#include <functional>
#include <map>

#include "../http/httprequest.cpp"

std::map<std::string , std::function<std::string(const HttpRequest&)>> createRoutes() {
    return {
        { "/api/hello" , [](const HttpRequest& req) {
            return R"({"message":"Hello from API"})"; //R represents raw string-literal
        }},
        { "/api/time" , [](const HttpRequest& req) {
            time_t now = time(0);
            std::string timeStr = std::ctime(&now);
            timeStr.pop_back(); //remove newline
            return R"({"time" : ")" + timeStr + R"("})";
        }}
    };
}