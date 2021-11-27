#include "Engine.hpp"
#include "Object.hpp"
#include "Texture.hpp"
#include "Common.hpp"

struct Suzanne : public zge::ModeledObject
{
    zge::Texture texture;

    Suzanne(): texture("./assets/textures/fiery.bmp")
    {
        
    }

    void doRender(zge::Engine &eng) override
    {

        shader_file->doUse();
        model_file->doUse();
        texture.doUse();

        zge::Matrix4x4 mvp = eng.camera.getProjection() * eng.camera.getView() * model_matrix;

        shader_file->sendUniform("mvp", mvp);
        shader_file->sendUniform("texture_sampler", texture.getTextureUnit());

        glDrawArrays(GL_TRIANGLES, 0, model_file->vertices.size());
    }

};

struct Skybox : public zge::ModeledObject
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
        s_shader->createUniform("skybox");
        s_shader->createUniform("mvp");
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
        glDepthMask(GL_FALSE);

        shader_file->doUse();
        model_file->doUse();
        s_cubemap.doUse();

        zge::Matrix4x4 mvp = eng.camera.getProjection() * zge::Matrix4x4(zge::Matrix3x3(eng.camera.getView())) * model_matrix;

        shader_file->sendUniform("mvp", mvp);

        shader_file->sendUniform("skybox", s_cubemap.getTextureUnit());

        glDrawArrays(GL_TRIANGLES, 0, model_file->vertices.size());

        glDepthMask(GL_TRUE);
    }

};

class PokemonEngine : public zge::Engine
{
    void onCreate() override
    {
        auto heart_model = std::make_shared<zge::Model>("./assets/objs/suzanne.obj");
        auto basic_shader = std::make_shared<zge::Shader>("./assets/shaders/basic.vert", "./assets/shaders/basic.frag");
        auto basic_shader_b = std::make_shared<zge::Shader>("./assets/shaders/basic.vert", "./assets/shaders/basic_b.frag");

        auto texture_shader = std::make_shared<zge::Shader>("./assets/shaders/texture_shader.vert", "./assets/shaders/texture_shader.frag");

        // cubemap stuff (skybox)
        auto skybox = std::make_shared<Skybox>();
        addObject(skybox);

        for (int i = 0; i < 10; i++)
        {
            auto myObj = std::make_shared<zge::ModeledObject>(heart_model);
            myObj->setShaderFile(basic_shader);
        
            myObj->setModelMatrix(glm::translate(myObj->getModelMatrix(), 
                        zge::Vector3(
                            getRandomInt(-10, 10),
                            getRandomInt(-10, 10),
                            getRandomInt(-10, 10)
                            )));
        
            addObject(myObj);
        }

        auto plane_object = std::make_shared<zge::Plane>(2, 2);
        plane_object->setShaderFile(basic_shader_b);
        
        plane_object->setModelMatrix(glm::translate(plane_object->getModelMatrix(),
                    zge::Vector3(0, 0, 0)));

        // addObject(plane_object);

        auto suzanneObject = std::make_shared<Suzanne>();
        suzanneObject->setShaderFile(texture_shader);
        suzanneObject->setModelFile(heart_model);
        addObject(suzanneObject);

        basic_shader->createUniform("mvp");
        texture_shader->createUniform("texture_sampler");
    }

    void doUpdate() override
    {

    }

    void doRender() override
    {

    }
};

int main()
{
    PokemonEngine engine{};

    std::string name{"Pokemon Game"};

    if (engine.doConsturct(1024, 576, name))
        engine.doStart();
}
