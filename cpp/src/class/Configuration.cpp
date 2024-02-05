#include <Configuration.hpp>

Config::Configuration::Configuration()
{
}

Config::Configuration::~Configuration()
{
}

void Config::Configuration::loadFile(std::string filename) throw(std::invalid_argument, std::runtime_error)
{
    std::ifstream config_file(filename.c_str());
    std::string line;

    if (!utils::ends_with(filename, ".toml"))
        throw std::invalid_argument("File is not a .config file");
    if (!config_file.is_open())
        throw std::runtime_error("Error: Could not open file " + filename);
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
                throw std::invalid_argument("Error: Unknown label " + line);
        } else 
            std::cout << line << std::endl;
    }
}
