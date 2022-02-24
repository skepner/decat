#include <iostream>

#include "gzip.hh"

// ----------------------------------------------------------------------

GZip::~GZip()
{
    inflateEnd(&strm_);
}

// ----------------------------------------------------------------------

namespace gzip_internal
{
    constexpr const unsigned char Signature[] = {0x1F, 0x8B};
    constexpr ssize_t BufSize = 409600;

} // namespace gzip_internal

bool GZip::compressed(std::string_view input)
{
    if (input.size() < sizeof(gzip_internal::Signature))
        return false;
    return std::memcmp(input.data(), gzip_internal::Signature, sizeof(gzip_internal::Signature)) == 0;
}

// ----------------------------------------------------------------------

std::string GZip::decompress(std::string_view input)
{
    if (!initialized_) {
        if (inflateInit2(&strm_, 15 + 32) != Z_OK) // 15 window bits, and the +32 tells zlib to to detect if using gzip or zlib
            throw compressor_failed("gzip decompression failed during initialization");
        initialized_ = true;
    }

    strm_.next_in = reinterpret_cast<decltype(strm_.next_in)>(const_cast<char*>(input.data()));
    strm_.avail_in = static_cast<decltype(strm_.avail_in)>(input.size());

    std::string output(gzip_internal::BufSize, '\x00');
    ssize_t offset = 0;
    for (;;) {
        strm_.next_out = reinterpret_cast<decltype(strm_.next_out)>(output.data() + offset);
        strm_.avail_out = gzip_internal::BufSize;
        auto const r = inflate(&strm_, Z_NO_FLUSH);
        if (r == Z_OK) {
            if (strm_.avail_out > 0) { // done
                output.resize(output.size() - strm_.avail_out);
                break;
            }
            else { // output grow
                offset += gzip_internal::BufSize;
                output.resize(static_cast<size_t>(offset + gzip_internal::BufSize));
            }
        }
        else if (r == Z_STREAM_END) {
            output.resize(static_cast<size_t>(offset + gzip_internal::BufSize) - strm_.avail_out);
            break;
        }
        else {
            throw compressor_failed("gzip decompression failed, code: " + std::to_string(r));
        }
    }
    return output;
}

// ----------------------------------------------------------------------
