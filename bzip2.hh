#pragma once

#include <bzlib.h>

#include "process.hh"

// ----------------------------------------------------------------------

class BZip2 : public Compressor
{
  public:
    ~BZip2() override;

    static bool compressed(std::string_view input);
    std::string decompress(std::string_view input) override;

  private:
    bz_stream strm_{};
    bool initialized_{false};
};

// ----------------------------------------------------------------------
