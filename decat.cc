#include <iostream>

#include "read.hh"

// ----------------------------------------------------------------------

int main(int argc, const char* const* argv)
{
    try {
        if (argc > 1) {
            for (int arg_no = 1; arg_no < argc; ++arg_no)
                read_file(argv[arg_no]);
        }
        else {
            read_stdin();
        }
    }
    catch (std::exception& err) {
        std::cerr << "> " << err.what() << '\n';
    }
}

// ----------------------------------------------------------------------
