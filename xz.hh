#pragma once

#pragma GCC diagnostic push
#ifdef __clang__
#pragma GCC diagnostic ignored "-Wdocumentation"
#pragma GCC diagnostic ignored "-Wreserved-macro-identifier"
#endif
#define lzma_nothrow
#include <lzma.h>
#pragma GCC diagnostic pop

#include "process.hh"

// ----------------------------------------------------------------------

class XZ_Compressor
{
  public:
    static constexpr ssize_t BufSize = 409600;
    static constexpr const unsigned char Signature[] = {0xFD, '7', 'z', 'X', 'Z', 0x00};

    using stream = lzma_stream;

    static inline std::string name() { return "xz"; }

    static inline void destroy(stream& strm) { lzma_end(&strm); }

    static inline bool compressed(std::string_view input)
    {
        if (input.size() < sizeof(Signature))
            return false;
        return std::memcmp(input.data(), Signature, sizeof(Signature)) == 0;
    }

    static inline stream init()
    {
        stream strm = LZMA_STREAM_INIT;
        if (lzma_stream_decoder(&strm, UINT64_MAX, LZMA_TELL_UNSUPPORTED_CHECK | LZMA_CONCATENATED) != LZMA_OK)
            throw compressor_failed("lzma decompression failed");
        return strm;
    }

    static inline void next_in(stream& strm, std::string_view input)
        {
            strm.next_in = reinterpret_cast<decltype(strm.next_in)>(input.data());
        }

    static inline decompression_result decompress(stream& strm)
    {
        switch (lzma_code(&strm, LZMA_RUN)) {
            case LZMA_OK:
                return decompression_result::OK;
            case LZMA_STREAM_END:
                return decompression_result::END;
            case LZMA_NO_CHECK:
            case LZMA_UNSUPPORTED_CHECK:
            case LZMA_GET_CHECK:
            case LZMA_MEM_ERROR:
            case LZMA_MEMLIMIT_ERROR:
            case LZMA_FORMAT_ERROR:
            case LZMA_OPTIONS_ERROR:
            case LZMA_DATA_ERROR:
            case LZMA_BUF_ERROR:
            case LZMA_PROG_ERROR:
                return decompression_result::ERROR;
        }
        return decompression_result::ERROR;
    }
};

using XZ = Decompressor<XZ_Compressor>;

// ----------------------------------------------------------------------
