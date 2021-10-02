//
// Created by Nuno on 3/25/2021.
//

#include <Abstractions/Directory.hpp>

#include <filesystem>

std::string Directory::CurrentWorkingDirectory() {
    return std::filesystem::current_path().string();
}
