#include "html_shared.h"
#include <string>

std::string generateHeader(const std::string& title, const std::string& activeNav) {
    return R"(<!doctype html>
<html lang="en">
    <head>
        <meta charset="UTF-8" />
        <meta name="viewport" content="width=device-width, initial-scale=1.0" />
        <meta http-equiv="X-UA-Compatible" content="ie=edge" />
        <title>)" + title + R"(</title>
        <link rel="stylesheet" href="./style.css" />
        <link rel="icon" href="./favicon.ico" type="image/x-icon" />
    </head>
    <body>
        <header>
            )" + generateNavigation(activeNav) + R"(
        </header>)";
}

std::string generateNavigation(const std::string& activeNav) {
    std::string html = R"(<nav class="main-nav">
                <a href="/" class="nav-link)";

    if (activeNav == "home") {
        html += " active";
    }

    html += R"(">home</a>
                <a href="/gallery" class="nav-link)";

    if (activeNav == "gallery") {
        html += " active";
    }

    html += R"(">gallery</a>
                <img class="logo" src="lambda.png" alt="λ" width="40" />
            </nav>)";

    return html;
}

std::string generateFooter() {
    return R"(
            <footer>
                <p>copyleft 🄯 iliya lesani</p>
            </footer>
            <a href="#" class="back-to-top">⇡</a>
            <script src="index.js"></script>
        </body>
    </html>)";
}

std::string generatePageWrapper(const std::string& title, const std::string& activeNav, const std::string& content) {
    return generateHeader(title, activeNav) + content + generateFooter();
}

bool isUrl(const std::string& str) {
    return str.substr(0, 7) == "http://" || str.substr(0, 8) == "https://";
}
