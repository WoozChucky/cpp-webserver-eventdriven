//
// Created by Nuno Levezinho Silva on 18/09/2019.
//

#ifndef HTTPHEADER_HPP
#define HTTPHEADER_HPP

#include <string>

class HttpHeader {

public:

    explicit HttpHeader(std::string key, std::string value);

    const std::string& operator [](const std::string& key);

    const std::string & GetValue() const;
    const std::string & GetKey() const;

private:
    std::string _key;
    std::string _value;

};

typedef std::vector<HttpHeader> HttpHeaders;

#endif //HTTPHEADER_HPP
