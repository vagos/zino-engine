#include "Lighting.hpp"
#include "Common.hpp"
#include "Engine.hpp"
#include "Shader.hpp"

namespace zge 
{

    int LightSource::n_lights = 0;

    void LightSource::doRender(Engine &eng)
    {
        auto light_shader = eng_getAssetTyped("Texture Shader", zge::Shader);

        light_shader->doUse();
        light_shader->sendUniform("light", *this);

        // auto basic_shader = eng_getAssetTyped("Basic Shader", zge::Shader);
        // zge::Matrix4x4 mvp = eng.camera.getProjection() * eng.camera.getView() * getModelMatrix();
        // basic_shader->doUse();
        // basic_shader->sendUniform("mvp", mvp);

        // model->doUse();
        // model->doRender(eng);
    }

    void LightSource::doUpdate(Engine &eng)
    {

        // Move across z-axis
        if (eng.isKeyHeld(Key(Y))) {
            position += speed * zge::Vector3(0.0, 0.0, 1.0);
        }
        if (eng.isKeyHeld(Key(I))) {
            position -= speed * zge::Vector3(0.0, 0.0, 1.0);
        }
        // Move across x-axis
        if (eng.isKeyHeld(Key(K))) {
            position += speed * zge::Vector3(1.0, 0.0, 0.0);
        }
        if (eng.isKeyHeld(Key(H))) {
            position -= speed * zge::Vector3(1.0, 0.0, 0.0);
        }
        // Move across y-axis
        if (eng.isKeyHeld(Key(U))) {
            position += speed * zge::Vector3(0.0, 1.0, 0.0);
        }
        if (eng.isKeyHeld(Key(J))) {
            position -= speed * zge::Vector3(0.0, 1.0, 0.0);
        }

        // We have the direction of the light and the point where the light is looking at
        // We will use this information to calculate the "up" vector, 
        // just like we did with the camera

        direction = normalize(target_position - position);


        // converting direction to cylidrical coordinates
        float x = direction.x;
        float y = direction.y;
        float z = direction.z;

        // We don't need to calculate the vertical angle
        
        float horizontalAngle;
        if (z > 0.0) horizontalAngle = atan(x/z);
        else if (z < 0.0) horizontalAngle = atan(x/z) + 3.1415f;
        else horizontalAngle = 3.1415f / 2.0f;

        // Right vector
        Vector3 right(
            sin(horizontalAngle - 3.14f / 2.0f),
            0,
            cos(horizontalAngle - 3.14f / 2.0f)
        );

        // Up vector
        Vector3 up = cross(right, direction);
       
        look_at = lookAt(
            position,
            target_position,
            up 
        );

    }


} // namespace zge
