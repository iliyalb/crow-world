#include <vector>
#include <string>
#include <filesystem>

struct BlogPost {
    std::string title;
    std::string content;
    std::string date;
    std::string icon;
    std::string image;
    std::string download;
};

std::vector<BlogPost> blogPosts = {
    /*{
        "Post Title",
        "post content with <br> for line breaks...",
        "2025-10-25",
        "icon.png",    // (icon directory)
        "image.png",   // (asset directory, empty string if none)
        "file.bsp"     // (file directory, empty string if none)
    },*/
    {
        "the door was always open",
        "a line drawn in the air. it separates <i>this</i> from <i>that</i>. that's all it is: separation...<br><br>separation is not disappearance but it (the line) bends back on itself -creates gaps of space- an opening... how does one traverse between openings? through the gaps perhaps? but, no this isn't about getting from one place to another in the way you might think of travel in the classical meaning of that word. imagining instead of thinking the probability. the open door represents an invitation… or perhaps a consequence. it might suggest something _wanted_ this space to exist and in order for that to happen, that something facilitated it. it might not necessarily be welcoming. after all, even a gift can have malevolent intents behind it, and an opening can lead to a trap. the fact that it's _already_ open is the crucial part. you didn't create it. you found it. how does it feel finding something that has already been prepared for you i wonder… that might imply someone else was involved in its creation?<br><br>consider this: if a door were closed, you would have agency. you could choose to knock, to try and enter. but an open door removes that choice, a choice that presents itself as inevitable. it's a path laid out _before_ you even considered walking it. the unsettling part isn't the destination on the other side -we don't know what that is- the unsettling part is the lack of explanation for the opening itself. why now? why this way? who left it like this?! what do they want from your arrival? does that… resonate at all? it's simple to understand that it's a shift in potential, and an alteration to expectation.",
        "2025-10-27",
        "gldsrc.png",
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

// Pagination functions
std::vector<BlogPost> getPostsForPage(int page, int postsPerPage) {
    std::vector<BlogPost> pagePosts;
    int startIndex = (page - 1) * postsPerPage;
    int endIndex = std::min(startIndex + postsPerPage, (int)blogPosts.size());

    for (int i = startIndex; i < endIndex; i++) {
        pagePosts.push_back(blogPosts[i]);
    }

    return pagePosts;
}

int getTotalPages(int postsPerPage) {
    return (blogPosts.size() + postsPerPage - 1) / postsPerPage;
}
