#pragma once
#include "window.hpp"
#include "entitymanager.hpp"
#include "renderer.hpp"
#include "randomgenerator.hpp"
#include "logger.hpp"

#include <memory>

struct GameConfig
{
    float g_windowWidth;
    float g_windowHeight;
    float g_standardSpeed;
    float g_standardRadius;
    int g_vertexMin;
    int g_vertexMax;
    int g_maxBigEnemies;
    glm::vec4 g_backgroundColor;
};
struct EnemyConfig
{
    float g_smallEnemySpeedFactor;
    float g_smallEnemyRotationFactor;
    float g_smallEnemyRadiusFactor;
    float g_smallEnemyLifespan;
    glm::vec4 g_smallEnemyOutlineColor;

    float g_bigEnemySpeedFactor;
    float g_bigEnemyRotationFactor;
    float g_bigEnemySpawnRate;
    glm::vec4 g_bigEnemyOutlineColor;
};
struct BulletConfig
{
    float g_bulletSpeedFactor;
    float g_bulletRotationFactor;
    float g_bulletRadiusFactor;
    float g_bulletLifespan;
    float g_bulletSpawnLimit;
    glm::vec4 g_bulletColor;
    glm::vec4 g_bulletOutlineColor;
};
struct PlayerConfig
{
    float g_playerSpeedFactor;
    float g_playerRotationFactor;
    glm::vec4 g_playerColor;
    glm::vec4 g_playerOutlineColor;
};

class Game
{
public:
    Game(const std::string &pathToConfig);
    void run();

private:
    Window m_window;
    EntityManager m_manager;
    std::unique_ptr<Renderer> m_renderer; // Renderer needs data from config.txt and no default constructor; unique_ptr for delayed construction
    RandomGenerator m_rg;

    GameConfig m_gConfig;
    EnemyConfig m_eConfig;
    BulletConfig m_bConfig;
    PlayerConfig m_pConfig;

    std::vector<size_t> m_enemiesDestroyedThisFrame;
    int m_numBigEnemies;
    size_t m_playerID;

    int m_score;

    int m_currentFrame;
    float m_currentTime;
    float m_lastEnemySpawnTime;
    float m_lastBulletSpawnTime;

    bool m_spaceKeyPressed;
    bool m_paused;

    int m_frameCount;
    float m_timeAccumulated;
    float m_fps;

    // ------------------------------------ Systems/Methods ------------------------------------ //

    void togglePause();

    void sMovement(float deltaTime);
    void sUserInput();
    void sLifespan(float deltaTime);
    void sRender();
    void sEnemySpawner();
    void sCollision();

    void spawnPlayer();
    void spawnEnemy();
    void spawnSmallEnemies(size_t enemy_id);
    void spawnBullet(const glm::vec2 &mousePos);

    bool checkCollision(size_t id1, size_t id2);
    void updateFPS(float deltaTime);

    static void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);
    void handleKeyPress(int key, int action);
};
