#include "FileSystem.hpp"

namespace filesystem{
    std::filesystem::path GetSourceFolder(){
        std::filesystem::path currentPath   = std::filesystem::current_path();
        std::string stemString = currentPath.stem().string();

        while (stemString != "PuyoPuyo"){
            currentPath = currentPath.parent_path();
            stemString = currentPath.stem().string();
        }

        if (std::filesystem::exists(currentPath)) {
            return currentPath;
        }
        return std::filesystem::path("NOT_FOUND");
    }
}