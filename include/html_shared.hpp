#pragma once
#include <string>

// Shared HTML generation functions
std::string generateHeader(const std::string& title, const std::string& activeNav = "home");
std::string generateNavigation(const std::string& activeNav = "home");
std::string generateFooter();
std::string generatePageWrapper(const std::string& title, const std::string& activeNav, const std::string& content);

// Utility function to check if a string is a URL
bool isUrl(const std::string& str);

// Markdown rendering
std::string renderMarkdown(const std::string& markdown);
