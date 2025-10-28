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

extern std::vector<BlogPost> blogPosts;

// Function declarations
std::vector<std::string> getAvailableIcons();
std::vector<BlogPost> getPostsForPage(int page, int postsPerPage);
int getTotalPages(int postsPerPage);
