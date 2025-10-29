#pragma once
#include <vector>
#include <string>

struct BlogPost {
    std::string title;
    std::string content;
    std::string date;
    std::string icon;
    std::string image;
    std::string download;
};

// Storage and post APIs
bool initializeDatabase(const std::string& dbPath);
bool seedDatabaseIfEmpty(const std::string& dbPath);
std::vector<std::string> getAvailableIcons();
std::vector<BlogPost> getPostsForPage(const std::string& dbPath, int page, int postsPerPage);
int getTotalPages(const std::string& dbPath, int postsPerPage);
std::vector<BlogPost> getAllPosts(const std::string& dbPath);
