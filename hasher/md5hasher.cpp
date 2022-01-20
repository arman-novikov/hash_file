#include "md5hasher.hpp"

#include <boost/uuid/detail/md5.hpp>
#include <boost/algorithm/hex.hpp>

std::string Md5Hasher::hash(const void* buff, size_t buf_size)
{
    using boost::uuids::detail::md5;
    static constexpr size_t DIGEST_OFFSET = sizeof(md5::digest_type)/sizeof(int);
    md5 hash;
    md5::digest_type digest;
    std::string result;
    hash.process_bytes(buff, buf_size);
    hash.get_digest(digest);

    const auto start = reinterpret_cast<const int*>(&digest);
    boost::algorithm::hex(start,
                          start + DIGEST_OFFSET,
                          std::back_inserter(result));

    return result;
}
