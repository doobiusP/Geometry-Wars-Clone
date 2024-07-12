#include "game.hpp"

Game::Game(const std::string &pathToConfig)
{
    std::ifstream config(pathToConfig);
    if (!config)
    {
        std::cerr << "Failed to open config file: " << pathToConfig << std::endl;
        std::exit(EXIT_FAILURE);
    }
    int vMin, vMax, radius;
    float SCR_WIDTH, SCR_HEIGHT;
    config >> vMin >> vMax >> radius >> SCR_WIDTH >> SCR_HEIGHT;

    if (config.fail())
    {
        std::cerr << "Error reading config file: " << pathToConfig << std::endl;
        std::exit(EXIT_FAILURE);
    }
}
