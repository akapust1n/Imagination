#include "../3rd_part/md5/md5.h"
#include "Matchmaking.h"
#include "crow.h"
using namespace crow;

int main()
{
    crow::SimpleApp app;
    Matchmaking mk;
    CROW_ROUTE(app, "/")
    ([]() {
        response res;
       // res.add_header("Access-Control-Allow-Origin", "*");
        res.body = "Hello world!";
        return res;
    });

    CROW_ROUTE(app, "/ws")
        .websocket()
        .onopen([&](crow::websocket::connection& conn) {
            CROW_LOG_INFO << "new websocket connection";
            mk.addPlayer({ &conn });

        })
        .onclose([&](crow::websocket::connection& conn, const std::string& reason) {
            CROW_LOG_INFO << "websocket connection closed: " << reason;

        })
        .onmessage([&](crow::websocket::connection& conn, const std::string& data, bool is_binary) {
            CROW_LOG_INFO << "connection message";

            conn.send_text(data);
        });

    app.port(5001).multithreaded().run();
}
