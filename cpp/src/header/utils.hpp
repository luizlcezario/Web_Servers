#ifndef UTILS_HPP
#define UTILS_HPP

#include <string>

#define WHITESPACE " \n\r\t\f\v"

namespace utils
{
    std::string trim(const std::string &s, const std::string &replace = WHITESPACE);
    bool ends_with(std::string const &value, std::string const &ending);
    bool starts_with(std::string const &value, std::string const &ending);
};
#endif // UTILS_HPP
