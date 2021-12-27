#ifndef COMMON_ZE_HPP
#define COMMON_ZE_HPP

#include <GL/eglew.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

#include <iostream>

#define GLCall(c) c

namespace zge 
{
    typedef GLFWwindow Window;
    typedef glm::vec4 Vector4;
    typedef glm::vec3 Vector3;
    typedef glm::vec2 Vector2;
    typedef glm::mat4 Matrix4x4;
    typedef glm::mat3 Matrix3x3;
}

#endif /* COMMON_ZE_HPP */
