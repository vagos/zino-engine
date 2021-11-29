#ifndef SKYBOX_HPP
#define SKYBOX_HPP

#include "Object.hpp"
#include "Texture.hpp"
#include "Common.hpp"
#include "Engine.hpp"

struct Skybox : public zge::ModeledObject // TODO Turn this into proper class.
{
    zge::Cubemap s_cubemap;

    Skybox(): s_cubemap({"./assets/textures/skybox/right.bmp",
                "./assets/textures/skybox/left.bmp",
                "./assets/textures/skybox/top.bmp",
                "./assets/textures/skybox/bottom.bmp",
                "./assets/textures/skybox/front.bmp",
                "./assets/textures/skybox/back.bmp",
                })
    {
        auto s_shader = std::make_shared<zge::Shader>("./assets/shaders/skybox.vert", "./assets/shaders/skybox.frag");
        setShaderFile(s_shader);

        auto cubeModel = std::make_shared<zge::Model>();
        std::vector<zge::Vector3> skyboxVertices{
            // positions          
            zge::Vector3(-1.0f,  1.0f, -1.0f),
            zge::Vector3(-1.0f, -1.0f, -1.0f),
            zge::Vector3( 1.0f, -1.0f, -1.0f),
            zge::Vector3( 1.0f, -1.0f, -1.0f),
            zge::Vector3( 1.0f,  1.0f, -1.0f),
            zge::Vector3(-1.0f,  1.0f, -1.0f),

            zge::Vector3(-1.0f, -1.0f,  1.0f),
            zge::Vector3(-1.0f, -1.0f, -1.0f),
            zge::Vector3(-1.0f,  1.0f, -1.0f),
            zge::Vector3(-1.0f,  1.0f, -1.0f),
            zge::Vector3(-1.0f,  1.0f,  1.0f),
            zge::Vector3(-1.0f, -1.0f,  1.0f),

            zge::Vector3( 1.0f, -1.0f, -1.0f),
            zge::Vector3( 1.0f, -1.0f,  1.0f),
            zge::Vector3( 1.0f,  1.0f,  1.0f),
            zge::Vector3( 1.0f,  1.0f,  1.0f),
            zge::Vector3( 1.0f,  1.0f, -1.0f),
            zge::Vector3( 1.0f, -1.0f, -1.0f),

            zge::Vector3(-1.0f, -1.0f,  1.0f),
            zge::Vector3(-1.0f,  1.0f,  1.0f),
            zge::Vector3( 1.0f,  1.0f,  1.0f),
            zge::Vector3( 1.0f,  1.0f,  1.0f),
            zge::Vector3( 1.0f, -1.0f,  1.0f),
            zge::Vector3(-1.0f, -1.0f,  1.0f),

            zge::Vector3(-1.0f,  1.0f, -1.0f),
            zge::Vector3( 1.0f,  1.0f, -1.0f),
            zge::Vector3( 1.0f,  1.0f,  1.0f),
            zge::Vector3( 1.0f,  1.0f,  1.0f),
            zge::Vector3(-1.0f,  1.0f,  1.0f),
            zge::Vector3(-1.0f,  1.0f, -1.0f),

            zge::Vector3(-1.0f, -1.0f, -1.0f),
            zge::Vector3(-1.0f, -1.0f,  1.0f),
            zge::Vector3( 1.0f, -1.0f, -1.0f),
            zge::Vector3( 1.0f, -1.0f, -1.0f),
            zge::Vector3(-1.0f, -1.0f,  1.0f),
            zge::Vector3( 1.0f, -1.0f,  1.0f)
        };
        cubeModel->setVertices(skyboxVertices);
        setModelFile(cubeModel);
    }

    void doRender(zge::Engine &eng) override
    {
        glDepthFunc(GL_LEQUAL);

        shader_file->doUse();
        model_file->doUse();
        s_cubemap.doUse();

        zge::Matrix4x4 mvp = eng.camera.getProjection() * zge::Matrix4x4(zge::Matrix3x3(eng.camera.getView())) * model_matrix;
        shader_file->sendUniform("mvp", mvp);

        shader_file->sendUniform("skybox", s_cubemap.getTextureUnit());

        glDrawArrays(GL_TRIANGLES, 0, model_file->vertices.size());

        glDepthMask(u_int8_t(GL_LESS)); // -Woverflow ???
    }
};


#endif /* SKYBOX_HPP */
