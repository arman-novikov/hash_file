#include "md5hasher.hpp"

#include <boost/uuid/detail/md5.hpp>
#include <boost/algorithm/hex.hpp>

using boost::uuids::detail::md5;

static std::string toString(const md5::digest_type& digest)
{
    const auto start = reinterpret_cast<const int*>(&digest);
    const auto end = start + (sizeof(md5::digest_type)/sizeof(int));
    std::string result;
    boost::algorithm::hex(start, end, std::back_inserter(result));
    return result;
}

std::string Md5Hasher::hash(const void* buff, size_t buf_size)
{
    md5 hash;
    md5::digest_type digest;
    hash.process_bytes(buff, buf_size);
    hash.get_digest(digest);

    return toString(digest);
}
