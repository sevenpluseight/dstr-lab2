#include <filesystem>

#include "path_utils.hpp"
#include "message_handler.hpp"

namespace fs = std::filesystem;

std::string getDataFilePath(const std::string& filename) {
    fs::path currentPath = fs::current_path();
    fs::path projectRoot;

    // Traverse up the directory tree to find the project root (where CMakeLists.txt is located)
    fs::path tempPath = currentPath;
    while (tempPath.has_parent_path()) {
        if (fs::exists(tempPath / "CMakeLists.txt")) {
            projectRoot = tempPath;
            break;
        }
        tempPath = tempPath.parent_path();
    }

    if (projectRoot.empty()) {
        MessageHandler::error("Could not find project root (CMakeLists.txt not found in parent directories).");
        return "";
    }

    fs::path dataDir = projectRoot / "data";

    if (!fs::exists(dataDir)) {
        if (!fs::create_directories(dataDir)) {
            MessageHandler::error("Failed to create data directory: " + dataDir.string());
            return "";
        }
    } else if (!fs::is_directory(dataDir)) {
        MessageHandler::error("Path exists but is not a directory: " + dataDir.string());
        return "";
    }

    fs::path filePath = dataDir / filename;
    return filePath.string();
}
