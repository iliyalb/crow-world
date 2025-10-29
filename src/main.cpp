#include "crow.h"
#include "post.hpp"
#include "gallery.hpp"
#include "html_shared.hpp"
#include <regex>
#include <fstream>
#include <sstream>
#include <vector>
#include <ctime>

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
    std::string content = R"(
        <main class="blog-container">
            <div class="blog-posts">
)";

    // Get posts for current page
    const std::string dbPath = "storage.db";
    std::vector<BlogPost> pagePosts = getPostsForPage(dbPath, page, postsPerPage);
    for (const auto& post : pagePosts) {
        content += R"(<article class="blog-post">
                    <div class="post-meta">
                        <span class="post-date">)" + post.date + R"(</span>
                        <div class="post-actions">)";

        // Add download button if file exists
        if (!post.download.empty()) {
            std::string downloadUrl;
            if (isUrl(post.download)) {
                // It's a URL, use it directly
                downloadUrl = post.download;
            } else {
                // It's a file, prepend file/ path
                downloadUrl = "file/" + post.download;
            }
            content += R"(<a href=")" + downloadUrl + R"(" class="download-btn" title="Download )" + post.download + R"(">DOWNLOAD</a>)";
        }

        content += R"(<span class="post-icon"><img src="icon/)" + post.icon + R"(" alt="icon" width="16" height="16" /></span>
                        </div>
                    </div>
                    <h2 class="post-title">)" + post.title + R"(</h2>
                    <div class="post-content">)" + renderMarkdown(post.content) + R"(</div>)";

        // Add optional image if it exists
        if (!post.image.empty()) {
            content += R"(<div class="post-image"><img src="asset/)" + post.image + R"(" alt="post image" /></div>)";
        }

        content += R"(</article>)";
    }

    // Add pagination controls
    int totalPages = getTotalPages(dbPath, postsPerPage);
    content += R"(
            </div>
            <div class="pagination-controls">
                <div class="pagination-left">)";

    // Page numbers
    for (int i = 1; i <= totalPages; i++) {
        if (i == page) {
            content += R"(<span class="page-number current">)" + std::to_string(i) + R"(</span>)";
        } else {
            content += R"(<a href="/?page=)" + std::to_string(i) + R"(&per_page=)" + std::to_string(postsPerPage) + R"(" class="page-number">)" + std::to_string(i) + R"(</a>)";
        }
    }

    content += "</div>\n";
    content += "                <div class=\"pagination-right\">\n";
    content += "                    <label for=\"posts-per-page\">posts:</label>\n";
    content += "                    <select id=\"posts-per-page\" onchange=\"changePostsPerPage(this.value)\">\n";

    // Add options with proper selected state
    if (postsPerPage == 5) {
        content += "                        <option value=\"5\" selected>5</option>\n";
    } else {
        content += "                        <option value=\"5\">5</option>\n";
    }

    if (postsPerPage == 10) {
        content += "                        <option value=\"10\" selected>10</option>\n";
    } else {
        content += "                        <option value=\"10\">10</option>\n";
    }

    if (postsPerPage == 25) {
        content += "                        <option value=\"25\" selected>25</option>\n";
    } else {
        content += "                        <option value=\"25\">25</option>\n";
    }

    content += "                    </select>\n";
    content += "                </div>\n";
    content += "            </div>\n";
    content += "        </main>\n";
    content += "        <script>\n";
    content += "            function changePostsPerPage(value) {\n";
    content += "                window.location.href = '/?page=1&per_page=' + value;\n";
    content += "            }\n";
    content += "        </script>\n";

    return generatePageWrapper("iliya's world", "home", content);
}

int main() {
    crow::SimpleApp app;

    const std::string dbPath = "storage.db";
    initializeDatabase(dbPath);
    seedDatabaseIfEmpty(dbPath);

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
        const std::string dbPath = "storage.db";
        auto posts = getAllPosts(dbPath);
        for (size_t i = 0; i < posts.size(); ++i) {
            json_posts[i]["title"] = posts[i].title;
            json_posts[i]["content"] = posts[i].content;
            json_posts[i]["date"] = posts[i].date;
            json_posts[i]["icon"] = posts[i].icon;
            json_posts[i]["image"] = posts[i].image;
            json_posts[i]["download"] = posts[i].download;
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
