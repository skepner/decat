#include <unistd.h>

#include "process.hh"

#include "brotli.hh"
#include "bzip2.hh"
#include "gzip.hh"
#include "xz.hh"

// ----------------------------------------------------------------------

class Plain_Compressor
{
  public:
    using stream = int;
    static constexpr ssize_t BufSize = 409600;
    static inline std::string name() { return "plain"; }
    static inline void destroy(stream&) {}
    static inline bool compressed(std::string_view /*input*/) { return true; }
};

template <> inline std::string Decompressor<Plain_Compressor>::decompress(std::string_view input) { return std::string{input}; }

using Plain = Decompressor<Plain_Compressor>;

// ----------------------------------------------------------------------

void Processor::process(std::string_view chunk)
{
    if (!compressor_) {
        if (GZip::compressed(chunk))
            compressor_ = std::make_unique<GZip>();
        else if (XZ::compressed(chunk))
            compressor_ = std::make_unique<XZ>();
        else if (Brotli::compressed(chunk))
            compressor_ = std::make_unique<Brotli>();
        else if (BZip2::compressed(chunk))
            compressor_ = std::make_unique<BZip2>();
        else
            compressor_ = std::make_unique<Plain>();
    }

    const auto decompressed = compressor_->decompress(chunk);
    write(STDOUT_FILENO, decompressed.data(), decompressed.size());
}

// ----------------------------------------------------------------------
