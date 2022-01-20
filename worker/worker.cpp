#include "worker.hpp"

void Worker::AddTask(const void* buff, size_t buff_size, std::string* res)
{
    std::unique_lock lock{mtx_};
    tasks_.push({buff, buff_size, res});
    lock.unlock();
    cv_.notify_one();
}

void Worker::Launch()
{
    finish_ = false;
    done_ = false;
    ftr_ = std::async(std::launch::async, &Worker::routine_, this);
}

void Worker::Wait() {
    {
        std::unique_lock lock{done_mtx_};
        done_cv_.wait(lock, [this](){return done_;});
    }
    {
        std::unique_lock lock{mtx_};
        finish_ = true;
        lock.unlock();
        cv_.notify_one();
    }
    if (ftr_.valid()){
        ftr_.get();
    }
}

void Worker::routine_() {
    while (true) {
        {
            std::unique_lock<std::mutex> lock(mtx_);
            cv_.wait(lock, [this]{return finish_ || !tasks_.empty();});
            if (finish_) {
                std::unique_lock done_lock{done_mtx_};
                done_ = true;
                done_lock.unlock();
                done_cv_.notify_one();
                return;
            }
        }
        std::unique_lock lock{mtx_};
        HashTask t = tasks_.front();
        tasks_.pop();
        lock.unlock();
        *(t.res) = hasher_->hash(t.buff, t.buff_size);
        lock.lock();
        if (tasks_.empty()) {
            std::unique_lock done_lock{done_mtx_};
            done_ = true;
            done_lock.unlock();
            done_cv_.notify_one();
        }
    }
}
