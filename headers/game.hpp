#pragma once
#include "renderer.hpp"
#include "window.hpp"
#include "entitymanager.hpp"

#include <random>
#include <fstream>

class Game
{
public:
    Game(const std::string &pathToConfig);

private:
    Window m_window;
    EntityManager2 m_manager;
    int m_score;
    int m_currentFrame;
    int m_lastEnemySpawnTime;
    bool m_paused;
    bool m_running;
};