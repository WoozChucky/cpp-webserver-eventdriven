//
// Created by Nuno Levezinho Silva on 30/09/2019.
//

#include <Abstractions/DynamicLibrary.hpp>

#include <dlfcn.h>
#include <unistd.h>
#include <sys/param.h>
#include <vector>
#include <cstdarg>
#include <cassert>

#include "DynamicLibraryImpl.hpp"

std::string vsprintf(const char* format, va_list args) {
    va_list argsCopy;
    va_copy(argsCopy, args);
    const int required = vsnprintf(nullptr, 0, format, args);
    va_end(args);
    if (required < 0) {
        va_end(argsCopy);
        return "";
    }
    std::vector< char > buffer(required + 1);
    const int result = vsnprintf(&buffer[0], required + 1, format, argsCopy);
    va_end(argsCopy);
    if (result < 0) {
        return "";
    }
    return std::string(buffer.begin(), buffer.begin() + required);
}

std::string internal_sprintf(const char* format, ...) {
    va_list args;
    va_start(args, format);
    return vsprintf(format, args);
}


std::string DynamicLibraryImpl::GetDynamicLibraryFileExtension() {
    return "dylib";
}

DynamicLibrary::DynamicLibrary()
        : _impl(new DynamicLibraryImpl()){

}

DynamicLibrary::DynamicLibrary(DynamicLibrary &&other) noexcept
        : _impl(std::move(other._impl)) {

}

DynamicLibrary::~DynamicLibrary() {
    if (_impl == nullptr) {
        return;
    }
    Unload();
}

DynamicLibrary &DynamicLibrary::operator=(DynamicLibrary &&other) noexcept {
    assert(this != &other);
    _impl = std::move(other._impl);
    return *this;
}

bool DynamicLibrary::Load(const std::string &path, const std::string &name) {
    Unload();
    std::vector<char> originalPath(MAXPATHLEN);
    (void)getcwd(&originalPath[0], originalPath.size());
    (void)chdir(path.c_str());
    const auto library = internal_sprintf(
            "%s/lib%s.%s",
            path.c_str(),
            name.c_str(),
            DynamicLibraryImpl::GetDynamicLibraryFileExtension().c_str()
    );
    _impl->libraryHandle = dlopen(library.c_str(), RTLD_NOW);
    (void)chdir(&originalPath[0]);
    return (_impl->libraryHandle != nullptr);
}

void DynamicLibrary::Unload() {
    if (_impl->libraryHandle != nullptr) {
        (void)dlclose(_impl->libraryHandle);
        _impl->libraryHandle = nullptr;
    }
}

void *DynamicLibrary::GetProcedure(const std::string &name) {
    return dlsym(_impl->libraryHandle, name.c_str());
}

std::string DynamicLibrary::GetLastError() {
    return dlerror();
}

