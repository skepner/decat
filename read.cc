#include <sys/select.h>
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

void read_file(std::string_view filename)
{
    if (filename == "-") {
        read_stdin();
        return;
    }
}

// ----------------------------------------------------------------------
