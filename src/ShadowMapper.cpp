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
            
        zge::Matrix4x4 vp = main_light->getProjection() * main_light->getView();
        depth_shader->sendUniform("vp", vp);

        // Render the scene
        for (auto& o : Object::objects)
        {
            o->doBasicRender(eng, *depth_shader);
        }

        f_b.doUnuse();
    
#ifdef DEBUG

        auto debug_shader = eng_getAssetTyped("Debug Shader", Shader);

        debug_shader->doUse();
        debug_shader->sendUniform("depthmap_sampler", d_t);
        
        quad.doRender(eng);
#endif
    }

}
