#include "utils.hpp"


std::vector<std::string> utils::split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    char* cstr = new char[s.size() + 1];
    std::strcpy(cstr, s.c_str());

    char* token = std::strtok(cstr, &delim);
    while (token != NULL) {
        elems.push_back(std::string(token));
        token = std::strtok(NULL, &delim);
    }

    delete[] cstr;
    return elems;
}