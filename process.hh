#pragma once

#include <string_view>
#include <string>
#include <stdexcept>
#include <memory>

// ----------------------------------------------------------------------

class compressor_failed : public std::runtime_error { public: using std::runtime_error::runtime_error; };

// ----------------------------------------------------------------------

enum class decompression_result { OK, END, ERROR };

class DecompressorBase
{
  public:
    virtual ~DecompressorBase() = default;
    virtual std::string decompress(std::string_view input) = 0;
};

// ----------------------------------------------------------------------

template <typename Compressor> class Decompressor : public DecompressorBase
{
  public:
    using Base = Compressor;

    Decompressor() = default;
    ~Decompressor() override { if (initialized_) Compressor::destroy(strm_); }

    static inline bool compressed(std::string_view input) { return Compressor::compressed(input); }

    std::string decompress(std::string_view input) override
    {
        if (!initialized_) {
            strm_ = Compressor::init();
            initialized_ = true;
        }

        Compressor::next_in(strm_, input);
        strm_.avail_in = static_cast<decltype(strm_.avail_in)>(input.size());
        std::string output(Compressor::BufSize, '\x00');
        ssize_t offset = 0;
        bool done{false};
        while (!done) {
            strm_.next_out = reinterpret_cast<decltype(strm_.next_out)>(&*(output.begin() + offset));
            strm_.avail_out = Compressor::BufSize;
            switch (Compressor::decompress(strm_)) {
                case decompression_result::OK:
                    if (strm_.avail_out > 0) {
                        output.resize(output.size() - strm_.avail_out);
                        done = true;
                    }
                    else {
                        offset += Compressor::BufSize;
                        output.resize(static_cast<size_t>(offset) + Compressor::BufSize);
                    }
                    break;
                case decompression_result::END:
                    output.resize(static_cast<size_t>(offset) + Compressor::BufSize - strm_.avail_out);
                    done = true;
                    break;
                case decompression_result::ERROR:
                    throw compressor_failed(Compressor::name() + " decompression failed");
            }
        }
        return output;
    }

  private:
    typename Compressor::stream strm_{};
    bool initialized_{false};
};

// ----------------------------------------------------------------------

class Processor
{
  public:
    Processor() = default;

    void process(std::string_view chunk);

  private:
    std::unique_ptr<DecompressorBase> compressor_{};
};

// ----------------------------------------------------------------------
