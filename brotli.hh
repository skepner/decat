#pragma once

#pragma GCC diagnostic push
#ifdef __clang__
#pragma GCC diagnostic ignored "-Wdocumentation-pedantic"
#pragma GCC diagnostic ignored "-Wdocumentation-unknown-command"
#endif

#include <brotli/decode.h>

#pragma GCC diagnostic pop

#include "process.hh"

// ----------------------------------------------------------------------

class Brotli_Compressor
{
  public:
    static constexpr ssize_t BufSize = 409600;

    struct stream
    {
        BrotliDecoderState* brotli{nullptr};
        const uint8_t* next_in{nullptr};
        size_t avail_in{0};
        uint8_t* next_out{nullptr};
        size_t avail_out{0};
    };

    static inline std::string name() { return "brotli"; }

    static inline void destroy(stream& strm)
    {
        if (strm.brotli)
            BrotliDecoderDestroyInstance(strm.brotli);
    }

    static inline bool compressed(std::string_view input)
    {
        try {
            Decompressor<Brotli_Compressor> brotli;
            brotli.decompress(input.substr(0, 100));
            return true;
        }
        catch (compressor_failed&) {
            return false;
        }
    }

    static inline stream init()
    {
        stream strm;
        strm.brotli = BrotliDecoderCreateInstance(nullptr, nullptr, nullptr);
        return strm;
    }

    static inline void next_in(stream& strm, std::string_view input)
        {
            strm.next_in = reinterpret_cast<decltype(strm.next_in)>(input.data());
        }

    static inline decompression_result decompress(stream& strm)
    {
        size_t available_out = 0;
        const BrotliDecoderResult result = BrotliDecoderDecompressStream(strm.brotli, &strm.avail_in, &strm.next_in, &available_out, nullptr, nullptr);
        const uint8_t* next_out = BrotliDecoderTakeOutput(strm.brotli, &available_out);
        switch (result) {
            case BROTLI_DECODER_RESULT_SUCCESS:
            case BROTLI_DECODER_RESULT_NEEDS_MORE_INPUT:
                if (available_out != 0) {
                    if (available_out > strm.avail_out)
                        return decompression_result::ERROR;
                    std::memcpy(strm.next_out, next_out, available_out);
                    strm.avail_out -= available_out;
                }
                return decompression_result::OK;
            case BROTLI_DECODER_RESULT_ERROR:
            case BROTLI_DECODER_RESULT_NEEDS_MORE_OUTPUT:
                return decompression_result::ERROR;
        }
        return decompression_result::ERROR;
    }
};

using Brotli = Decompressor<Brotli_Compressor>;

// ----------------------------------------------------------------------
