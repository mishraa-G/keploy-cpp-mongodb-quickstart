#include <iostream>
#include "httplib.h"
int main(){
    httplib::Server server;

    server.Get("/health", [](const httplib::Request&, httplib::Response& res) {
        res.set_content(R"({"status": "ok"})", "application/json");
    });

    std::cout << "Server running on port 8080\n";
    server.listen("0.0.0.0", 8080);
    
    return 0;
}