#include <fstream>
#include <vector>
#include <tuple>
#include <string>
#include <string_view>
#include <memory>
#include <boost/iostreams/device/mapped_file.hpp>
#include "hasher/md5hasher.hpp"
#include "worker/worker.hpp"

using args_t = std::tuple<std::string, std::string, size_t>;
static std::string hash_buffer(const void* buff,
                               size_t buff_size,
                               size_t block_size);
static void save_res(std::string_view dest, std::string_view data);
static args_t parse_arg(int argc, char *argv[]);

int main(int argc, char *argv[]) {
    const auto [inp, dest, block_size] = parse_arg(argc, argv);
    auto src = std::make_unique<boost::iostreams::mapped_file_source>(inp);
    auto res = hash_buffer(src->data(), src->size(), block_size);
    save_res(dest, res);
    return 0;
}

std::string hash_buffer(const void* buff, size_t buff_size, size_t block_size)
{
    Md5Hasher hasher{};
    const size_t num_cores = std::max(std::thread::hardware_concurrency(), 1U);
    size_t num_blocks = buff_size / block_size;
    if (buff_size % block_size) {
        num_blocks += 1; // remainings if there are any
    }
    std::vector<std::string> results(num_blocks);
    std::vector<Worker> workers;
    workers.reserve(num_cores);
    for (size_t i = 0; i < num_cores; ++i) {
        workers.emplace_back(Worker{&hasher});
    }    
    const size_t num_blocks_for_each = num_blocks / workers.size();
    for (size_t i = 0; i < workers.size(); ++i) {
        size_t end = num_blocks_for_each;
        if (i == workers.size() - 1) {
            end = num_blocks - i * num_blocks_for_each;
        }

        for (size_t j = 0; j < end; ++j) {
            const size_t offset = i * num_blocks_for_each + j;
            const auto start = reinterpret_cast<const uint8_t*>(buff) + offset * block_size;
            const auto segm_size = offset != num_blocks - 1 ? block_size:
                buff_size - offset * block_size; // the last one takes remainings
            workers[i].AddTask(
                reinterpret_cast<const void*>(start),
                segm_size,
                &results[offset]
            );
        }
        workers[i].Launch();
    }
    for (auto& i: workers) {
        i.Wait();
    }

    std::string res{};
    for (const auto &i : results) { // can't use std::accumulate in c++17
        res += i;
    }
    return res;
}

void save_res(std::string_view dest, std::string_view data)
{
    std::ofstream out(dest.data());
    out << data.data();
}

args_t parse_arg(int argc, char *argv[]) {
    size_t block_size = 1'048'576; // 1Mb
    if(argc < 3) {
        throw std::runtime_error("not enough args");
    }

    if (argc == 4) {
        int customer_block_size = std::stoi(argv[3]);
        if (customer_block_size <= 0 ) { // can't use fmt
            std::string err("failed to parse ");
            err += argv[3];
            err += " as a block size";
            throw std::runtime_error(err);
        }
        block_size = static_cast<size_t>(customer_block_size);
    }

    return std::tuple{std::string{argv[1]}, std::string{argv[2]}, block_size};
}
