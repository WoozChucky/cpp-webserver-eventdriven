#include <Abstractions/Directory.hpp>

#include <filesystem>

std::string Directory::CurrentWorkingDirectory() {
	return {std::filesystem::current_path()};
}
