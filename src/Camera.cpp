#include "Camera.hpp"
#include "Common.hpp"
#include "Engine.hpp"

#include <glm/ext/matrix_transform.hpp>
#include <glm/geometric.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/dual_quaternion.hpp>
#include <glm/gtx/string_cast.hpp>

zge::Camera::Camera() 
{
    look_at = glm::lookAt(position, Vector3(0, 0, 0), V_UP);
}

void zge::Camera::doUpdate(Engine& eng)
{
    Vector2 mouse_position{eng.getCursorPosition()};
    Vector2 window_size{eng.getWindowSize()};

    eng.setCursorPosition(Vector2(window_size.x / 2, window_size.y / 2));

    horizontal_view_angle += glm::radians(eng.mouse_sensitivity * float(window_size.x/2.0f - mouse_position.x));
    vertical_view_angle   += glm::radians(eng.mouse_sensitivity * float(window_size.y/2.0f - mouse_position.y));

    view_direction = Vector3{
        glm::cos(vertical_view_angle) * sin(horizontal_view_angle), 
        sin(vertical_view_angle),
        cos(vertical_view_angle) * cos(horizontal_view_angle)
    };
    view_direction = glm::normalize(view_direction);

    right_to_view = glm::cross(view_direction, V_UP);
    right_to_view = glm::normalize(right_to_view);

    Vector3 up_to_view{glm::cross(right_to_view, view_direction)};
    up_to_view = glm::normalize(up_to_view);

    /*

    if (eng.isKeyHeld(Key(W))) position += glm::normalize(view_direction) * eng.getElapsedTime() * speed;
    if (eng.isKeyHeld(Key(S))) position -= glm::normalize(view_direction) * eng.getElapsedTime() * speed;
    if (eng.isKeyHeld(Key(D))) position += glm::normalize(right_to_view)  * eng.getElapsedTime() * speed;
    if (eng.isKeyHeld(Key(A))) position -= glm::normalize(right_to_view)  * eng.getElapsedTime() * speed;

    if (eng.isKeyHeld(Key(Q))) position += Vector3(0, 1, 0) * eng.getElapsedTime() * speed;
    if (eng.isKeyHeld(Key(E))) position -= Vector3(0, 1, 0) * eng.getElapsedTime() * speed;

    // position.y = 0; // player can't fly
    
    */

    projection = glm::perspective(glm::radians(fov), window_size.x / window_size.y, 0.1f, 10000.0f);

    look_at = glm::lookAt(
            position,
            position + view_direction,
            up_to_view
            );
}
