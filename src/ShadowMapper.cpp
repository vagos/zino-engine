#include "ShadowMapper.hpp"

namespace zge 
{

    Shadowmapper::Shadowmapper()
    {
       f_b.doUse(); 
       f_b.attachTexture(d_t); 
    }

    void Shadowmapper::doRender(Engine &eng)
    {
        glViewport(0, 0, Engine::width, Engine::height);
        
        f_b.doUse();

        glClear(GL_DEPTH_BUFFER_BIT);

        auto depth_shader = eng_getAssetTyped("Depth Shader", Shader);

        depth_shader->doUse();

        // Draw the scene
        for (auto& o : Object::objects)
        {
            o->model->doUse();
            zge::Matrix4x4 mvp = eng.camera.getProjection() * eng.camera.getView() * o->getModelMatrix();
            depth_shader->sendUniform("mvp", mvp);
        }

        f_b.doUnuse();
    }

}
