//
// Created by Nuno Levezinho Silva on 30/09/2019.
//

#include <Abstractions/File.hpp>
#include <fstream>

std::vector<U8> File::ReadAllBytes(const std::string& filename) {

    std::ifstream ifs(filename, std::ios::binary|std::ios::ate);

    ifs.seekg(0, std::ios::end);

    std::ifstream::pos_type position = ifs.tellg();

    std::vector<U8> result(position);

    ifs.seekg(0, std::ios::beg);

    ifs.read(reinterpret_cast<char *>(result.data()), position);

    ifs.close();

    return result;
}

bool File::Exists(const std::string& filename) {

    std::ifstream infile(filename);

    return infile.good();
}
