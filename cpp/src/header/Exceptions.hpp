
#ifndef EXPECTIONS_HPP
#define EXPECTIONS_HPP

#include <exception>
#include <string>

namespace Excp {

class FileNotOpen : public std::exception
{
    public:
    std::string _filename;
    FileNotOpen(std::string filename): std::exception(), _filename("File" + filename + "File not open") {};
     ~FileNotOpen() throw() {}
   const char* what(void) const throw() {
        return _filename.c_str();
    };
};

class WrongFile : public std::exception
{
    public:
    std::string _filename;
    WrongFile(std::string filename) : _filename("File " + filename + " is not a .toml file") {};
    ~WrongFile() throw() {}
   const char* what(void) const throw() {
        return _filename.c_str();
    };
};
class BadLabel : public std::exception
{
    public:
    std::string _value;
    BadLabel(std::string label) : _value("Error: Unknown label " + label) {};
    ~BadLabel() throw() {}
     const char *what() const throw() {
        return _value.c_str();
    }
};
class BadValue : public std::exception
{
    public:
    std::string _value;
    BadValue(std::string value) : _value("Error: Unknown value " + value) {};
    ~BadValue() throw() {}
    const char *what() const throw() {
        return _value.c_str();
    }
};

};
#endif // EXPECTIONS_HPP