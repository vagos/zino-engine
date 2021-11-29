#include "Engine.hpp"
#include "Common.hpp"
#include "Object.hpp"
#include "Texture.hpp"
#include "Skybox.hpp"
#include "Lighting.hpp"
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

std::shared_ptr<zge::LightSource> main_light = nullptr;

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
        shader_file->sendUniform("light_position", main_light->getPosition());
        shader_file->sendUniform("view_position", eng.camera.getPosition());

        glDrawArrays(GL_TRIANGLES, 0, model_file->vertices.size());
    }

};

class PokemonEngine : public zge::Engine
{

    void onCreate() override
    {
        auto cube_model = std::make_shared<zge::Model>("./assets/objs/cube.obj");
        auto suzanne_model = std::make_shared<zge::Model>("./assets/objs/suzanne.obj");
        auto basic_shader = std::make_shared<zge::Shader>("./assets/shaders/basic.vert", "./assets/shaders/basic.frag"); // testing ligthing
        auto lighting_shader = std::make_shared<zge::Shader>("./assets/shaders/basic.vert", "./assets/shaders/lighting.frag"); 

        auto texture_shader = std::make_shared<zge::Shader>("./assets/shaders/texture_shader.vert", "./assets/shaders/texture_shader.frag");

        // cubemap stuff (skybox)
        auto skybox = std::make_shared<Skybox>();
        addObject(skybox);

        for (int i = 0; i < 10; i++)
        {
            auto myObj = std::make_shared<zge::ModeledObject>(cube_model);
            myObj->setShaderFile(basic_shader);
        
            myObj->setModelMatrix(glm::translate(myObj->getModelMatrix(), 
                        zge::Vector3(
                            getRandomInt(-10, 10),
                            getRandomInt(-10, 10),
                            getRandomInt(-10, 10)
                            )));
        
            // addObject(myObj);
        }

        auto plane_object = std::make_shared<zge::Plane>(2, 2);
        plane_object->setShaderFile(basic_shader);
        plane_object->setModelMatrix(glm::translate(plane_object->getModelMatrix(), zge::Vector3(0, 0, 0)));
        // addObject(plane_object);

        main_light = std::make_shared<zge::LightSource>();
        main_light->setShaderFile(lighting_shader);
        main_light->setModelFile(cube_model);
        addObject(main_light);

        auto other_cube = std::make_shared<Suzanne>();
        other_cube->setShaderFile(texture_shader);
        other_cube->setModelFile(suzanne_model);
        // other_cube->setModelMatrix(glm::translate(other_cube->getModelMatrix(), zge::Vector3(0, 0, 10)));
        addObject(other_cube);
    }


    void doUpdate() override
    {
        if (isKeyPressed(Key(J)))
        {
            main_light->setModelMatrix(glm::translate(zge::Matrix4x4(1), camera.getPosition()));
            // std::clog << glm::to_string(main_light->position) << '\n';
        }
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
