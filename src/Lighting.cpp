#include "Lighting.hpp"
#include "Engine.hpp"
#include "Shader.hpp"

namespace zge 
{

    int LightSource::n_lights = 0;

    void LightSource::doRender(Engine &eng)
    {
        auto light_shader = eng_getAssetTyped("Texture Shader", zge::Shader);

        // light_shader->doUse();
        light_shader->sendUniform("light", *this);
        light_shader->sendUniform("view_position", eng.camera.position);

        auto basic_shader = eng_getAssetTyped("Basic Shader", zge::Shader);
        zge::Matrix4x4 mvp = eng.camera.getProjection() * eng.camera.getView() * getModelMatrix();
        basic_shader->doUse();
        basic_shader->sendUniform("mvp", mvp);

        model->doUse();
        model->doRender(eng);
    }

    void LightSource::doUpdate(Engine &eng)
    {

    }


} // namespace zge
