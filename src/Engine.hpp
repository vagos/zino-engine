#ifndef ENGINE_HPP
#define ENGINE_HPP

#include <string>
#include <memory>
#include <iostream>
#include <vector>

#include "Common.hpp"
#include "Camera.hpp"

class Window_c // Wrapper for a Window backend. (Not implemented)
{

};

#define Key(k_n) GLFW_KEY_##k_n

namespace zge 
{
    class Object;
    
    const float PI = glm::pi<float>();
    const Vector3 V_UP = Vector3(0, 1, 0);

    class Engine 
    {

    public:
        virtual void onCreate() = 0;
        virtual void doUpdate() = 0;
        virtual void doRender() = 0;

        void doStart();
        bool doConsturct(int w, int h, std::string& app_name);

        void addObject(std::shared_ptr<Object> obj);

        void doExit() { running = false; }

        float getElapsedTime() { return elapsed_time; }

        Vector2 getWindowSize();
        Vector2 getCursorPosition();
        void setCursorPosition(Vector2&& pos);
        bool isKeyPressed(int key);

        int getRandomInt(int a, int b);

    public:

        void _doRender();
        void _doUpdate();

        Camera camera;

        bool running;

        float previous_time;
        float elapsed_time;
    
        const float mouse_sensitivity = 0.01f;

        Window* window;

        // GLuint basic_shader_program_id; 
        GLuint mvp_uniform_location;

        friend Object;

    };

};

#endif /* ENGINE_HPP */
