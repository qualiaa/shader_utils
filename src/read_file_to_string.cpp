#include "read_file_to_string.hpp"
#include <sstream>
#include <fstream>

std::string read_file_to_string(std::string const& file_path)
{
    // XXX: Point of failure - no file
    std::ifstream current_file{file_path};
    std::stringstream sstream;
    sstream << current_file.rdbuf();
    return sstream.str();
}
