//
// Created by Nuno Levezinho Silva on 30/09/2019.
//

#ifndef DYNAMICLIBRARYIMPL_HPP
#define DYNAMICLIBRARYIMPL_HPP

#include <string>

struct DynamicLibraryImpl {

    // Properties

    void* libraryHandle = nullptr;

    // Methods

    static std::string GetDynamicLibraryFileExtension();
};

#endif //DYNAMICLIBRARYIMPL_HPP
