//
// Created by Nuno Levezinho Silva on 18/09/2019.
//

#ifndef HTTPHEADER_HPP
#define HTTPHEADER_HPP

#include <string>
#include <vector>

class HttpHeader {

public:

    explicit HttpHeader(std::string key, std::string value);

    std::string operator [](const std::string& key) const;

    [[nodiscard]] const std::string & GetValue() const;
    [[nodiscard]] const std::string & GetKey() const;

private:
    std::string _key;
    std::string _value;

};

using HttpHeaders = std::vector<HttpHeader>;

#endif //HTTPHEADER_HPP
