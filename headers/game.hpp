#pragma once
#include "renderer.hpp"
#include "window.hpp"
#include "entitymanager.hpp"

#include <random>
#include <fstream>
#include <iostream>

class Game
{
public:
    Game(const std::string &pathToConfig);
    void run();

    ~Game();
    Game(const Game &) = delete;
    Game &operator=(const Game &) = delete;
    Game(Game &&) = delete;
    Game &operator=(Game &&) = delete;

private:
    Window m_window;
    EntityManager2 m_manager;
    Renderer *m_renderer; // For some reason default constructor and copy assignment of Renderer is implicity deleted, so have to use dynamic allocation...
    int m_score;
    int m_currentFrame;
    float m_lastEnemySpawnTime;
    bool m_paused;
    bool m_running;

    int m_vMin;
    int m_vMax;
};