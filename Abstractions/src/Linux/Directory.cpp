//
// Created by Nuno on 3/25/2021.
//

#include <Abstractions/Directory.hpp>

#include <unistd.h>

std::string Directory::CurrentWorkingDirectory() {
    return std::string(get_current_dir_name());
}
