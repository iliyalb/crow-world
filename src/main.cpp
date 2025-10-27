#include "crow.h"
#include "post.h"
#include "gallery.h"
#include <fstream>
#include <sstream>
#include <vector>
#include <filesystem>
#include <ctime>
#include <algorithm>

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

std::string getCurrentTime() {
    time_t now = time(0);
    char buffer[100];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M", localtime(&now));
    return std::string(buffer);
}

std::string generateBlogHTML(int page = 1, int postsPerPage = 5) {
    std::string html = R"(
<!doctype html>
<html lang="en">
    <head>
        <meta charset="UTF-8" />
        <meta name="viewport" content="width=device-width, initial-scale=1.0" />
        <meta http-equiv="X-UA-Compatible" content="ie=edge" />
        <title>iliya's world</title>
        <link rel="stylesheet" href="./style.css" />
        <link rel="icon" href="./favicon.ico" type="image/x-icon" />
    </head>
    <body>
        <header>
            <a href="/"><img src="lambda.png" alt="λ" width="14" /></a>
        </header>
        <hr />
        <nav class="main-nav">
            <a href="/" class="nav-link active">Home</a>
            <a href="/gallery" class="nav-link">Gallery</a>
        </nav>
        <main class="blog-container">
            <div class="blog-posts">
)";

    // Get posts for current page
    std::vector<BlogPost> pagePosts = getPostsForPage(page, postsPerPage);
    for (const auto& post : pagePosts) {
        html += R"(<article class="blog-post">
                    <div class="post-meta">
                        <span class="post-date">)" + post.date + R"(</span>
                        <div class="post-actions">)";

        // Add download button if file exists
        if (!post.download.empty()) {
            html += R"(<a href="file/)" + post.download + R"(" class="download-btn" title="Download )" + post.download + R"(">DOWNLOAD</a>)";
        }

        html += R"(<span class="post-icon"><img src="icon/)" + post.icon + R"(" alt="icon" width="16" height="16" /></span>
                        </div>
                    </div>
                    <h2 class="post-title">)" + post.title + R"(</h2>
                    <div class="post-content">)" + post.content + R"(</div>)";

        // Add optional image if it exists
        if (!post.image.empty()) {
            html += R"(<div class="post-image"><img src="asset/)" + post.image + R"(" alt="post image" /></div>)";
        }

        html += R"(</article>)";
    }

    // Add pagination controls
    int totalPages = getTotalPages(postsPerPage);
    html += R"(
            </div>
            <div class="pagination-controls">
                <div class="pagination-left">)";

    // Page numbers
    for (int i = 1; i <= totalPages; i++) {
        if (i == page) {
            html += R"(<span class="page-number current">)" + std::to_string(i) + R"(</span>)";
        } else {
            html += R"(<a href="/?page=)" + std::to_string(i) + R"(&per_page=)" + std::to_string(postsPerPage) + R"(" class="page-number">)" + std::to_string(i) + R"(</a>)";
        }
    }

    html += "</div>\n";
    html += "                <div class=\"pagination-right\">\n";
    html += "                    <label for=\"posts-per-page\">Posts per page:</label>\n";
    html += "                    <select id=\"posts-per-page\" onchange=\"changePostsPerPage(this.value)\">\n";

    // Add options with proper selected state
    if (postsPerPage == 5) {
        html += "                        <option value=\"5\" selected>5</option>\n";
    } else {
        html += "                        <option value=\"5\">5</option>\n";
    }

    if (postsPerPage == 10) {
        html += "                        <option value=\"10\" selected>10</option>\n";
    } else {
        html += "                        <option value=\"10\">10</option>\n";
    }

    if (postsPerPage == 25) {
        html += "                        <option value=\"25\" selected>25</option>\n";
    } else {
        html += "                        <option value=\"25\">25</option>\n";
    }

    html += "                    </select>\n";
    html += "                </div>\n";
    html += "            </div>\n";
    html += "        </main>\n";
    html += "        <footer>\n";
    html += "            <p>copyleft 🄯 iliya lesani</p>\n";
    html += "        </footer>\n";
    html += "        <script>\n";
    html += "            function changePostsPerPage(value) {\n";
    html += "                window.location.href = '/?page=1&per_page=' + value;\n";
    html += "            }\n";
    html += "        </script>\n";
    html += "        <script src=\"index.js\"></script>\n";
    html += "    </body>\n";
    html += "</html>\n";
    return html;
}

int main() {
    crow::SimpleApp app;

    CROW_ROUTE(app, "/")([](const crow::request& req){
        int page = 1;
        int postsPerPage = 5;

        // Parse query parameters
        auto pageParam = req.url_params.get("page");
        auto perPageParam = req.url_params.get("per_page");

        if (pageParam) {
            try {
                page = std::stoi(pageParam);
                if (page < 1) page = 1;
            } catch (...) {
                page = 1;
            }
        }

        if (perPageParam) {
            try {
                postsPerPage = std::stoi(perPageParam);
                if (postsPerPage != 5 && postsPerPage != 10 && postsPerPage != 25) {
                    postsPerPage = 5;
                }
            } catch (...) {
                postsPerPage = 5;
            }
        }

        return generateBlogHTML(page, postsPerPage);
    });

    CROW_ROUTE(app, "/gallery")([]{
        return generateGalleryHTML();
    });

    CROW_ROUTE(app, "/api/posts")([]{
        crow::json::wvalue json_posts;
        for (size_t i = 0; i < blogPosts.size(); ++i) {
            json_posts[i]["title"] = blogPosts[i].title;
            json_posts[i]["content"] = blogPosts[i].content;
            json_posts[i]["date"] = blogPosts[i].date;
            json_posts[i]["icon"] = blogPosts[i].icon;
            json_posts[i]["image"] = blogPosts[i].image;
            json_posts[i]["download"] = blogPosts[i].download;
        }
        return json_posts;
    });

    CROW_ROUTE(app, "/<path>")([](const std::string& path) {
        std::string fullPath;

        if (startsWith(path, "asset/")) {
            fullPath = path;
        } else if (startsWith(path, "file/")) {
            fullPath = path;
        } else if (startsWith(path, "icon/")) {
            fullPath = path;
        } else {
            fullPath = path;  // Serve from root
        }

        return readFile(fullPath);
    });

    app.port(18080).multithreaded().run();
}
