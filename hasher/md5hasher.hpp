#pragma once
#include "ihasher.hpp"

class Md5Hasher: public IHasher {
public:
    std::string hash(const void *buff, size_t buf_size) override;
    ~Md5Hasher() override = default;
};
