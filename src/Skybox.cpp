#include "Skybox.hpp"

namespace zge 
{

    Skybox::Skybox(): s_shader("./assets/shaders/skybox.vert", "./assets/shaders/skybox.frag")
    {
        model = std::make_shared<zge::Model>("./assets/objs/skybox.obj");

        model->texture = std::make_shared<Cubemap>(std::vector<std::string>{
            "./assets/textures/skybox/right.bmp",
            "./assets/textures/skybox/left.bmp",
            "./assets/textures/skybox/bottom.bmp",
            "./assets/textures/skybox/top.bmp",
            "./assets/textures/skybox/front.bmp",
            "./assets/textures/skybox/back.bmp",
       });
    }


    void Skybox::doRender(zge::Engine &eng) 
    {
        glDepthFunc(GL_LEQUAL);

        s_shader.doUse();
        model->doUse();

        zge::Matrix4x4 mvp = eng.camera.getProjection() * zge::Matrix4x4(zge::Matrix3x3(eng.camera.getView())) * model_matrix;
        // zge::Matrix4x4 mvp = eng.camera.getProjection() * eng.camera.getView() * model_matrix;

        s_shader.sendUniform("mvp", mvp);
        s_shader.sendUniform("time", eng.getTime());

        s_shader.sendUniform("skybox", model->texture->getTextureUnit());

        glDrawArrays(GL_TRIANGLES, 0, model->vertices.size());

        glDepthMask(GL_LESS); // -Woverflow ???
    }

}
