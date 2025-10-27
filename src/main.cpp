#include "crow.h"
#include <fstream>
#include <sstream>

std::string readFile(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open())
        return "file not found";

    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

int main() {
    crow::SimpleApp app;

    CROW_ROUTE(app, "/")([]{
        return readFile("index.html");
    });

    CROW_ROUTE(app, "/<string>")([](const std::string& filePath) {
        return readFile(filePath);
    });

    app.port(18080).multithreaded().run();
}
