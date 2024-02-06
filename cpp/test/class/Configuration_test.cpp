#include "unitTests.hpp"

void testFileName() {
    Config::Configuration config;
    std::cout << "\033[1;33mTest Configuration::loadFile\033[0;0m" << std::endl;
    assertEq(doesThrow([&config]() { config.loadFile("test.conf"); }, Excp::WrongFile("test.conf") ), "Test if verify extension");
    assertEq(doesThrow([&config]() { config.loadFile("test.toml "); }, Excp::WrongFile("test.toml ") ), "Test if verify extension");
    assertEq(doesThrow([&config]() { config.loadFile("test.toml"); }, Excp::FileNotOpen("test.toml") ), "Test if verify file exists");
}

