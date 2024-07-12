#include "game.hpp"

Game::Game(const std::string &pathToConfig) : m_manager{}, m_score{0}, m_currentFrame{0}, m_lastEnemySpawnTime{0}, m_lastBulletSpawnTime{0}, m_paused{0}
{
    std::ifstream config(pathToConfig);
    if (!config)
    {
        std::cerr << "Failed to open config file: " << pathToConfig << std::endl;
        std::exit(EXIT_FAILURE);
    }

    config >> m_vMin >> m_vMax >> m_standardRadius >> m_standardSpeed >> m_windowW >> m_windowH;

    if (config.fail())
    {
        std::cerr << "Error reading config file: " << pathToConfig << std::endl;
        std::exit(EXIT_FAILURE);
    }
    config.close();

    m_rg = RandomGenerator(m_windowW, m_windowH, m_standardRadius, m_vMin, m_vMax);
    m_window = Window(m_windowW, m_windowH);

    std::string pathToVert = "shaders/polyplace.glsl";
    std::string pathToFrag = "shaders/polyfill.glsl";
    std::string pathToOutlineFrag = "shaders/polyoutline.glsl";
    m_renderer = std::make_unique<Renderer>(m_vMin, m_vMax, m_standardRadius, m_windowW, m_windowH, pathToVert, pathToFrag, pathToOutlineFrag);

    spawnPlayer();
}

void Game::run()
{
    spawnEnemy();
    spawnEnemy();
    float lastTime = static_cast<float>(glfwGetTime());
    float currTime, deltaTime;

    while (!glfwWindowShouldClose(m_window.window))
    {
        currTime = static_cast<float>(glfwGetTime());
        deltaTime = currTime - lastTime;
        lastTime = currTime;

        m_manager.update();
        sUserInput();
        sMovement(deltaTime);

        sCollision();

        sRender();
        glfwPollEvents();
        ++m_currentFrame;
    }

    glfwTerminate();
}

void Game::togglePause()
{
    m_paused = !m_paused;
}

void Game::sMovement(float deltaTime)
{
    // All entity movement - no collision detection, not even with border
    auto &enemies = m_manager.getEntities(ENTITY_TYPE::ENEMY);
    auto &bullets = m_manager.getEntities(ENTITY_TYPE::BULLET);

    for (size_t enemyID : enemies)
    {
        Entity *enemy = m_manager.getEntityByID(enemyID);
        enemy->cTransform.value().pos = enemy->cTransform.value().pos + (m_standardSpeed * deltaTime * enemy->cTransform.value().vel);
        enemy->cTransform.value().angle += 3 * deltaTime;
    }

    for (size_t bulletID : bullets)
    {
        Entity *bullet = m_manager.getEntityByID(bulletID);
        bullet->cTransform.value().pos = bullet->cTransform.value().pos + (4.5f * m_standardSpeed * deltaTime * bullet->cTransform.value().vel);
        bullet->cTransform.value().angle += 5 * deltaTime;
    }

    Entity *player = m_manager.getEntityByID(m_playerID);
    CInput &playerInp = player->cInput.value();
    CTransform &playerMov = player->cTransform.value();

    if (playerInp.up)
    {
        playerMov.vel += glm::vec2(0.0f, -1.0f);
        playerInp.up = 0;
    }
    if (playerInp.down)
    {
        playerMov.vel += glm::vec2(0.0f, 1.0f);
        playerInp.down = 0;
    }
    if (playerInp.left)
    {
        playerMov.vel += glm::vec2(-1.0f, 0.0f);
        playerInp.left = 0;
    }
    if (playerInp.right)
    {
        playerMov.vel += glm::vec2(1.0f, 0.0f);
        playerInp.right = 0;
    }
    if (playerInp.shoot && m_currentFrame - m_lastBulletSpawnTime >= 100)
    {
        double xpos, ypos;
        glfwGetCursorPos(m_window.window, &xpos, &ypos);
        spawnBullet(glm::vec2(static_cast<float>(xpos), static_cast<float>(ypos)));

        m_lastBulletSpawnTime = m_currentFrame;
        playerInp.shoot = 0;
    }

    if (playerMov.vel != glm::vec2(0.0f, 0.0f))
        playerMov.vel = glm::normalize(playerMov.vel);

    playerMov.pos = playerMov.pos + (3.5f * m_standardSpeed * deltaTime * playerMov.vel);
    playerMov.angle -= 3 * deltaTime;
    playerMov.vel = glm::vec2(0.0f, 0.0f);
}

void Game::sUserInput()
{
    CInput &playerInp = m_manager.getEntityByID(m_playerID)->cInput.value();
    if (glfwGetKey(m_window.window, GLFW_KEY_W) == GLFW_PRESS)
    {
        playerInp.up = 1;
    }
    if (glfwGetKey(m_window.window, GLFW_KEY_A) == GLFW_PRESS)
    {
        playerInp.left = 1;
    }
    if (glfwGetKey(m_window.window, GLFW_KEY_S) == GLFW_PRESS)
    {
        playerInp.down = 1;
    }
    if (glfwGetKey(m_window.window, GLFW_KEY_D) == GLFW_PRESS)
    {
        playerInp.right = 1;
    }
    if (!playerInp.shoot && glfwGetKey(m_window.window, GLFW_KEY_SPACE) == GLFW_PRESS)
    {
        playerInp.shoot = 1;
    }
}

void Game::sRender()
{
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    m_renderer->DrawEntity(m_manager.getEntityByID(m_playerID));

    auto &enemies = m_manager.getEntities(ENTITY_TYPE::ENEMY);
    auto &bullets = m_manager.getEntities(ENTITY_TYPE::BULLET);

    for (size_t enemyID : enemies)
    {
        m_renderer->DrawEntity(m_manager.getEntityByID(enemyID));
    }

    for (size_t bulletID : bullets)
    {
        m_renderer->DrawEntity(m_manager.getEntityByID(bulletID));
    }

    glfwSwapBuffers(m_window.window);
}

void Game::sCollision()
{
    auto &enemies = m_manager.getEntities(ENTITY_TYPE::ENEMY);
    auto &bullets = m_manager.getEntities(ENTITY_TYPE::BULLET);

    for (auto enemyID : enemies)
    {
        if (checkCollision(m_playerID, enemyID))
        {
            // Game over mechanics -- TODO
            CTransform &playerMov = m_manager.getEntityByID(m_playerID)->cTransform.value();
            playerMov.pos = glm::vec2(m_windowW / 2, m_windowH / 2);
            m_manager.getEntityByID(enemyID)->destroy();
        }

        for (auto bulletID : bullets)
        {
            if (checkCollision(bulletID, enemyID))
            {
                m_manager.getEntityByID(enemyID)->destroy();
                m_manager.getEntityByID(bulletID)->destroy();
            }
        }
    }

    for (auto bulletID : bullets)
    {
        glm::vec2 &bulletPos = m_manager.getEntityByID(bulletID)->cTransform.value().pos;
        float collRadius = m_manager.getEntityByID(bulletID)->cCollision.value().radius;

        if (bulletPos.x - collRadius <= 0.0f || bulletPos.x + collRadius >= m_windowW || bulletPos.y - collRadius <= 0.0f || bulletPos.y + collRadius >= m_windowH)
        {
            m_manager.getEntityByID(bulletID)->destroy();
        }
    }
    for (auto enemyID : enemies)
    {
        Entity *enemy = m_manager.getEntityByID(enemyID);
        float collRadius = enemy->cCollision.value().radius;
        CTransform &enemyMov = enemy->cTransform.value();

        if (enemyMov.pos.x - collRadius <= 0.0f || enemyMov.pos.x + collRadius >= m_windowW)
        {
            enemyMov.vel.x *= -1;
        }
        if (enemyMov.pos.y - collRadius <= 0.0f || enemyMov.pos.y + collRadius >= m_windowH)
        {
            enemyMov.vel.y *= -1;
        }
    }

    Entity *player = m_manager.getEntityByID(m_playerID);
    float collRadius = player->cCollision.value().radius;
    CTransform &playerMov = player->cTransform.value();

    if (playerMov.pos.x - collRadius <= 0.0f)
    {
        playerMov.pos.x = collRadius;
    }
    if (playerMov.pos.x + collRadius >= m_windowW)
    {
        playerMov.pos.x = m_windowW - collRadius;
    }
    if (playerMov.pos.y - collRadius <= 0.0f)
    {
        playerMov.pos.y = collRadius;
    }
    if (playerMov.pos.y + collRadius >= m_windowH)
    {
        playerMov.pos.y = m_windowH - collRadius;
    }
}

void Game::spawnPlayer()
{
    m_playerID = m_manager.createEntity(ENTITY_TYPE::PLAYER);
    Entity *player = m_manager.getEntityByID(m_playerID);

    player->addTransform(glm::vec2(m_windowW / 2, m_windowH / 2), glm::vec2(0.0f, 0.0f), 0.0f);
    player->addShape(m_vMax, 1.0f, glm::vec4(0.0f, 0.0f, 0.0f, 1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
    player->addCollision(m_standardRadius);
    player->addInput();
}

void Game::spawnEnemy()
{
    size_t enemyID = m_manager.createEntity(ENTITY_TYPE::ENEMY);
    Entity *enemy = m_manager.getEntityByID(enemyID);
    glm::vec2 currPos = m_rg.generateCoordinates();

    enemy->addTransform(currPos, glm::normalize(currPos - m_rg.generateCoordinates()), 0.0f);
    enemy->addShape(m_rg.generateVertex(), 1.0f, m_rg.generateColor(), glm::vec4(1.0f, 0.1f, 0.1f, 1.0f));
    enemy->addCollision(m_standardRadius);
}

void Game::spawnBullet(const glm::vec2 &mousePos)
{
    size_t bulletID = m_manager.createEntity(ENTITY_TYPE::BULLET);
    Entity *bullet = m_manager.getEntityByID(bulletID);

    Entity *player = m_manager.getEntityByID(m_playerID);
    glm::vec2 bulletVel = glm::normalize(mousePos - player->cTransform.value().pos);
    float r = player->cCollision.value().radius;

    glm::vec2 correctPos = player->cTransform.value().pos + (1.2f * r * bulletVel);

    bullet->addTransform(correctPos, bulletVel, 0.0f);
    bullet->addShape(m_vMax, 0.2f, glm::vec4(0.3f, 0.2f, 1.0f, 1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
    bullet->addCollision(0.2f * m_standardRadius);
}

bool Game::checkCollision(size_t id1, size_t id2)
{
    Entity *e1 = m_manager.getEntityByID(id1);
    Entity *e2 = m_manager.getEntityByID(id2);

    return glm::distance(e1->cTransform.value().pos, e2->cTransform.value().pos) <= 2 * m_standardRadius;
}
