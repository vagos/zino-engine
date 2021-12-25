#include "Collider.hpp"
#include "Engine.hpp"
#include "Common.hpp"
#include "Model.hpp"
#include "Object.hpp"
#include "Texture.hpp"
#include "Skybox.hpp"
#include "Lighting.hpp"
#include "Shader.hpp"
#include "Ball.hpp"
#include "Cube.hpp"

#include <glm/ext/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <memory>

std::shared_ptr<zge::LightSource> main_light = nullptr;

/*
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
        shader_file->sendUniform("view_position", eng.camera.position);

        shader_file->sendUniform("material.ambient", zge::Vector3(1.0f, 0.5f, 0.31f));
        shader_file->sendUniform("material.diffuse", zge::Vector3(1.0f, 0.5f, 0.31f));
        shader_file->sendUniform("material.specular",zge::Vector3( 0.5f, 0.5f, 0.5f));
        shader_file->sendUniform("material.shininess", 32.0f);

        // glm::vec3 lightColor;
        // lightColor.x = sin(eng.getTime() * 2.0f);
        // lightColor.y = sin(eng.getTime() * 0.7f);
        // lightColor.z = sin(eng.getTime() * 1.3f);
        //   
        // main_light->diffuse = lightColor   * glm::vec3(0.5f); 
        // main_light->ambient = main_light->diffuse * glm::vec3(0.2f); 

        shader_file->sendUniform("light.position", main_light->position);
        shader_file->sendUniform("light.ambient", main_light->ambient);
        shader_file->sendUniform("light.diffuse", main_light->diffuse);
        shader_file->sendUniform("light.specular", main_light->specular);
        
        shader_file->sendUniform("light.Kc", 1.0f);
        shader_file->sendUniform("light.Kl", 0.09f);
        shader_file->sendUniform("light.Kq", 0.032f);
        
        glDrawArrays(GL_TRIANGLES, 0, model_file->vertices.size());
    }

};
*/

struct Tree : public zge::Object
{

    std::shared_ptr<zge::Shader> texture_shader = nullptr;
    std::shared_ptr<zge::Model> debug_model = nullptr;

    Tree(zge::Engine& eng) 
    {
        setModelMatrix(glm::scale(getModelMatrix(), zge::Vector3(0.2f)));

        model = std::static_pointer_cast<zge::Model>(eng.getAsset("Tree Model"));
        rigid_body = std::make_shared<zge::RigidBody>();
        rigid_body->mass = 1000000.0f;

        collider = std::make_shared<zge::CubeCollider>(*model, rigid_body->position);

        debug_model = eng_getAssetTyped("Sphere Model", zge::Model);
    }

    void doRender(zge::Engine &eng) override
    {
        texture_shader->doUse();
        model->doUse();

        zge::Matrix4x4 mvp = eng.camera.getProjection() * eng.camera.getView() * model_matrix;

        texture_shader->sendUniform("mvp", mvp);
        texture_shader->sendUniform("texture_sampler", model->texture->getTextureUnit());

        glDrawArrays(GL_TRIANGLES, 0, model->vertices.size());


        debug_model->doUse();

        // zge::Matrix4x4 d_mvp = eng.camera.getProjection() * eng.camera.getView() * glm::scale(zge::Matrix4x4(1), zge::Vector3(collider->radius))
        //     * model_matrix;

        zge::Matrix4x4 d_mvp = eng.camera.getProjection() * eng.camera.getView() * model_matrix;
        
        texture_shader->sendUniform("mvp", d_mvp);
        texture_shader->sendUniform("texture_sampler", model->texture->getTextureUnit());

        debug_model->doRender(eng);
    }

};

class TestingEngine : public zge::Engine
{
    std::shared_ptr<Ball> ball;

    void onCreate() override
    {
        // auto suzanne_model = std::make_shared<zge::Model>("./assets/objs/suzanne.obj");
        // auto heart_model = std::make_shared<zge::Model>("./assets/objs/heart.obj");
        auto cube_model = std::make_shared<zge::Model>("./assets/objs/cube.obj");
        auto tree_model = std::make_shared<zge::Model>("./assets/objs/tree.obj");
        auto sphere_model = std::make_shared<zge::Model>("./assets/objs/sphere.obj"); 
        std::shared_ptr<zge::Texture> moss_texture = std::make_shared<zge::Texture>("./assets/textures/trees/Mossy_Tr.bmp");

        addAsset(tree_model, "Tree Model");
        addAsset(sphere_model, "Sphere Model");
        addAsset(cube_model, "Cube Model");

        auto basic_shader = std::make_shared<zge::Shader>("./assets/shaders/basic.vert", "./assets/shaders/basic.frag"); 
        auto lighting_shader = std::make_shared<zge::Shader>("./assets/shaders/basic.vert", "./assets/shaders/lighting.frag"); // testing ligthing
        auto texture_shader = std::make_shared<zge::Shader>("./assets/shaders/texture_shader.vert", "./assets/shaders/texture_shader.frag");

        addAsset(basic_shader, "Basic Shader");
        addAsset(texture_shader, "Texture Shader");

        // cubemap stuff (skybox)
        auto skybox = std::make_shared<zge::Skybox>();
        skybox->name = std::string("Skybox");
        addObject(skybox);

        // add cube
        auto cube = std::make_shared<Cube>(*this);
        addObject(cube);

        for (int i = 0; i < 1; i++) // create trees
        {
            auto new_tree = std::make_shared<Tree>(*this);

            new_tree->model->texture = moss_texture;
            new_tree->texture_shader = texture_shader;
        
            // new_tree->setModelMatrix(glm::translate(new_tree->getModelMatrix(), 
            //             zge::Vector3(
            //                 getRandomInt(-100, 100),
            //                 -5,
            //                 getRandomInt(-100, 100)                            
            //                 )));
            
            new_tree->rigid_body->position = zge::Vector3(10.0f, 0.0f, 0.0f);

            // addObject(new_tree);
        }

        // main_light = std::make_shared<zge::LightSource>();
        // main_light->setShaderFile(lighting_shader);
        // main_light->setModelFile(cube_model);
        // addObject(main_light);

        // auto monkey = std::make_shared<Suzanne>();
        // monkey->setShaderFile(texture_shader);
        // monkey->setModelFile(suzanne_model);
        // addObject(monkey);
    }


    void doUpdate() override
    {
        if (isKeyPressed(Key(J)))
        {
            // main_light->setModelMatrix(glm::translate(zge::Matrix4x4(1), camera.position));
        }

        if (isKeyPressed(Key(T))) // throw ball 
        {
            auto new_ball = std::make_shared<Ball>(*this);

            new_ball->rigid_body->position = camera.position + 10.0f * camera.view_direction;
            new_ball->rigid_body->mass = 100.0f;
            new_ball->rigid_body->applyForce(camera.view_direction * 5000.0f);
            
            addObject(new_ball);

            // std::clog << "new ball\n";
        }
    }

    void doRender() override
    {

    }
};


int main()
{
    TestingEngine engine{};

    std::string name{"Testing"};

    if (engine.doConsturct(1024, 576, name))
        engine.doStart();
}
