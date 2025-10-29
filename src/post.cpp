#include <vector>
#include <string>
#include <filesystem>
#include <sqlite3.h>

struct BlogPost {
    std::string title;
    std::string content;
    std::string date;
    std::string icon;     // (icon directory)
    std::string image;    // (asset directory)
    std::string download; // (url or file directory)
};

// Function to scan icon directory for available icons
std::vector<std::string> getAvailableIcons() {
    std::vector<std::string> icons;
    std::string iconPath = "icon/";

    try {
        for (const auto& entry : std::filesystem::directory_iterator(iconPath)) {
            if (entry.is_regular_file()) {
                std::string filename = entry.path().filename().string();
                std::string extension = entry.path().extension().string();

                // Check for common image extensions
                if (extension == ".png" || extension == ".jpg" || extension == ".jpeg" ||
                    extension == ".gif" || extension == ".bmp" || extension == ".webp") {
                    icons.push_back(filename);
                }
            }
        }
    } catch (const std::filesystem::filesystem_error& e) {
        // If icon directory doesn't exist or can't be read, return empty vector
    }

    return icons;
}

// SQLite helpers and APIs
std::vector<BlogPost> getPostsForPage(const std::string& dbPath, int page, int postsPerPage) {
    std::vector<BlogPost> posts;
    sqlite3* db = nullptr;
    if (sqlite3_open(dbPath.c_str(), &db) != SQLITE_OK) {
        return posts;
    }

    int offset = (page - 1) * postsPerPage;
    const char* selectSql =
        "SELECT title, content, date, icon, image, download"
        " FROM posts"
        " ORDER BY date DESC, id DESC"
        " LIMIT ? OFFSET ?";
    sqlite3_stmt* stmt = nullptr;
    if (sqlite3_prepare_v2(db, selectSql, -1, &stmt, nullptr) == SQLITE_OK) {
        sqlite3_bind_int(stmt, 1, postsPerPage);
        sqlite3_bind_int(stmt, 2, offset);
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            BlogPost p;
            p.title = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
            p.content = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
            p.date = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
            p.icon = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
            p.image = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));
            p.download = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5));
            posts.push_back(std::move(p));
        }
    }
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return posts;
}

int getTotalPages(const std::string& dbPath, int postsPerPage) {
    if (postsPerPage <= 0) return 0;
    int total = 0;
    sqlite3* db = nullptr;
    if (sqlite3_open(dbPath.c_str(), &db) != SQLITE_OK) {
        return 0;
    }
    const char* countSql = "SELECT COUNT(1) FROM posts";
    sqlite3_stmt* stmt = nullptr;
    if (sqlite3_prepare_v2(db, countSql, -1, &stmt, nullptr) == SQLITE_OK) {
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            total = sqlite3_column_int(stmt, 0);
        }
    }
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return (total + postsPerPage - 1) / postsPerPage;
}

std::vector<BlogPost> getAllPosts(const std::string& dbPath) {
    std::vector<BlogPost> posts;
    sqlite3* db = nullptr;
    if (sqlite3_open(dbPath.c_str(), &db) != SQLITE_OK) {
        return posts;
    }
    const char* selectSql =
        "SELECT title, content, date, icon, image, download"
        " FROM posts"
        " ORDER BY date DESC, id DESC";
    sqlite3_stmt* stmt = nullptr;
    if (sqlite3_prepare_v2(db, selectSql, -1, &stmt, nullptr) == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            BlogPost p;
            p.title = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
            p.content = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
            p.date = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
            p.icon = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
            p.image = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));
            p.download = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5));
            posts.push_back(std::move(p));
        }
    }
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return posts;
}
