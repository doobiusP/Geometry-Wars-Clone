#pragma once
#include "window.hpp"
#include "entitymanager.hpp"
#include "renderer.hpp"

#include <random>

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
    EntityManager m_manager;
    Renderer *m_renderer; // Default constructor and copy assignment of Renderer is implicitly deleted, so having to use dynamic allocation...

    int m_score;

    int m_currentFrame;
    float m_lastEnemySpawnTime;

    bool m_paused;
    bool m_running;

    std::mt19937 m_gen;
    std::uniform_int_distribution<> m_rng;

    int m_vMin;
    int m_vMax;
};