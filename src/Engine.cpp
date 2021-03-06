#include <cstdlib>
#include <memory>
#include <stdexcept>
#include <fstream>

#include "Engine.hpp"
#include "Model.hpp"
#include "Object.hpp"
#include "Common.hpp"

#define KEY_COOLDOWN 0.5f

namespace zge
{

void zge::Engine::doStart()
{
    running = true;    

    onCreate();

    while ( running )
    {
       _doUpdate(); 
       _doRender();
    }
}

void zge::Engine::_doRender()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    doRender();
    
    for (auto& o : Object::objects)
    {
        o->doRender(*this);
    }

    if (glfwWindowShouldClose(window)) doExit();

    glfwSwapBuffers(window);
    glfwPollEvents();
}

void zge::Engine::_doUpdate()
{
    float current_time = glfwGetTime(); // Calculate elpapsed time.
    elapsed_time =  current_time - previous_time;
    previous_time = current_time;
    key_cooldown = key_cooldown < 0 ? KEY_COOLDOWN : key_cooldown - elapsed_time; 

    doUpdate();

    camera.doUpdate(*this);
    
    // add objects that should be spawned
    Object::objects.reserve(Object::objects.size() + std::distance(objs_to_spawn.begin(), objs_to_spawn.end()));
    Object::objects.insert(Object::objects.end(), objs_to_spawn.begin(), objs_to_spawn.end());
    objs_to_spawn.clear();


    for (auto& o : Object::objects)
    {
        if (!o) continue;
        o->resetModelMatrix();
        o->doUpdate(*this);
    }

    Object::removeNullObjects();
}

bool zge::Engine::doConsturct(int w, int h, std::string &app_name)
{
    if (!glfwInit()) throw std::runtime_error("Backend failed to initialize!\n");
    
    // Standard GLFW window hints.
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create a window
    window = glfwCreateWindow(w, h, app_name.c_str(), NULL, NULL);
    if (window == nullptr) throw std::runtime_error("Failed to open Window!");
    glfwMakeContextCurrent(window);

    // GLEW initialize
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) throw std::runtime_error("Failed to initialize GL bindings!");

    // Set the mouse at the center of the screen
    glfwPollEvents();
    glfwSetCursorPos(window, w / 2, h / 2);

    // Some GLFW options
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    // Hide the mouse and enable unlimited movement
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Some GLFW callbacks
    glfwSetWindowSizeCallback(window, [](Window* window, int w, int h) 
            {
            glViewport(0, 0, w, h);
            });

    // OpenGL settings
	glViewport(0, 0, w, h);
    // Gray background color
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);
    // Enable blending for transparency
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // Cull triangles which normal is not towards the camera
    glEnable(GL_CULL_FACE);

    // logGLParameters();

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    Engine::width = w; Engine::height = h;

    // random number generator
    srand(time(NULL));

    return true;
}

zge::Vector2 zge::Engine::getCursorPosition()
{
    double x, y;
    glfwGetCursorPos(window, &x, &y);
    return Vector2(x, y);
}

zge::Vector2 zge::Engine::getWindowSize()
{
    int w, h;
    glfwGetWindowSize(window, &w, &h);
    return Vector2(w, h);
}

void zge::Engine::setCursorPosition(Vector2&& pos)
{
    glfwSetCursorPos(window, pos.x, pos.y);
}

void zge::Engine::addObject(std::shared_ptr<Object> obj)
{
    objs_to_spawn.push_back(obj);
}

void Engine::addObject(std::shared_ptr<Object> obj, std::string obj_name)
{
    addObject(obj);
    objects[obj_name] = obj;
}

bool zge::Engine::isKeyPressed(int key)
{
    if (key_cooldown < 0) return glfwGetKey(window, key) == GLFW_PRESS;
    return false;
}

bool zge::Engine::isKeyHeld(int key)
{
    return glfwGetKey(window, key) == GLFW_PRESS;
}


// void zge::Engine::loadShaders(std::string v_s_path, std::string f_s_path)

int zge::Engine::getRandomInt(int a, int b)
{
    return a + ( random() % (b - a) );
}

float Engine::getRandomFloat()
{
    return float(random()) / float(RAND_MAX);
}

float Engine::getRandomFloat(float a, float b)
{
    return getRandomFloat() * (b - a) + a;
}

void Engine::addAsset(std::shared_ptr<Asset> asset, std::string asset_name)
{
    assets[asset_name] = asset;
}

std::shared_ptr<Asset> Engine::getAsset(std::string asset_name)
{
    auto asset = assets[asset_name];

    if (!asset) std::runtime_error("Asset not found!");

    return asset;
}

std::shared_ptr<Object> Engine::getObject(std::string object_name)
{
    auto object = objects[object_name];
    if (!object) std::runtime_error("Object not found!");
    return object;
}


int Engine::width = 0;
int Engine::height = 0;

}
