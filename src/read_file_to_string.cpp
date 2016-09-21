#include "read_file_to_string.hpp"
#include <sstream>
#include <stdexcept>
#include <fstream>

std::string read_file_to_string(std::string const& file_path)
{
    std::ifstream current_file{file_path};
    if (not current_file) {
        std::stringstream errstream;
        errstream << "Could not open file '" << file_path << '\'';
        throw std::runtime_error(errstream.str());
    }
    std::stringstream sstream;
    sstream << current_file.rdbuf();
    return sstream.str();
}
