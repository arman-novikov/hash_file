#pragma once

#include <queue>
#include <future>
#include <mutex>
#include <condition_variable>
#include "../hasher/ihasher.hpp"


class Worker
{
    struct HashTask {
        const void* buff;
        size_t buff_size;
        std::string* res;
    };
public:
    Worker(IHasher* hasher): hasher_{hasher}, finish_{true} {}
    Worker(Worker&& w):
        hasher_{w.hasher_},
        finish_{true}, done_{true},
        mtx_{},
        done_mtx_{},
        cv_{},
        done_cv_{},
        tasks_{std::move(w.tasks_)},
        ftr_{}
    {}
    void AddTask(const void* buff, size_t buff_size, std::string* res);
    void Launch();
    void Wait();
private:
    void routine_();
    using task_container_t = std::queue<HashTask>;
    IHasher* hasher_;
    bool finish_, done_;
    std::mutex mtx_, done_mtx_;
    std::condition_variable cv_, done_cv_;
    task_container_t tasks_;
    std::future<void> ftr_;
};
