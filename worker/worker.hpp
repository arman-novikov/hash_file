#pragma once

#include <queue>
#include <future>
#include "../hasher/ihasher.hpp"


class Worker
{
    struct HashTask {
        const void* buff;
        size_t buff_size;
        std::string* res;
    };
public:
    Worker(IHasher* hasher): hasher_{hasher}{}
    void AddTask(const void* buff, size_t buff_size, std::string* res);
    void Launch();
    void Wait();
private:
    void routine_();
    using task_container_t = std::queue<HashTask>;
    IHasher* hasher_;
    task_container_t tasks_;
    std::future<void> ftr_;
};
