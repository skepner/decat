#pragma once

#include <unistd.h>
#include <string_view>

// ----------------------------------------------------------------------

void read_file(std::string_view filename);
void read_file_descriptor(int fd);
inline void read_stdin() { read_file_descriptor(STDIN_FILENO); }

// ----------------------------------------------------------------------
