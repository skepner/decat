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

class XZ : public Compressor
{
  public:
    ~XZ() override;

    static bool compressed(std::string_view input);
    std::string decompress(std::string_view input) override;

  private:
    lzma_stream strm_{};
    bool initialized_{false};
};

// ----------------------------------------------------------------------
