#include <unistd.h>

#include "process.hh"

// ----------------------------------------------------------------------

void Processor::process(std::string_view chunk)
{
    write(STDOUT_FILENO, chunk.data(), chunk.size());
}

// ----------------------------------------------------------------------
