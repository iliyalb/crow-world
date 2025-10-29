#include "html_shared.hpp"
#include <string>
#include <regex>
#include <sstream>

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

static std::string escapeHtml(const std::string& input) {
    std::string out;
    out.reserve(input.size());
    for (char c : input) {
        switch (c) {
            case '&': out += "&amp;"; break;
            case '<': out += "&lt;"; break;
            case '>': out += "&gt;"; break;
            case '"': out += "&quot;"; break;
            case '\'': out += "&#39;"; break;
            default: out += c; break;
        }
    }
    return out;
}

static std::string allowHtmlBrOnly(const std::string& input) {
    // Convert escaped <br>, <br/>, <br /> back to real <br> tags, keep everything else escaped
    std::string out = input;
    out = std::regex_replace(out, std::regex(R"(&lt;br\s*/?&gt;)", std::regex::icase), "<br>");
    out = std::regex_replace(out, std::regex(R"(&lt;br\s*/\s*&gt;)", std::regex::icase), "<br>");
    return out;
}

// Markdown renderer: headings, paragraphs, lists, links, inline code, bold, italics
std::string renderMarkdown(const std::string& markdown) {
    try {
        std::stringstream input(markdown);
        std::string line;
        std::string html;
        bool inList = false;

        auto closeListIfOpen = [&]() {
            if (inList) {
                html += "</ul>";
                inList = false;
            }
        };

        // Precompiled regex patterns without lookbehind
        const std::regex reHeading(R"(^#{1,6}\s+(.*)$)");
        const std::regex reList(R"(^\s*[-\*]\s+(.*)$)");
        const std::regex reLink(R"(\[(.+?)\]\((.+?)\))");
        const std::regex reCode(R"(`([^`]+)`)");
        const std::regex reBoldStars(R"(\*\*([^\n*]+?)\*\*)");
        const std::regex reBoldUnders(R"(__([^\n_]+?)__)");
        const std::regex reItalStar(R"(\*([^\n*]+?)\*)");
        const std::regex reItalUnder(R"(_([^\n_]+?)_)");

        while (std::getline(input, line)) {
            // Trim carriage returns
            if (!line.empty() && line.back() == '\r') {
                line.pop_back();
            }

            if (line.empty()) {
                closeListIfOpen();
                html += "<br>"; // preserve blank line as break between paragraphs
                continue;
            }

            // Headings: # .. ######
            std::smatch m;
            if (std::regex_match(line, m, reHeading)) {
                closeListIfOpen();
                std::string content = m[1].str();
                size_t level = 0;
                while (level < line.size() && line[level] == '#') {
                    level++;
                }
                if (level < 1) {
                    level = 1;
                }
                if (level > 6) {
                    level = 6;
                }
                content = escapeHtml(content);
                content = allowHtmlBrOnly(content);
                // inline formatting after escaping
                content = std::regex_replace(content, reLink, R"(<a href="$2">$1</a>)");
                content = std::regex_replace(content, reCode, R"(<code>$1</code>)");
                // bold before italics to avoid conflict
                content = std::regex_replace(content, reBoldStars, R"(<strong>$1</strong>)");
                content = std::regex_replace(content, reBoldUnders, R"(<strong>$1</strong>)");
                content = std::regex_replace(content, reItalStar, R"(<em>$1</em>)");
                content = std::regex_replace(content, reItalUnder, R"(<em>$1</em>)");
                html += "<h" + std::to_string(level) + ">" + content + "</h" + std::to_string(level) + ">";
                continue;
            }

            // Unordered list items: - text or * text
            if (std::regex_match(line, m, reList)) {
                if (!inList) {
                    html += "<ul>";
                    inList = true;
                }
                std::string item = escapeHtml(m[1].str());
                item = allowHtmlBrOnly(item);
                item = std::regex_replace(item, reLink, R"(<a href="$2">$1</a>)");
                item = std::regex_replace(item, reCode, R"(<code>$1</code>)");
                item = std::regex_replace(item, reBoldStars, R"(<strong>$1</strong>)");
                item = std::regex_replace(item, reBoldUnders, R"(<strong>$1</strong>)");
                item = std::regex_replace(item, reItalStar, R"(<em>$1</em>)");
                item = std::regex_replace(item, reItalUnder, R"(<em>$1</em>)");
                html += "<li>" + item + "</li>";
                continue;
            }

            // Paragraph line with inline formatting
            closeListIfOpen();
            std::string para = escapeHtml(line);
            para = allowHtmlBrOnly(para);
            para = std::regex_replace(para, reLink, R"(<a href="$2">$1</a>)");
            para = std::regex_replace(para, reCode, R"(<code>$1</code>)");
            para = std::regex_replace(para, reBoldStars, R"(<strong>$1</strong>)");
            para = std::regex_replace(para, reBoldUnders, R"(<strong>$1</strong>)");
            para = std::regex_replace(para, reItalStar, R"(<em>$1</em>)");
            para = std::regex_replace(para, reItalUnder, R"(<em>$1</em>)");
            html += "<p>" + para + "</p>";
        }

        closeListIfOpen();
        return html;
    } catch (const std::exception&) {
        // Fail-safe: return escaped preformatted block on any error
        return std::string("<pre>") + escapeHtml(markdown) + "</pre>";
    }
}
