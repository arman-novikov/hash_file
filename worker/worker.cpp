#include "worker.hpp"
#include <fstream>
#include <vector>


Worker::Worker(IHasher* hasher, std::string_view inp_file):
    hasher_{hasher}, inp_file_{inp_file.data()}{}

void Worker::AddTask(size_t offset,
                     size_t buff_size,
                     std::string* res)
{
    tasks_.push({offset, buff_size, res});
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
    std::ifstream inp(inp_file_, std::ios::binary | std::ios::in);
    std::vector<char> buff;
    if (!inp) {
        std::string err = "failed to open ";
        err += inp_file_;
        throw std::runtime_error(err);
    }
    while (!tasks_.empty()) {
        HashTask t = tasks_.front();
        buff.reserve(t.buff_size);
        inp.seekg(t.offset);
        inp.read(&buff[0], t.buff_size);
        *(t.res) = hasher_->hash(&buff[0], t.buff_size);
        tasks_.pop();
    }
}
