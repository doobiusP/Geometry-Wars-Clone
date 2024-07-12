#include "game.hpp"

Game::Game(const std::string &pathToConfig) : m_manager{}, m_score{0}, m_currentFrame{0}, m_lastEnemySpawnTime{0}, m_paused{0}, m_running{1}
{
    std::ifstream config(pathToConfig);
    if (!config)
    {
        std::cerr << "Failed to open config file: " << pathToConfig << std::endl;
        std::exit(EXIT_FAILURE);
    }
    int vMin, vMax;
    float radius, width, height;

    config >> vMin >> vMax >> radius >> width >> height;

    if (config.fail())
    {
        std::cerr << "Error reading config file: " << pathToConfig << std::endl;
        std::exit(EXIT_FAILURE);
    }

    m_vMin = vMin;
    m_vMax = vMax;

    std::random_device rd;
    m_gen = std::mt19937(rd());
    m_rng = std::uniform_int_distribution<>(m_vMin, m_vMax);

    m_window = Window(width, height);

    std::string pathToVert = "shaders/polyplace.glsl";
    std::string pathToFrag = "shaders/polyfill.glsl";
    std::string pathToOutlineFrag = "shaders/polyoutline.glsl";
    m_renderer = new Renderer(vMin, vMax, radius, width, height, pathToVert, pathToFrag, pathToOutlineFrag);

    config.close();
}

Game::~Game()
{
    delete m_renderer;
}

void Game::run()
{

    int randVert = m_rng(m_gen);
    double lastTime = glfwGetTime();
    double currTime;

    while (!glfwWindowShouldClose(m_window.window))
    {
        currTime = glfwGetTime();
        glClearColor(0.2f, 0.3f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        if (currTime - lastTime >= 2)
        {
            lastTime = currTime;
            randVert = m_rng(m_gen);
        }

        m_renderer->Draw(3, glm::vec3(0.2f, 0.5f, 0.3f), glm::vec2(300, 100), -50 * glm::radians(currTime));
        m_renderer->Draw(randVert, glm::vec3(0.1f, 0.6f, 0.2f), glm::vec2(300, 120), 200 * glm::radians(currTime));
        glfwSwapBuffers(m_window.window);

        glfwPollEvents();
    }

    glfwTerminate();
}
