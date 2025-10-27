#include <vector>
#include <string>
#include <filesystem>

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
    std::string html = R"(
<!doctype html>
<html lang="en">
    <head>
        <meta charset="UTF-8" />
        <meta name="viewport" content="width=device-width, initial-scale=1.0" />
        <meta http-equiv="X-UA-Compatible" content="ie=edge" />
        <title>iliya's world - Gallery</title>
        <link rel="stylesheet" href="./style.css" />
        <link rel="icon" href="./favicon.ico" type="image/x-icon" />
    </head>
    <body>
        <header>
            <a href="/"><img src="lambda.png" alt="λ" width="14" /></a>
        </header>
        <hr />
        <nav class="main-nav">
            <a href="/" class="nav-link">Home</a>
            <a href="/gallery" class="nav-link active">Gallery</a>
        </nav>
        <main class="gallery-container">
            <div class="gallery-grid">)";

    // Dynamically scan asset directory for images
    std::vector<std::string> assetImages = getAssetImages();
    for (const auto& image : assetImages) {
        html += R"(<div class="gallery-item">
                    <img src="asset/)" + image + R"(" alt=")" + image + R"(" />
                </div>)";
    }

    html += R"(
            </div>
        </main>
        <footer>
            <p>copyleft 🄯 iliya lesani</p>
        </footer>
        <script src="index.js"></script>
    </body>
</html>
)";
    return html;
}
