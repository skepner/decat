#include <unistd.h>

#include "process.hh"

#include "gzip.hh"

// ----------------------------------------------------------------------

class Plain : public Compressor
{
  public:
    static bool compressed(std::string_view input) { return true; }
    std::string decompress(std::string_view input) override { return std::string{input}; }
};

// ----------------------------------------------------------------------

void Processor::process(std::string_view chunk)
{
    if (!compressor_) {
        if (GZip::compressed(chunk))
            compressor_ = std::make_unique<GZip>();
        else
            compressor_ = std::make_unique<Plain>();
    }

    const auto decompressed = compressor_->decompress(chunk);
    write(STDOUT_FILENO, decompressed.data(), decompressed.size());
}

// ----------------------------------------------------------------------
