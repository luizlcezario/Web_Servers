#include <Configuration.hpp>


Config::Configuration::Configuration() : isError(false), isLocation(false)
{
}

Config::Configuration::~Configuration()
{
    for (std::vector<Server *>::iterator it = _config.begin(); it != _config.end(); it++)
        delete *it;
    _config.clear();
}

std::vector<Config::Server *> Config::Configuration::getConfig() {
    return _config;
}

void Config::Configuration::_populateServer(Config::Server *server, std::string line) {
    static std::string location;

    if (utils::starts_with(line, "[")) {
        line = utils::trim(utils::trim(line, "[]"));
        if (line ==  std::string(SLABEL) + "." + std::string(ERRORLABEL)) {
            isError = true;
            isLocation = false;
        } else if (utils::starts_with(line, SLABEL + std::string(".") + LOCALABEL + std::string(".\""))) {
            isLocation = true;
            isError = false;
        } else {
            throw Excp::BadLabel(line);
        }
    } else if (line.find("=")) {
        std::string key = utils::trim(line.substr(0, line.find("=")));
        std::string value = utils::trim(line.substr(line.find("=") + 1));
        if (isError)
            server->setErrorPages(atoi(key.c_str()), value);
        else if (isLocation) 
            server->setLocationsConfig(location, key, value);
        else
            server->setConfig(key, value);
    }
}


void Config::Configuration::loadFile(std::string filename) throw(Excp::FileNotOpen, Excp::WrongFile, Excp::BadLabel)
{
    std::ifstream config_file(filename.c_str());
    std::string line;
    Config::Server *server = NULL;

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
            if (line == SLABEL) {
                if (server != NULL) {
                    server->parseConfig();
                    _config.push_back(server);
                    isError = false;
                    isLocation = false;
                }
                server = new Config::Server();
            }
            else
                throw Excp::BadLabel(line);
        }
        else {
            _populateServer(server, line);
        }
    }
    if (server != NULL) {
        server->parseConfig();
        _config.push_back(server); 
    }
}
