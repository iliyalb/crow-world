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

bool startsWith(const std::string& str, const std::string& prefix) {
    return str.find(prefix) == 0;
}

int main() {
    crow::SimpleApp app;

    CROW_ROUTE(app, "/")([]{
        return readFile("index.html");
    });

    CROW_ROUTE(app, "/<path>")([](const std::string& path) {
        std::string fullPath;

        if (startsWith(path, "asset/")) {
            fullPath = path;
        } else {
            fullPath = path;  // Serve from root or asset
        }

        return readFile(fullPath);
    });

    app.port(18080).multithreaded().run();
}
