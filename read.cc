#include <fcntl.h>
#include <sys/select.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <cerrno>
#include <cstring>
#include <stdexcept>

#include "read.hh"

// ======================================================================

void read_file_descriptor(int fd)
{
    while (true) {
        fd_set fds;
        FD_ZERO(&fds);
        FD_SET(fd, &fds);
        const auto ret = select(fd + 1, &fds, nullptr, nullptr, nullptr);
        if (ret == -1)
            throw std::runtime_error{std::strerror(errno)};
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
    mmapped mapped{filename};

}

// ----------------------------------------------------------------------
