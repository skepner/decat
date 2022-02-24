#pragma once

#include <zlib.h>

#include "process.hh"

// ----------------------------------------------------------------------

class GZip_Compressor
{
  public:
    static constexpr ssize_t BufSize = 409600;
    static constexpr const unsigned char Signature[] = {0x1F, 0x8B};

    using stream = z_stream;

    static inline std::string name() { return "gzip"; }

    static inline void destroy(stream& strm) { inflateEnd(&strm); }

    static inline bool compressed(std::string_view input)
    {
        if (input.size() < sizeof(Signature))
            return false;
        return std::memcmp(input.data(), Signature, sizeof(Signature)) == 0;
    }

    static inline stream init()
    {
        stream strm;
        strm.zalloc = nullptr;
        strm.zfree = nullptr;
        strm.opaque = nullptr;
        if (inflateInit2(&strm, 15 + 32) != Z_OK) // 15 window bits, and the +32 tells zlib to to detect if using gzip or zlib
            throw compressor_failed("bz2 decompression failed during initialization");
        return strm;
    }

    static inline void next_in(stream& strm, std::string_view input)
        {
            strm.next_in = reinterpret_cast<decltype(strm.next_in)>(const_cast<char*>(input.data()));
        }

    static inline decompression_result decompress(stream& strm)
    {
        switch (inflate(&strm, Z_NO_FLUSH)) {
            case Z_OK:
                return decompression_result::OK;
            case Z_STREAM_END:
                return decompression_result::END;
            default:
                return decompression_result::ERROR;
        }
        return decompression_result::ERROR;
    }
};

using GZip = Decompressor<GZip_Compressor>;

// ----------------------------------------------------------------------
