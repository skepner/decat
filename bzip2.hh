#pragma once

#include <iostream>
#include <bzlib.h>

#include "process.hh"

// ----------------------------------------------------------------------

class BZip2_Compressor
{
  public:
    static constexpr ssize_t BufSize = 409600;
    static constexpr const unsigned char Signature[] = {'B', 'Z', 'h'};

    using stream = bz_stream;

    static inline std::string name() { return "bzip2"; }

    static inline void destroy(stream& strm)
        {
            BZ2_bzDecompressEnd(&strm);
        }

    static inline bool compressed(std::string_view input)
    {
        if (input.size() < sizeof(Signature))
            return false;
        return std::memcmp(input.data(), Signature, sizeof(Signature)) == 0;
    }

    static inline stream init()
    {
        stream strm;
        strm.bzalloc = nullptr;
        strm.bzfree = nullptr;
        strm.opaque = nullptr;
        if (BZ2_bzDecompressInit(&strm, 0 /*verbosity*/, 0 /* not small */) != BZ_OK)
            throw compressor_failed("bz2 decompression failed during initialization");
        return strm;
    }

    static inline void next_in(stream& strm, std::string_view input)
        {
            strm.next_in = const_cast<decltype(strm.next_in)>(input.data());
        }

    static inline decompression_result decompress(stream& strm)
    {
        const auto res = BZ2_bzDecompress(&strm);
        switch (res) {
            case BZ_OK:
                std::cerr << ">> bzip2 OK\n";
                return decompression_result::OK;
            case BZ_STREAM_END:
                std::cerr << ">> bzip2 END\n";
                return decompression_result::END;
            default:
                std::cerr << "> bzip2 error: " << res << '\n';
                return decompression_result::ERROR;
        }
        return decompression_result::ERROR;
    }
};

using BZip2 = Decompressor<BZip2_Compressor>;

// ----------------------------------------------------------------------
