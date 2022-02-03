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
#include "Particles.hpp"
#include "ShadowMapper.hpp"
#include "Tree.hpp"

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
        auto moss_texture = std::make_shared<zge::Texture>("./assets/textures/trees/Mossy_Tr.bmp");
        auto grass_texture = std::make_shared<zge::Texture>("./assets/textures/grass/grass_o.bmp");
        auto fiert_texture = std::make_shared<zge::Texture>("./assets/textures/fiery.bmp");
        auto water_texture = std::make_shared<zge::Texture>("./assets/textures/water.bmp");
        auto spider_monkey_texture = std::make_shared<zge::Texture>("./assets/textures/spider_monkey.bmp");
        auto pointy_ear_monster = std::make_shared<zge::Model>("./assets/objs/little-monster.obj");
        auto spider_monkey_model = std::make_shared<zge::Model>("./assets/objs/spider_monkey.obj");

        addAsset(fiert_texture, "Fiery Texture");
        addAsset(water_texture, "Water Texture");
        addAsset(spider_monkey_texture, "Spider Monkey Texture");
        addAsset(tree_model, "Tree Model");
        addAsset(sphere_model, "Sphere Model");
        addAsset(cube_model, "Cube Model");
        addAsset(suzanne_model, "Suzanne Model");
        addAsset(pointy_ear_monster, "Pointy Monster Model");
        addAsset(spider_monkey_model, "Spider Monkey Model");

        auto basic_shader = std::make_shared<zge::Shader>("./assets/shaders/basic.vert", "./assets/shaders/basic.frag"); 
        auto lighting_shader = std::make_shared<zge::Shader>("./assets/shaders/basic.vert", "./assets/shaders/lighting.frag"); // testing ligthing

        auto texture_shader = std::make_shared<zge::Shader>("./assets/shaders/texture.vert", "./assets/shaders/texture.frag");
        auto transform_shader = std::make_shared<zge::Shader>("./assets/shaders/transform.vert", "./assets/shaders/transform.frag");
        auto water_texture_shader = std::make_shared<zge::Shader>("./assets/shaders/water_texture.vert", "./assets/shaders/water_texture.frag");
        auto fire_texture_shader = std::make_shared<zge::Shader>("./assets/shaders/fire_texture.vert", "./assets/shaders/fire_texture.frag");

        auto depth_shader = std::make_shared<zge::Shader>("./assets/shaders/depth.vert", "./assets/shaders/depth.frag");
        auto debug_shader = std::make_shared<zge::Shader>("./assets/shaders/debug.vert", "./assets/shaders/debug.frag");

        addAsset(basic_shader, "Basic Shader");
        addAsset(transform_shader, "Transform Shader");
        addAsset(texture_shader, "Texture Shader");
        addAsset(depth_shader, "Depth Shader");
        addAsset(debug_shader, "Debug Shader");

        addAsset(water_texture_shader, "Water Texture Shader");
        addAsset(fire_texture_shader, "Fire Texture Shader");

        addAsset(moss_texture, "Moss Texture");
        addAsset(grass_texture, "Grass Texture");

        // cubemap stuff (skybox)
        auto skybox = std::make_shared<zge::Skybox>();
        skybox->name = std::string("Skybox");
        addObject(skybox);

        auto new_island = std::make_shared<Cube>(*this);
        new_island->rigid_body->setPosition({0, 0, 0});
        new_island->applyTransofrmation(glm::scale(zge::Matrix4x4(1), zge::Vector3(100, getRandomInt(1, 5), 100)));
        addObject(new_island);

        auto new_tree = std::make_shared<Tree>(*this);
        zge::Vector3 t_v{0, 0, 0};
        new_tree->rigid_body->setPosition(t_v);
        addObject(new_tree);

        for (int i = 0; i < 0; i++)
        {
            int gap = 200;

            auto new_island = std::make_shared<Cube>(*this);
            
            zge::Vector3 random_v{getRandomInt(-gap, gap), getRandomInt(-gap, gap), getRandomInt(-gap, gap)};
            new_island->applyTransofrmation(glm::scale(zge::Matrix4x4(1), zge::Vector3(100, getRandomInt(1, 5), 100)));

            new_island->rigid_body->setPosition(random_v);

            addObject(new_island);

            for (int i = 0; i < 1; i++) // create trees
            {
                if (getRandomFloat() < 0.5) continue;

                auto new_tree = std::make_shared<Tree>(*this);
                zge::Vector3 t_v{getRandomInt(-50, 50), 0, getRandomInt(-50, 50)};
                new_tree->rigid_body->setPosition(random_v + t_v);

                addObject(new_tree);
            }
        }

        main_light = std::make_shared<zge::LightSource>();
        // main_light->model = cube_model;
        addObject(main_light, "Main Light");

        // auto monkey = std::make_shared<Suzanne>();
        // monkey->model = suzanne_model;
        // addObject(monkey);

        s_m = std::make_shared<zge::Shadowmapper>();

        // particles 
        auto particle_shader = std::make_shared<zge::Shader>("./assets/shaders/particle.vert", "./assets/shaders/particle.frag");
        addAsset(particle_shader, "Particle Shader");

        auto water_particle_shader = std::make_shared<zge::Shader>("./assets/shaders/water.vert", "./assets/shaders/water.frag");
        addAsset(water_particle_shader, "Water Particle Shader");

        // auto particle_emitter = std::make_shared<zge::ParticleEmitter>(*this);
        // addObject(particle_emitter, "Particle Emitter");

        transforming_model = std::make_shared<zge::TransformingModel>("./assets/objs/spider_monkey.obj", "./assets/objs/spider_monkey_other.obj");
    }

    void doUpdate() override
    {
        if (isKeyPressed(Key(N)))
        {
            main_light->position = camera.position;
        }

        if (isKeyPressed(Key(C)))
        {
           catching_monsters = !catching_monsters; 

           std::clog << "CATCH: " << (catching_monsters ? "True" : "False") << '\n';
        }

        if (isKeyPressed(Key(T))) // throw ball 
        {
            auto new_ball = std::make_shared<Ball>(*this);

            if (catching_monsters)
            {
                new_ball->type = zge::Object::Type::CATCH_BALL;
            }

            new_ball->rigid_body->position = camera.position;
            new_ball->rigid_body->mass = 100.0f;
            new_ball->rigid_body->applyForce(camera.view_direction * 9000.0f);
            
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
        texture_shader->sendUniform("light_vp", main_light->getProjection() *
                                                   main_light->getView());

        transforming_model->doRender(*this);
    }

    std::shared_ptr<zge::Shadowmapper> s_m;
    std::shared_ptr<zge::TransformingModel> transforming_model;

    bool catching_monsters = false;
};


int main()
{
    TestingEngine engine{};

    std::string name{"Testing"};

    if (engine.doConsturct(1024, 576, name))
        engine.doStart();
}
