#include <vector>
#include <string>
#include <filesystem>
#include "html_shared.hpp"

// Function to scan asset directory for images
std::vector<std::string> getAssetImages() {
    std::vector<std::string> images;
    std::string assetPath = "asset/";

    try {
        for (const auto& entry : std::filesystem::directory_iterator(assetPath)) {
            if (entry.is_regular_file()) {
                std::string filename = entry.path().filename().string();
                std::string extension = entry.path().extension().string();

                // Check for common image extensions
                if (extension == ".png" || extension == ".jpg" || extension == ".jpeg" ||
                    extension == ".gif" || extension == ".bmp" || extension == ".webp") {
                    images.push_back(filename);
                }
            }
        }
    } catch (const std::filesystem::filesystem_error& e) {
        // If asset directory doesn't exist or can't be read, return empty vector
    }

    return images;
}

// Function to generate gallery HTML
std::string generateGalleryHTML() {
    std::string content = R"(
        <main class="gallery-container">
            <div class="gallery-grid">)";

    // Dynamically scan asset directory for images
    std::vector<std::string> assetImages = getAssetImages();
    for (const auto& image : assetImages) {
        content += R"(<div class="gallery-item">
                    <img src="asset/)" + image + R"(" alt=")" + image + R"(" />
                </div>)";
    }

    content += R"(
            </div>
        </main>)";

    return generatePageWrapper("iliya's world - gallery", "gallery", content);
}
