#pragma once
#include "window.hpp"
#include "entitymanager.hpp"
#include "renderer.hpp"
#include "randomgenerator.hpp"

#include <random>
#include <memory>

class Game
{
public:
    Game(const std::string &pathToConfig);
    void run();

private:
    Window m_window;
    EntityManager m_manager;
    std::unique_ptr<Renderer> m_renderer; // Default constructor and copy assignment of Renderer is implicitly deleted, so having to use unique_ptr...
    RandomGenerator m_rg;

    size_t m_playerID;

    int m_score;

    int m_currentFrame;
    float m_lastEnemySpawnTime;
    float m_lastBulletSpawnTime;

    bool m_paused;

    int m_vMin;
    int m_vMax;

    float m_windowW;
    float m_windowH;

    float m_standardSpeed;
    float m_standardRadius;

    // ------------------------------------ Systems/Methods ------------------------------------ //

    void togglePause();

    void sMovement(float deltaTime);
    void sUserInput();
    void sLifespan();
    void sRender();
    void sEnemySpawner();
    void sCollision();

    void spawnPlayer();
    void spawnEnemy();
    void spawnSmallEnemies(size_t enemy_id);
    void spawnBullet(const glm::vec2 &mousePos);

    bool checkCollision(size_t id1, size_t id2);
};
