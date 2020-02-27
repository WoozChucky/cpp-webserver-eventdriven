//
// Created by Nuno Levezinho Silva on 30/09/2019.
//

#ifndef FILE_HPP
#define FILE_HPP

#include <vector>
#include <string>

class File {

public:
    static std::vector<char> ReadAllBytes(const std::string& filename);
    static bool Exists(const std::string& filename);

};

#endif //FILE_HPP
