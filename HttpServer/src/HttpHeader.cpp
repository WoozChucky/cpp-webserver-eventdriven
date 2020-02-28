//
// Created by Nuno Levezinho Silva on 30/09/2019.
//

#include <Http/HttpHeader.hpp>
#include <utility>

HttpHeader::HttpHeader(std::string key, std::string value) {
    this->_key = std::move(key);
    this->_value = std::move(value);
}

std::string HttpHeader::operator[](const std::string &key) const
{
    if (this->_key == key)
        return _value;
    return "";
}

const std::string &HttpHeader::GetValue() const {
    return _value;
}

const std::string &HttpHeader::GetKey() const {
    return _key;
}