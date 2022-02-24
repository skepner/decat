#pragma once

#include <zlib.h>

#include "process.hh"

// ----------------------------------------------------------------------

class GZip : public Compressor
{
  public:
    GZip() : Compressor()
    {
        strm_.zalloc = Z_NULL;
        strm_.zfree = Z_NULL;
        strm_.opaque = Z_NULL;
    }

    ~GZip() override;

    static bool compressed(std::string_view input);
    std::string decompress(std::string_view input) override;

  private:
    z_stream strm_{};
    bool initialized_{false};
};

// ----------------------------------------------------------------------
