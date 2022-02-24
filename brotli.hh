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

class Brotli : public Compressor
{
    enum class check_if_compressed { no, yes };

  public:
    ~Brotli() override;

    static bool compressed(std::string_view input);
    std::string decompress(std::string_view input) override { return decompress_and_check(input, check_if_compressed::no); }

  private:
    BrotliDecoderState* decoder_{nullptr};

    std::string decompress_and_check(std::string_view input, check_if_compressed cic);
};

// ----------------------------------------------------------------------
