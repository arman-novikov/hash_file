#include "worker.hpp"

void Worker::AddTask(const void* buff, size_t buff_size, std::string* res)
{
    tasks_.push({buff, buff_size, res});
}

void Worker::Launch()
{
    if (!tasks_.empty()) {
        ftr_ = std::async(std::launch::async, &Worker::routine_, this);
    }
}

void Worker::Wait() {
    if (ftr_.valid()){
        ftr_.get();
    }
}

void Worker::routine_() {
    while (!tasks_.empty()) {
        HashTask t = tasks_.front();
        *(t.res) = hasher_->hash(t.buff, t.buff_size);
        tasks_.pop();
    }
}
