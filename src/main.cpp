#include <iostream>
#include <vector>
#include "httplib.h"
#include <bsoncxx/json.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/uri.hpp>
int main(){

    mongocxx::instance instance{};

    mongocxx client client{
        mongocxx::uri{
            std::string("mongodb://") + 
            (std::getenv("MONGODB_HOST")? std::getenv("MONGODB_HOST") : "localhost") + ":27017" 
        }
    };

    auto db = client["keploy"];
    auto collection = db["items"];

    httplib::Server server;

    server.Get("/health", [](const httplib::Request&, httplib::Response& res) {
        res.set_content(R"({"status": "ok"})", "application/json");
    });

    erver.Post("/items", [&](const httplib::Request& req, httplib::Response& res) {
        auto doc = bsoncxx::from_json(req.body);
        collection.insert_one(doc.view());
        res.set_content(R"({"message":"Item created"})", "application/json");
    });

    // List items
    server.Get("/items", [&](const httplib::Request&, httplib::Response& res) {
        std::string result = "[";
        bool first = true;

        for (auto&& doc : collection.find({})) {
            if (!first) result += ",";
            first = false;
            result += bsoncxx::to_json(doc);
        }
        result += "]";

        res.set_content(result, "application/json");
    });

    std::cout << "Server running on port 8080\n";
    server.listen("0.0.0.0", 8080);
    
    return 0;
}