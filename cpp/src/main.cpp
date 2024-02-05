#include <iostream>
#include <fstream>
#include <cstdio>
#include <Configuration.hpp>

int main(int argc, char *argv[])
{
	Config::Configuration config;

	std::string path = "./configuration/server.toml";
	if (argc != 2)
	{
		FILE *file = std::fopen(path.c_str(), "r");
		if (file == NULL)
		{
			std::cerr << "Usage: " << argv[0] << " <filename>.toml" << std::endl
					  << "Usage optional: save the file in the same directory as the executable in the folder configuration as 'server.config'" << std::endl;
			return 1;
		}
		std::fclose(file);
	}
	else
	{
		path = argv[1];
	}
	try
	{
		std::cout << "Loading file: " << path << std::endl;
		config.loadFile(path);
	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() << '\n';
	}
}