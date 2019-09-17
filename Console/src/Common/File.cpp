//
// Created by Nuno Levezinho Silva on 17/09/2019.
//

#include <Console/Common/File.hpp>
#include <fstream>

std::vector<char> File::ReadAllBytes(const char *filename) {

    std::ifstream ifs(filename, std::ios::binary|std::ios::ate);

    ifs.seekg(0, std::ios::end);

    std::ifstream::pos_type position = ifs.tellg();

    std::vector<char> result(position);

    ifs.seekg(0, std::ios::beg);

    ifs.read(result.data(), position);

    ifs.close();

    return result;
}

