#include <fcntl.h>
#include <sys/select.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <cerrno>
#include <string>
#include <array>
#include <stdexcept>

#include "read.hh"
#include "process.hh"

// ======================================================================

void read_file_descriptor(int fd)
{
    using namespace std::string_literals;
    if (fcntl(fd, F_SETFL, O_NONBLOCK) < 0)
        throw std::runtime_error{"reading file description: "s + std::strerror(errno)};

    constexpr size_t buffer_size = 1024;
    std::array<char, buffer_size> buffer;

    Processor processor;
    while (true) {
        fd_set fds;
        FD_ZERO(&fds);
        FD_SET(fd, &fds);
        const auto ret = select(fd + 1, &fds, nullptr, nullptr, nullptr);
        if (ret == -1)
            throw std::runtime_error{"selecting on file descriptor: "s + std::strerror(errno)};

        const auto nread = read(fd, buffer.data(), buffer_size);
        if (nread == -1) {
            if (errno != EAGAIN)
                throw std::runtime_error{"reading file description: "s + std::strerror(errno)};
        }
        else if (nread == 0) {  // EOF
            break;
        }
        else {
            processor.process(std::string_view(buffer.data(), static_cast<size_t>(nread)));
        }

        // getline(std::cin, message);
        // if (std::cin.eof()) {
        //   std::cout << "eof." << std::endl;
        //   break;
        // }
        // std::cout << "read: " << message << std::endl;
    }
}

// ----------------------------------------------------------------------

class mmapped
{
  public:
    mmapped(const std::filesystem::path& filename)
    {
        if (std::filesystem::exists(filename)) {
            mmapped_len_ = std::filesystem::file_size(filename);
            fd_ = open(filename.c_str(), O_RDONLY);
            if (fd_ >= 0) {
                mmapped_ = reinterpret_cast<char*>(mmap(nullptr, mmapped_len_, PROT_READ, MAP_FILE | MAP_PRIVATE, fd_, 0));
                if (mmapped_ == MAP_FAILED)
                    throw std::runtime_error{"cannot read " + filename.native() + ": " + std::strerror(errno)};
            }
            else
                throw std::runtime_error{"cannot open " + filename.native() + ": " + std::strerror(errno)};
        }
        else
            throw std::runtime_error{filename.native() + ": no such file"};
    }
    mmapped(const mmapped&) = delete;
    mmapped operator=(const mmapped&) = delete;

    ~mmapped()
    {
        if (fd_ > 2) {
            if (mmapped_)
                munmap(mmapped_, mmapped_len_);
            close(fd_);
        }
    }

    operator std::string_view() { return {mmapped_, mmapped_len_}; }

  private:
    int fd_{-1};
    char* mmapped_{nullptr};
    size_t mmapped_len_{0};
};

// ----------------------------------------------------------------------

void read_mmap(const std::filesystem::path& filename)
{
    constexpr size_t chunk_size = 1024;
    mmapped mapped{filename};
    Processor processor;
    const std::string_view data = mapped;
    for (size_t pos{0}; pos < data.size(); pos += chunk_size)
        processor.process(data.substr(pos, chunk_size));
}

// ----------------------------------------------------------------------
