#pragma once

#include <unistd.h>
#include <string_view>
#include <filesystem>

// ----------------------------------------------------------------------

void read_file_descriptor(int fd);
void read_mmap(const std::filesystem::path& filename);
inline void read_stdin() { read_file_descriptor(STDIN_FILENO); }

inline void read_file(const std::filesystem::path& filename)
{
    if (filename == "-")
        read_stdin();
    else
        read_mmap(filename);
}


// ----------------------------------------------------------------------
