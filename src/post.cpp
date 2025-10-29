#include <vector>
#include <string>
#include <filesystem>
#include <sqlite3.h>
#include <iostream>

struct BlogPost {
    std::string title;
    std::string content;
    std::string date;
    std::string icon;
    std::string image;
    std::string download;
};

// Seed posts used only if the database is empty
static std::vector<BlogPost> seedPosts = {
    /*{
        "Post Title",
        "post content in markdown...",
        "2025-10-25",
        "icon.png",    // (icon directory)
        "image.png",   // (asset directory, empty string if none)
        "file.bsp"     // (url or file directory, empty string if none)
    },*/
    {
        "it's not about that",
        "dim corridors of the mind, where thoughts maybe are hallucinations, even when the evident truth presented itself in front of our eyes. a thought that refuses to be pinned down always lingers: there is always an _easy way out_ that presents itself in *stressful* situations. the very act of being trapped is less about the walls themselves and more about the meaningless truth that follows upon those walls not being there in the first place. i don't have an answer but i do know it’s not so much that we are stuck in a walled place -what does the concept of being stuck even presuppose?- something that prevents us from reaching a destination or an endpoint as opposed to a state of stillness, a place where the usual direction has been turned inward, and the direction you once relied on has dissolved into a series of vague impressions.<br><br>consider this: what if there are no doors to get out. if there truly were doors, they would be defined by openness and choice. the idea might imply a *closed system*, but paradoxically, the absence of doors is itself a door, an invitation to question whether we even need a door at all...<br><br>perhaps the door is an illusion you create to give yourself a point of departure, in this sense, going back itself becomes the exit; going forward might present itself with *hard truths*. in fact, the insistence on moving forward can become an echo of the very *anxiety* that keeps a person in place. looking back isn’t a mere nostalgic gesture, it’s a deliberate act of revisiting the foundations of your experience, your memories, the sensations that first drew you into this space, and the words you’ve whispered to yourself. echoes of whispers that reminds us we are still part of the conversation. literally, look back at the wall’s texture, the pattern of the light, the way shadows shift with each breath. in those details lies a narrative: the story of how you entered this space, why you stayed, and what it means to be here.<br><br>in the end, the key is not in finding a construct we impose on reality. i began questioning whether we need it at all, shouldn't we free ourselves from the tyranny of conformity instead?",
        "2025-10-28",
        "hl.png",
        "not.about.that.png",
        "not_about_that.bsp"
    },
    {
        "the door was always open",
        "a line drawn in the air. it separates *this* from *that*. that's all it is: separation...<br><br>separation is not disappearance but it (the line) bends back on itself -creates gaps of space- an opening... how does one traverse between openings? through the gaps perhaps? but, no this isn't about getting from one place to another in the way you might think of travel in the classical meaning of that word. imagining instead of thinking the probability. the open door represents an invitation… or perhaps a consequence. it might suggest something wanted this space to exist and in order for that to happen, that something facilitated it. it might not necessarily be welcoming. after all, even a gift can have malevolent intents behind it, and an opening can lead to a trap. the fact that it's already open is the crucial part. you didn't create it. you found it. how does it feel finding something that has already been prepared for you i wonder… that might imply someone else was involved in its creation?<br><br>consider this: if a door were closed, you would have agency. you could choose to knock, to try and enter. but an open door removes that choice, a choice that presents itself as inevitable. it's a path laid out before you even considered walking it. the unsettling part isn't the destination on the other side -we don't know what that is- the __unsettling__ part is the lack of explanation for the opening itself. why now? why this way? who left it like this?! what do they want from your arrival? does that… resonate at all? it's simple to understand that it's a shift in potential, and an alteration to expectation.",
        "2025-10-27",
        "source.png",
        "broken.door.png",
        ""
    }
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
static bool execNonQuery(sqlite3* db, const std::string& sql) {
    char* errMsg = nullptr;
    int rc = sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errMsg);
    if (rc != SQLITE_OK) {
        if (errMsg) {
            std::cerr << "sqlite error: " << errMsg << std::endl;
            sqlite3_free(errMsg);
        }
        return false;
    }
    return true;
}

bool initializeDatabase(const std::string& dbPath) {
    sqlite3* db = nullptr;
    if (sqlite3_open(dbPath.c_str(), &db) != SQLITE_OK) {
        return false;
    }
    const char* createSql =
        "CREATE TABLE IF NOT EXISTS posts ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "title TEXT NOT NULL,"
        "content TEXT NOT NULL,"
        "date TEXT NOT NULL,"
        "icon TEXT NOT NULL,"
        "image TEXT NULL,"
        "download TEXT NULL"
        ");";
    bool ok = execNonQuery(db, createSql);
    sqlite3_close(db);
    return ok;
}

bool seedDatabaseIfEmpty(const std::string& dbPath) {
    sqlite3* db = nullptr;
    if (sqlite3_open(dbPath.c_str(), &db) != SQLITE_OK) {
        return false;
    }

    int count = 0;
    {
        const char* countSql = "SELECT COUNT(1) FROM posts";
        sqlite3_stmt* stmt = nullptr;
        if (sqlite3_prepare_v2(db, countSql, -1, &stmt, nullptr) == SQLITE_OK) {
            if (sqlite3_step(stmt) == SQLITE_ROW) {
                count = sqlite3_column_int(stmt, 0);
            }
        }
        sqlite3_finalize(stmt);
    }

    if (count == 0) {
        const char* insertSql =
            "INSERT INTO posts (title, content, date, icon, image, download)"
            " VALUES (?, ?, ?, ?, ?, ?)";
        sqlite3_stmt* stmt = nullptr;
        if (sqlite3_prepare_v2(db, insertSql, -1, &stmt, nullptr) != SQLITE_OK) {
            sqlite3_close(db);
            return false;
        }
        for (const auto& p : seedPosts) {
            sqlite3_bind_text(stmt, 1, p.title.c_str(), -1, SQLITE_TRANSIENT);
            sqlite3_bind_text(stmt, 2, p.content.c_str(), -1, SQLITE_TRANSIENT);
            sqlite3_bind_text(stmt, 3, p.date.c_str(), -1, SQLITE_TRANSIENT);
            sqlite3_bind_text(stmt, 4, p.icon.c_str(), -1, SQLITE_TRANSIENT);
            sqlite3_bind_text(stmt, 5, p.image.c_str(), -1, SQLITE_TRANSIENT);
            sqlite3_bind_text(stmt, 6, p.download.c_str(), -1, SQLITE_TRANSIENT);
            sqlite3_step(stmt);
            sqlite3_reset(stmt);
            sqlite3_clear_bindings(stmt);
        }
        sqlite3_finalize(stmt);
    }

    sqlite3_close(db);
    return true;
}

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
