#include <Configuration.hpp>


Config::Configuration::Configuration()
{
}

Config::Configuration::~Configuration()
{
}

void Config::Configuration::loadFile(std::string filename) throw(Excp::FileNotOpen, Excp::WrongFile, Excp::BadLabel)
{
    std::ifstream config_file(filename.c_str());
    std::string line;

    if (!utils::ends_with(filename, ".toml"))
        throw Excp::WrongFile(filename);
    if (!config_file.is_open())
        throw Excp::FileNotOpen(filename);
    while (std::getline(config_file, line))
    {
        if (line.find("#") != std::string::npos)
            line = line.substr(0, line.find("#"));
        if (line.empty())
            continue;
        line = utils::trim(line);
        if (utils::starts_with(line, "[[")) {
            line = utils::trim(utils::trim(line, "[]"));
            if (line == SLABEL)
                std::cout << "Found server label" << std::endl;
            else
                throw Excp::BadLabel(line);
        } else 
            std::cout << line << std::endl;
    }
}
