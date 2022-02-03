#include "ShadowMapper.hpp"
#include "Engine.hpp"
#include "Lighting.hpp"

namespace zge 
{

    Shadowmapper::Shadowmapper()
    {
       f_b.attachTexture(d_t); 
    }

    void Shadowmapper::doRender(Engine &eng)
    {
        glViewport(0, 0, Engine::width, Engine::height);
        f_b.doUse();

        glClear(GL_DEPTH_BUFFER_BIT);

        auto depth_shader = eng_getAssetTyped("Depth Shader", Shader);
        depth_shader->doUse();

        auto main_light = eng_getObjectTyped("Main Light", LightSource);

        // Render the scene
        for (auto& o : Object::objects)
        {
            if (!o->model) continue; 

            zge::Matrix4x4 mvp = main_light->getProjection() * main_light->getView() * o->getModelMatrix();
            depth_shader->sendUniform("mvp", mvp);

            o->model->doUse();
            o->model->doRender(eng);
            // o->doRender(eng);
        }

        f_b.doUnuse();

        auto debug_shader = eng_getAssetTyped("Debug Shader", Shader);

        debug_shader->doUse();
        debug_shader->sendUniform("depthmap_sampler", d_t);
        
        quad.doRender(eng);
    }

}
