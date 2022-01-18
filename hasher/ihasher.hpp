#pragma once
#include <string>

class IHasher {
public:
    virtual std::string hash(const void* buff, size_t buf_size) = 0;
    virtual ~IHasher(){}
};
