//
// Created by Nuno Levezinho Silva on 30/09/2019.
//

#ifndef DYNAMICLIBRARY_HPP
#define DYNAMICLIBRARY_HPP

#include <string>
#include <memory>

class DynamicLibrary {

public:

    DynamicLibrary();
    ~DynamicLibrary();
    DynamicLibrary(const DynamicLibrary&) = delete;
    DynamicLibrary(DynamicLibrary&& other) noexcept;
    DynamicLibrary& operator=(const DynamicLibrary& other) = delete;
    DynamicLibrary& operator=(DynamicLibrary&& other) noexcept;


    bool Load(const std::string& path, const std::string& name);
    void Unload();

    void* GetProcedure(const std::string& name);
    static std::string GetLastError();

private:
    std::unique_ptr< struct DynamicLibraryImpl > _impl;
};

#endif //DYNAMICLIBRARY_HPP
