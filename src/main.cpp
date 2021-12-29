#include "Collider.hpp"
#include "Engine.hpp"
#include "Common.hpp"
#include "Model.hpp"
#include "Object.hpp"
#include "Texture.hpp"
#include "Material.hpp"
#include "Skybox.hpp"
#include "Lighting.hpp"
#include "Shader.hpp"
#include "Ball.hpp"
#include "Cube.hpp"
#include "ShadowMapper.hpp"

#include <glm/ext/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <memory>

std::shared_ptr<zge::LightSource> main_light = nullptr;

struct Suzanne : public zge::Object
{
    zge::Texture texture;

    Suzanne(): texture("./assets/textures/fiery.bmp")
    {
        
    }

    void doRender(zge::Engine &eng) override
    {
        auto shader_file = eng_getAssetTyped("Texture Shader", zge::Shader);
        shader_file->doUse();
        model->doUse();
        texture.doUse();

        zge::Matrix4x4 mvp = eng.camera.getProjection() * eng.camera.getView() * getModelMatrix();

        shader_file->sendUniform("mvp", mvp);
        shader_file->sendUniform("texture_sampler", texture);
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
        
        model->doRender(eng);
    }

};

struct Tree : public zge::Object
{

    std::shared_ptr<zge::Shader> texture_shader = nullptr;
    std::shared_ptr<zge::Model> debug_model = nullptr;
    zge::Material material;

    Tree(zge::Engine& eng): material(zge::Vector3(1.0f, 0.5, 0.31f), 
            zge::Vector3(1.0f, 0.5f, 0.31f), zge::Vector3(0.5f, 0.5f, 0.5f), 32.0f)
    {
        model = std::static_pointer_cast<zge::Model>(eng.getAsset("Tree Model"));
        rigid_body = std::make_shared<zge::RigidBody>();

        rigid_body->setPosition(zge::Vector3(10, 10, 10));

        collider = std::make_shared<zge::CubeCollider>(*model, rigid_body->position);

        debug_model = eng_getAssetTyped("Sphere Model", zge::Model);

        // setModelMatrix(glm::scale(getModelMatrix(), zge::Vector3(0.2f)));

        applyTransofrmation(glm::scale(zge::Matrix4x4(1), zge::Vector3(0.2)));
    }

    void doUpdate(zge::Engine& eng) override
    {
        setModelMatrix(glm::translate(zge::Matrix4x4(1), rigid_body->position));
    }

    void doRender(zge::Engine &eng) override
    {
        texture_shader->doUse();
        model->doUse();

        zge::Matrix4x4 mvp = eng.camera.getProjection() * eng.camera.getView() * getModelMatrix();

        texture_shader->sendUniform("mvp", mvp);
        texture_shader->sendUniform("m", getModelMatrix());
        texture_shader->sendUniform("texture_sampler", *model->texture);
        texture_shader->sendUniform("material", material);

        model->doRender(eng);

        debug_model->doUse();

        // zge::Matrix4x4 d_mvp = eng.camera.getProjection() * eng.camera.getView() * glm::scale(zge::Matrix4x4(1), zge::Vector3(collider->radius))
        //     * model_matrix;

        zge::Matrix4x4 d_mvp = eng.camera.getProjection() * eng.camera.getView() * getModelMatrix();
        
        texture_shader->sendUniform("mvp", d_mvp);
        texture_shader->sendUniform("texture_sampler", *model->texture);

        debug_model->doRender(eng);
    }

};

class TestingEngine : public zge::Engine
{
    std::shared_ptr<Ball> ball;

    void onCreate() override
    {
        auto suzanne_model = std::make_shared<zge::Model>("./assets/objs/suzanne.obj");
        // auto heart_model = std::make_shared<zge::Model>("./assets/objs/heart.obj");
        auto cube_model = std::make_shared<zge::Model>("./assets/objs/cube.obj");
        auto tree_model = std::make_shared<zge::Model>("./assets/objs/tree.obj");
        auto sphere_model = std::make_shared<zge::Model>("./assets/objs/sphere.obj"); 
        std::shared_ptr<zge::Texture> moss_texture = std::make_shared<zge::Texture>("./assets/textures/trees/Mossy_Tr.bmp");
        auto grass_texture = std::make_shared<zge::Texture>("./assets/textures/grass/grass_o.bmp");

        addAsset(tree_model, "Tree Model");
        addAsset(sphere_model, "Sphere Model");
        addAsset(cube_model, "Cube Model");

        auto basic_shader = std::make_shared<zge::Shader>("./assets/shaders/basic.vert", "./assets/shaders/basic.frag"); 
        auto lighting_shader = std::make_shared<zge::Shader>("./assets/shaders/basic.vert", "./assets/shaders/lighting.frag"); // testing ligthing
        auto texture_shader = std::make_shared<zge::Shader>("./assets/shaders/texture.vert", "./assets/shaders/texture.frag");
        auto depth_shader = std::make_shared<zge::Shader>("./assets/shaders/depth.vert", "./assets/shaders/depth.frag");
        auto debug_shader = std::make_shared<zge::Shader>("./assets/shaders/debug.vert", "./assets/shaders/debug.frag");

        addAsset(basic_shader, "Basic Shader");
        addAsset(texture_shader, "Texture Shader");
        addAsset(depth_shader, "Depth Shader");
        addAsset(debug_shader, "Debug Shader");

        addAsset(moss_texture, "Moss Texture");
        addAsset(grass_texture, "Grass Texture");

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
            
            // new_tree->rigid_body->position = zge::Vector3(0.0f, 0.0f, 0.0f);

            addObject(new_tree);
        }

        main_light = std::make_shared<zge::LightSource>();
        main_light->model = cube_model;
        addObject(main_light, "Main Light");

        auto monkey = std::make_shared<Suzanne>();
        monkey->model = suzanne_model;
        addObject(monkey);

        s_m = std::make_shared<zge::Shadowmapper>();
    }


    void doUpdate() override
    {
        if (isKeyPressed(Key(J)))
        {
            main_light->position = camera.position;
        }

        if (isKeyPressed(Key(T))) // throw ball 
        {
            auto new_ball = std::make_shared<Ball>(*this);

            new_ball->rigid_body->position = camera.position;
            new_ball->rigid_body->mass = 100.0f;
            new_ball->rigid_body->applyForce(camera.view_direction * 50000.0f);
            
            addObject(new_ball);
        }
    }

    void doRender() override
    {
        // send texture to shadow mapper

        auto texture_shader = getAssetTyped("Texture Shader", zge::Shader);
        
        s_m->doRender(*this);

        s_m->d_t.doUse(3);

        texture_shader->sendUniform("shadowmap_sampler", s_m->d_t);
        texture_shader->sendUniform("light_vp", main_light->getProjection() * main_light->getView());

    }

    std::shared_ptr<zge::Shadowmapper> s_m;
};


int main()
{
    TestingEngine engine{};

    std::string name{"Testing"};

    if (engine.doConsturct(1024, 576, name))
        engine.doStart();
}
