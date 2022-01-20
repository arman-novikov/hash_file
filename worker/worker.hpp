#pragma once

#include <queue>
#include <future>
#include <string>
#include <string_view>
#include "../hasher/ihasher.hpp"


class Worker
{
    struct HashTask {
        size_t offset, buff_size;
        std::string* res;
    };
public:
    Worker(IHasher* hasher, std::string_view inp_file);
    void AddTask(size_t offset,
                 size_t buff_size,
                 std::string* res);
    void Launch();
    void Wait();
private:
    void routine_();
    using task_container_t = std::queue<HashTask>;
    IHasher* hasher_;
    std::string inp_file_;
    task_container_t tasks_;
    std::future<void> ftr_;
};
