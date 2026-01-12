#include <iostream>
#include "httplib.h"

#include <bson/bson.h>
#include <mongoc/mongoc.h>

int main() {
    mongoc_init();

    const char* mongo_host = std::getenv("MONGO_HOST");
    if (!mongo_host) mongo_host = "localhost";

    std::string uri_str = "mongodb://" + std::string(mongo_host) + ":27017";
    mongoc_client_t* client = mongoc_client_new(uri_str.c_str());

    mongoc_collection_t* collection =
        mongoc_client_get_collection(client, "keploy", "items");

    httplib::Server server;

    server.Get("/health", [](const httplib::Request&, httplib::Response& res) {
        res.set_content(R"({"status":"ok"})", "application/json");
    });

    server.Post("/items", [&](const httplib::Request& req, httplib::Response& res) {
        bson_error_t error;
        bson_t* doc = bson_new_from_json(
            (const uint8_t*)req.body.c_str(), req.body.size(), &error
        );

        if (!doc) {
            res.status = 400;
            res.set_content(R"({"error":"Invalid JSON"})", "application/json");
            return;
        }

        mongoc_collection_insert_one(collection, doc, nullptr, nullptr, &error);
        bson_destroy(doc);

        res.set_content(R"({"message":"Item created"})", "application/json");
    });

    server.Get("/items", [&](const httplib::Request&, httplib::Response& res) {
        bson_t query;
        bson_init(&query);

        mongoc_cursor_t* cursor =
            mongoc_collection_find_with_opts(collection, &query, nullptr, nullptr);

        std::string result = "[";
        bool first = true;
        const bson_t* doc;

        while (mongoc_cursor_next(cursor, &doc)) {
            if (!first) result += ",";
            first = false;

            char* json = bson_as_canonical_extended_json(doc, nullptr);
            result += json;
            bson_free(json);
        }

        result += "]";
        bson_destroy(&query);
        mongoc_cursor_destroy(cursor);

        res.set_content(result, "application/json");
    });

    std::cout << "Server running on port 8080\n";
    server.listen("0.0.0.0", 8080);

    mongoc_collection_destroy(collection);
    mongoc_client_destroy(client);
    mongoc_cleanup();

    return 0;
}
