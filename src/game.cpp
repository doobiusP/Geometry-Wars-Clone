#include "game.hpp"

Game::Game(const std::string &pathToConfig) : m_manager{}, m_score{0}, m_currentFrame{0}, m_lastEnemySpawnTime{0}, m_lastBulletSpawnTime{0}, m_paused{0}, m_spaceKeyPressed{0}, m_fps{0}, m_frameCount{0}, m_timeAccumulated{0}, m_currentTime{0}
{
    std::ifstream config(pathToConfig);
    if (!config)
    {
        std::cerr << "Failed to open config file: " << pathToConfig << std::endl;
        std::exit(EXIT_FAILURE);
    }

    // Game Config
    config >> m_gConfig.g_windowWidth >> m_gConfig.g_windowHeight >>
        m_gConfig.g_standardSpeed >> m_gConfig.g_standardRadius >> m_gConfig.g_vertexMin >> m_gConfig.g_vertexMax >>
        m_gConfig.g_backgroundColor.r >> m_gConfig.g_backgroundColor.g >> m_gConfig.g_backgroundColor.b >> m_gConfig.g_backgroundColor.a;

    m_gConfig.g_backgroundColor /= 255.0f;

    // Small Enemy Config
    config >> m_eConfig.g_smallEnemySpeedFactor >> m_eConfig.g_smallEnemyRotationFactor >> m_eConfig.g_smallEnemyRadiusFactor >>
        m_eConfig.g_smallEnemyLifespan >> m_eConfig.g_smallEnemyOutlineColor.r >>
        m_eConfig.g_smallEnemyOutlineColor.g >> m_eConfig.g_smallEnemyOutlineColor.b >> m_eConfig.g_smallEnemyOutlineColor.a;

    m_eConfig.g_smallEnemyOutlineColor /= 255.0f;

    // Big Enemy Config
    config >> m_eConfig.g_bigEnemySpeedFactor >> m_eConfig.g_bigEnemyRotationFactor >> m_eConfig.g_bigEnemySpawnRate >>
        m_eConfig.g_bigEnemyOutlineColor.r >> m_eConfig.g_bigEnemyOutlineColor.g >> m_eConfig.g_bigEnemyOutlineColor.b >> m_eConfig.g_bigEnemyOutlineColor.a;

    m_eConfig.g_bigEnemyOutlineColor /= 255.0f;

    // Bullet Config
    config >> m_bConfig.g_bulletSpeedFactor >> m_bConfig.g_bulletRotationFactor >> m_bConfig.g_bulletRadiusFactor >>
        m_bConfig.g_bulletLifespan >> m_bConfig.g_bulletSpawnLimit >> m_bConfig.g_bulletColor.r >> m_bConfig.g_bulletColor.g >> m_bConfig.g_bulletColor.b >> m_bConfig.g_bulletColor.a >>
        m_bConfig.g_bulletOutlineColor.r >> m_bConfig.g_bulletOutlineColor.g >> m_bConfig.g_bulletOutlineColor.b >> m_bConfig.g_bulletOutlineColor.a;

    m_bConfig.g_bulletColor /= 255.0f;
    m_bConfig.g_bulletOutlineColor /= 255.0f;

    // Player Config
    config >> m_pConfig.g_playerSpeedFactor >> m_pConfig.g_playerRotationFactor >>
        m_pConfig.g_playerColor.r >> m_pConfig.g_playerColor.g >> m_pConfig.g_playerColor.b >> m_pConfig.g_playerColor.a >>
        m_pConfig.g_playerOutlineColor.r >> m_pConfig.g_playerOutlineColor.g >> m_pConfig.g_playerOutlineColor.b >> m_pConfig.g_playerOutlineColor.a;

    m_pConfig.g_playerColor /= 255.0f;
    m_pConfig.g_playerOutlineColor /= 255.0f;

    if (config.fail())
    {
        std::cerr << "Error reading config file: " << pathToConfig << std::endl;
        std::exit(EXIT_FAILURE);
    }
    config.close();

    // ------------------------------------ Construction ------------------------------------ //

    m_rg = RandomGenerator(m_gConfig.g_windowWidth, m_gConfig.g_windowHeight, m_gConfig.g_standardRadius, m_gConfig.g_vertexMin, m_gConfig.g_vertexMax);
    m_window = Window(m_gConfig.g_windowWidth, m_gConfig.g_windowHeight);

    std::string pathToVert = "shaders/polyplace.glsl";
    std::string pathToFrag = "shaders/polyfill.glsl";
    std::string pathToOutlineFrag = "shaders/polyoutline.glsl";
    m_renderer = std::make_unique<Renderer>(m_gConfig.g_vertexMin, m_gConfig.g_vertexMax, m_gConfig.g_standardRadius, m_gConfig.g_windowWidth, m_gConfig.g_windowHeight, pathToVert, pathToFrag, pathToOutlineFrag);

    spawnPlayer();

    glfwSetWindowUserPointer(m_window.window, this);
    glfwSetKeyCallback(m_window.window, keyCallback);
}

void Game::run()
{
    float lastTime = static_cast<float>(glfwGetTime());
    float deltaTime;

    while (!glfwWindowShouldClose(m_window.window))
    {
        m_currentTime = static_cast<float>(glfwGetTime());
        deltaTime = m_currentTime - lastTime;
        lastTime = m_currentTime;

        m_manager.update();
        if (!m_paused)
        {
            sUserInput();
            sMovement(deltaTime);
            sCollision();
            sEnemySpawner();
            sLifespan(deltaTime);
        }
        sRender();
        updateFPS(deltaTime);

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
    auto &enemies = m_manager.getEntities(ENTITY_TYPE::ENEMY);
    auto &bullets = m_manager.getEntities(ENTITY_TYPE::BULLET);

    for (size_t enemyID : enemies)
    {
        Entity *enemy = m_manager.getEntityByID(enemyID);
        if (enemy->cLifespan.has_value()) // Small Enemy
        {
            enemy->cTransform.value().pos = enemy->cTransform.value().pos + (m_eConfig.g_smallEnemySpeedFactor * m_gConfig.g_standardSpeed * deltaTime * enemy->cTransform.value().vel);
            enemy->cTransform.value().angle += m_eConfig.g_smallEnemyRotationFactor * deltaTime;
        }
        else // Big Enemy
        {
            enemy->cTransform.value().pos = enemy->cTransform.value().pos + (m_eConfig.g_bigEnemySpeedFactor * m_gConfig.g_standardSpeed * deltaTime * enemy->cTransform.value().vel);
            enemy->cTransform.value().angle += m_eConfig.g_bigEnemyRotationFactor * deltaTime;
        }
    }

    for (size_t bulletID : bullets)
    {
        Entity *bullet = m_manager.getEntityByID(bulletID);
        bullet->cTransform.value().pos = bullet->cTransform.value().pos + (m_bConfig.g_bulletSpeedFactor * m_gConfig.g_standardSpeed * deltaTime * bullet->cTransform.value().vel);
        bullet->cTransform.value().angle += m_bConfig.g_bulletRotationFactor * deltaTime;
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

    if (playerMov.vel != glm::vec2(0.0f, 0.0f))
        playerMov.vel = glm::normalize(playerMov.vel);

    playerMov.pos = playerMov.pos + (m_pConfig.g_playerSpeedFactor * m_gConfig.g_standardSpeed * deltaTime * playerMov.vel);
    playerMov.angle += m_pConfig.g_playerRotationFactor * deltaTime;
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
    if (m_spaceKeyPressed && m_currentTime - m_lastBulletSpawnTime >= m_bConfig.g_bulletSpawnLimit)
    {
        double xpos, ypos;
        glfwGetCursorPos(m_window.window, &xpos, &ypos);
        spawnBullet(glm::vec2(static_cast<float>(xpos), static_cast<float>(ypos)));

        m_lastBulletSpawnTime = m_currentTime;
        m_spaceKeyPressed = 0;
    }
}

void Game::sLifespan(float deltaTime)
{
    auto &enemies = m_manager.getEntities(ENTITY_TYPE::ENEMY);
    auto &bullets = m_manager.getEntities(ENTITY_TYPE::BULLET);

    for (auto enemy : enemies)
    {
        Entity *enemyE = m_manager.getEntityByID(enemy);
        if (enemyE->cLifespan.has_value())
        {
            CLifespan &lf = enemyE->cLifespan.value();

            lf.remaining -= deltaTime;
            if (lf.remaining <= 0)
            {
                enemyE->destroy();
            }
            else
            {
                CShape &sh = enemyE->cShape.value();
                float frac = lf.remaining / lf.total;
                sh.color.a = frac;
                sh.outlineColor.a = frac;
            }
        }
    }

    for (auto bullet : bullets)
    {
        Entity *bulletE = m_manager.getEntityByID(bullet);
        CLifespan &lf = bulletE->cLifespan.value();

        lf.remaining -= deltaTime;
        if (lf.remaining <= 0)
        {
            bulletE->destroy();
        }
        else
        {
            CShape &sh = bulletE->cShape.value();
            float frac = lf.remaining / lf.total;
            sh.color.a = frac;
            sh.outlineColor.a = frac;
        }
    }
}

void Game::sRender()
{
    glm::vec4 &bg = m_gConfig.g_backgroundColor;
    glClearColor(bg.r, bg.g, bg.b, bg.a);
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

void Game::sEnemySpawner()
{
    if (m_currentTime - m_lastEnemySpawnTime >= m_eConfig.g_bigEnemySpawnRate)
    {
        spawnEnemy();
        m_lastEnemySpawnTime = m_currentTime;
    }

    for (auto enemyID : m_enemiesDestroyedThisFrame)
    {
        spawnSmallEnemies(enemyID);
    }

    m_enemiesDestroyedThisFrame.clear();
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
            playerMov.pos = glm::vec2(m_gConfig.g_windowWidth / 2, m_gConfig.g_windowHeight / 2);
        }

        for (auto bulletID : bullets)
        {
            if (checkCollision(bulletID, enemyID))
            {
                m_manager.getEntityByID(enemyID)->destroy();
                m_manager.getEntityByID(bulletID)->destroy();

                if (!m_manager.getEntityByID(enemyID)->cLifespan.has_value()) // Only big enemies can spawn small enemies
                    m_enemiesDestroyedThisFrame.push_back(enemyID);
            }
        }
    }

    for (auto bulletID : bullets)
    {
        glm::vec2 &bulletPos = m_manager.getEntityByID(bulletID)->cTransform.value().pos;
        float collRadius = m_manager.getEntityByID(bulletID)->cCollision.value().radius;

        if (bulletPos.x - collRadius <= 0.0f || bulletPos.x + collRadius >= m_gConfig.g_windowWidth || bulletPos.y - collRadius <= 0.0f || bulletPos.y + collRadius >= m_gConfig.g_windowHeight)
        {
            m_manager.getEntityByID(bulletID)->destroy();
        }
    }
    for (auto enemyID : enemies)
    {
        Entity *enemy = m_manager.getEntityByID(enemyID);
        float collRadius = enemy->cCollision.value().radius;
        CTransform &enemyMov = enemy->cTransform.value();

        if (enemyMov.pos.x - collRadius <= 0.0f || enemyMov.pos.x + collRadius >= m_gConfig.g_windowWidth)
        {
            enemyMov.vel.x *= -1;
        }
        if (enemyMov.pos.y - collRadius <= 0.0f || enemyMov.pos.y + collRadius >= m_gConfig.g_windowHeight)
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
    if (playerMov.pos.x + collRadius >= m_gConfig.g_windowWidth)
    {
        playerMov.pos.x = m_gConfig.g_windowWidth - collRadius;
    }
    if (playerMov.pos.y - collRadius <= 0.0f)
    {
        playerMov.pos.y = collRadius;
    }
    if (playerMov.pos.y + collRadius >= m_gConfig.g_windowHeight)
    {
        playerMov.pos.y = m_gConfig.g_windowHeight - collRadius;
    }
}

void Game::spawnPlayer()
{
    m_playerID = m_manager.createEntity(ENTITY_TYPE::PLAYER);
    Entity *player = m_manager.getEntityByID(m_playerID);

    player->addTransform(glm::vec2(m_gConfig.g_windowWidth / 2, m_gConfig.g_windowHeight / 2), glm::vec2(0.0f, 0.0f), 0.0f);
    player->addShape(m_gConfig.g_vertexMax, 1.0f, m_pConfig.g_playerColor, m_pConfig.g_playerOutlineColor);
    player->addCollision(m_gConfig.g_standardRadius);
    player->addInput();
}

void Game::spawnEnemy()
{
    size_t enemyID = m_manager.createEntity(ENTITY_TYPE::ENEMY);
    Entity *enemy = m_manager.getEntityByID(enemyID);
    glm::vec2 currPos = m_rg.generateCoordinates();

    enemy->addTransform(currPos, glm::normalize(currPos - m_rg.generateCoordinates()), 0.0f);
    enemy->addShape(m_rg.generateVertex(), 1.0f, m_rg.generateColor(), m_eConfig.g_bigEnemyOutlineColor);
    enemy->addCollision(m_gConfig.g_standardRadius);
}

void Game::spawnSmallEnemies(size_t enemy_id)
{
    Entity *baseEnemy = m_manager.getEntityByID(enemy_id);

    int baseVCount = baseEnemy->cShape.value().numSides;
    glm::vec4 baseColor = baseEnemy->cShape.value().color;
    glm::vec2 basePos = baseEnemy->cTransform.value().pos;
    float baseR = baseEnemy->cCollision.value().radius;

    const float PI = glm::pi<float>();

    for (int i = 0; i < baseVCount; ++i)
    {
        size_t smallEnemyID = m_manager.createEntity(ENTITY_TYPE::ENEMY);
        Entity *smallEnemy = m_manager.getEntityByID(smallEnemyID);

        float angle = (2.0f * PI / baseVCount) * i;
        glm::vec2 vel = glm::vec2(cos(angle), sin(angle));
        glm::vec2 pos = basePos + ((1.0f + m_eConfig.g_smallEnemyRadiusFactor) * baseR * vel);

        smallEnemy->addTransform(pos, vel, 0.0f);
        smallEnemy->addShape(baseVCount, m_eConfig.g_smallEnemyRadiusFactor, baseColor, m_eConfig.g_smallEnemyOutlineColor);
        smallEnemy->addCollision(m_eConfig.g_smallEnemyRadiusFactor * m_gConfig.g_standardRadius);
        smallEnemy->addLifeSpan(m_eConfig.g_smallEnemyLifespan);
    }
}

void Game::spawnBullet(const glm::vec2 &mousePos)
{
    size_t bulletID = m_manager.createEntity(ENTITY_TYPE::BULLET);
    Entity *bullet = m_manager.getEntityByID(bulletID);

    Entity *player = m_manager.getEntityByID(m_playerID);
    glm::vec2 bulletVel = glm::normalize(mousePos - player->cTransform.value().pos);
    float r = player->cCollision.value().radius;

    glm::vec2 correctPos = player->cTransform.value().pos + ((1.0f + m_bConfig.g_bulletRadiusFactor) * r * bulletVel);

    bullet->addTransform(correctPos, bulletVel, 0.0f);
    bullet->addShape(m_gConfig.g_vertexMax, m_bConfig.g_bulletRadiusFactor, m_bConfig.g_bulletColor, m_bConfig.g_bulletOutlineColor);
    bullet->addCollision(m_bConfig.g_bulletRadiusFactor * m_gConfig.g_standardRadius);
    bullet->addLifeSpan(m_bConfig.g_bulletLifespan);
}

bool Game::checkCollision(size_t id1, size_t id2)
{
    Entity *e1 = m_manager.getEntityByID(id1);
    Entity *e2 = m_manager.getEntityByID(id2);

    return glm::distance(e1->cTransform.value().pos, e2->cTransform.value().pos) <= e1->cCollision.value().radius + e2->cCollision.value().radius;
}

void Game::updateFPS(float deltaTime)
{
    ++m_frameCount;
    m_timeAccumulated += deltaTime;

    if (m_timeAccumulated >= 1.0f)
    {
        m_fps = m_frameCount / m_timeAccumulated;

        std::cout << "FPS: " << m_fps << '\n';

        m_frameCount = 0;
        m_timeAccumulated = 0.0f;
    }
}

void Game::keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    Game *game = static_cast<Game *>(glfwGetWindowUserPointer(window));
    if (game)
    {
        game->handleKeyPress(key, action);
    }
}

void Game::handleKeyPress(int key, int action)
{
    if (key == GLFW_KEY_SPACE)
    {
        if (action == GLFW_PRESS)
        {
            m_spaceKeyPressed = true;
        }
        else if (action == GLFW_RELEASE)
        {
            m_spaceKeyPressed = false;
        }
    }
    if (key == GLFW_KEY_P)
    {
        if (action == GLFW_PRESS)
        {
            togglePause();
        }
    }
}
