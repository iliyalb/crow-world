#include "html_shared.hpp"
#include <string>

std::string generateHeader(const std::string& title, const std::string& activeNav) {
    return R"(<!doctype html>
<html lang="en">
    <head>
        <meta charset="UTF-8" />
        <meta name="viewport" content="width=device-width, initial-scale=1.0" />
        <meta http-equiv="X-UA-Compatible" content="ie=edge" />
        <meta name="robots" content="index,follow" />
        <meta name="language" content="English">
        <meta name="description" content="iliya's schizoposting microblog on everything half-life">
        <meta name="keywords" content="iliya lesani, gldsrc, source engine, personal site, modding">
        <meta name="license" content="Unlicense">
        <meta name="dc.rights" content="No Rights Reserved">
        <link rel="license" href="https://unlicense.org/">
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
                <p>anti-copyright <s>©</s> iliya lesani</p>
                <p>
                    All rights released to the <b>public domain</b> via the
                    <a href="https://unlicense.org/" rel="license">
                       Unlicense
                    </a>
                </p>
                <div id="cookie-banner" class="cookie-banner">
                    <p>This site uses cookies. Who ate all the donuts...?
                       <a href="#" id="cookie-learn-more">Learn More</a></p>
                    <div class="cookie-buttons">
                        <button id="accept-cookies">OK</button>
                        <button id="reject-cookies">NO!</button>
                    </div>
                </div>
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
