#pragma once

#include <string_view>
#include <string>
#include <stdexcept>
#include <memory>

// ----------------------------------------------------------------------

class compressor_failed : public std::runtime_error { public: using std::runtime_error::runtime_error; };

// ----------------------------------------------------------------------

class Compressor
{
  public:
    Compressor() = default;
    virtual ~Compressor() = default;

    virtual std::string decompress(std::string_view input) = 0;
};

// ----------------------------------------------------------------------

class Processor
{
  public:
    Processor() = default;

    void process(std::string_view chunk);

  private:
    std::unique_ptr<Compressor> compressor_{};
};

// ----------------------------------------------------------------------


// ----------------------------------------------------------------------
