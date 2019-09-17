//
// Created by Nuno Levezinho Silva on 17/09/2019.
//

#ifndef FILE_HPP
#define FILE_HPP

#include <vector>

class File {

public:
    static std::vector<char> ReadAllBytes(const char* filename);

};

#endif //FILE_HPP
