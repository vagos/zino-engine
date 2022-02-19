#include "Collider.hpp"
#include "Player.hpp"
#include "Quad.hpp"
#include "Engine.hpp"
#include "Common.hpp"
#include "Model.hpp"
#include "Monster.hpp"
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
#include "Rock.hpp"

#include <memory>

// #define DEBUG

class TestingEngine : public zge::Engine
{
    void onCreate() override
    {
        auto suzanne_model         = std::make_shared<zge::Model>("./assets/objs/suzanne.obj");
        auto heart_model           = std::make_shared<zge::Model>("./assets/objs/heart.obj");
        auto cube_model            = std::make_shared<zge::Model>("./assets/objs/cube.obj");
        auto tree_model            = std::make_shared<zge::Model>("./assets/objs/tree.obj");
        auto sphere_model          = std::make_shared<zge::Model>("./assets/objs/sphere.obj");
        auto star_model            = std::make_shared<zge::Model>("./assets/objs/star.obj");
        auto pointy_ear_monster    = std::make_shared<zge::Model>("./assets/objs/little-monster.obj");
        auto spider_monkey_model   = std::make_shared<zge::Model>("./assets/objs/monkey.obj");
        auto pokeball_model        = std::make_shared<zge::Model>("./assets/objs/pokeball.obj");
        auto rock_model_a          = std::make_shared<zge::Model>("./assets/objs/rocks/rock-A.obj");
        auto rock_model_g          = std::make_shared<zge::Model>("./assets/objs/rocks/rock-G.obj");
        auto cat_model             = std::make_shared<zge::Model>("./assets/objs/cat.obj");
        auto gorilla_model         = std::make_shared<zge::Model>("./assets/objs/gorilla.obj");

        auto moss_texture          = std::make_shared<zge::Texture>("./assets/textures/trees/Mossy_Tr.bmp");
        auto grass_texture         = std::make_shared<zge::Texture>("./assets/textures/grass/grass_o.bmp");
        auto fiert_texture         = std::make_shared<zge::Texture>("./assets/textures/fiery.bmp");
        auto water_texture         = std::make_shared<zge::Texture>("./assets/textures/water.bmp");
        auto spider_monkey_texture = std::make_shared<zge::Texture>("./assets/textures/monkey.bmp");
        auto fire_particle_texture = std::make_shared<zge::Texture>("./assets/textures/grass/grass_o.bmp");
        auto pokeball_texture      = std::make_shared<zge::Texture>("./assets/textures/pokeball.bmp");
        auto pokeball_texture_b    = std::make_shared<zge::Texture>("./assets/textures/pokeball_b.bmp");
        auto rock_texture          = std::make_shared<zge::Texture>("./assets/textures/rocks/rock.bmp");
        auto cat_texture           = std::make_shared<zge::Texture>("./assets/textures/cat.bmp");
        auto rock_normal_texture   = std::make_shared<zge::Texture>("./assets/textures/rocks/rock-normals.bmp");
        
        addAsset(fiert_texture         , "Fiery Texture");
        addAsset(water_texture         , "Water Texture");
        addAsset(pokeball_texture      , "Pokeball Texture A");
        addAsset(pokeball_texture_b    , "Pokeball Texture B");
        addAsset(spider_monkey_texture , "Spider Monkey Texture");
        addAsset(rock_texture          , "Rock Texture");
        addAsset(rock_normal_texture   , "Rock Normal Texture");
        addAsset(cat_texture           , "Cat Texture");

        addAsset(tree_model            , "Tree Model");
        addAsset(sphere_model          , "Sphere Model");
        addAsset(cube_model            , "Cube Model");
        addAsset(star_model            , "Star Model");
        addAsset(suzanne_model         , "Suzanne Model");
        addAsset(pointy_ear_monster    , "Pointy Monster Model");
        addAsset(spider_monkey_model   , "Spider Monkey Model");
        addAsset(pokeball_model        , "Pokeball Model");
        addAsset(gorilla_model         , "Gorilla Model");
        addAsset(fire_particle_texture , "Fire Particle Texture");
        addAsset(rock_model_a          , "Rock Model A");
        addAsset(rock_model_g          , "Rock Model G");
        addAsset(cat_model             , "Cat Model");

        auto basic_shader         = std::make_shared<zge::Shader>("./assets/shaders/basic.vert"         , "./assets/shaders/basic.frag");
        auto lighting_shader      = std::make_shared<zge::Shader>("./assets/shaders/basic.vert"         , "./assets/shaders/lighting.frag"); // testing ligthing

        auto texture_shader       = std::make_shared<zge::Shader>("./assets/shaders/texture.vert"       , "./assets/shaders/texture.frag");
        auto transform_shader     = std::make_shared<zge::Shader>("./assets/shaders/transform.vert"     , "./assets/shaders/transform.frag");
        auto water_texture_shader = std::make_shared<zge::Shader>("./assets/shaders/water_texture.vert" , "./assets/shaders/water_texture.frag");
        auto fire_texture_shader  = std::make_shared<zge::Shader>("./assets/shaders/fire_texture.vert"  , "./assets/shaders/fire_texture.frag");

        auto depth_shader         = std::make_shared<zge::Shader>("./assets/shaders/depth.vert"                 , "./assets/shaders/depth.frag");
        auto debug_shader         = std::make_shared<zge::Shader>("./assets/shaders/debug.vert"                 , "./assets/shaders/debug.frag");

        addAsset(basic_shader         , "Basic Shader");
        addAsset(transform_shader     , "Transform Shader");
        addAsset(texture_shader       , "Texture Shader");
        addAsset(depth_shader         , "Depth Shader");
        addAsset(debug_shader         , "Debug Shader");

        addAsset(water_texture_shader , "Water Texture Shader");
        addAsset(fire_texture_shader  , "Fire Texture Shader");

        addAsset(moss_texture         , "Moss Texture");
        addAsset(grass_texture        , "Grass Texture");

        // cubemap stuff (skybox)
        auto skybox = std::make_shared<zge::Skybox>();
        skybox->name = std::string("Skybox");
        addObject(skybox);

        auto new_island = std::make_shared<Cube>(*this);
        new_island->rigid_body->setPosition({0, 0, 0});
        new_island->applyTransofrmation(glm::scale(zge::Matrix4x4(1), zge::Vector3(100, getRandomInt(1, 5), 100))); // 100 x 100 cube
        addObject(new_island);


        for (int i = 0; i < 10; i++)
        {
            auto new_tree = std::make_shared<Tree>(*this);
            zge::Vector3 t_v{getRandomInt(-50, 50), 1, getRandomInt(-50, 50)};
            new_tree->rigid_body->setPosition(t_v);
            addObject(new_tree);

            auto new_rock = std::make_shared<Rock>(*this);
            zge::Vector3 r_v{getRandomInt(-50, 50), 1, getRandomInt(-50, 50)};
            new_rock->rigid_body->setPosition(r_v);
            addObject(new_rock);
        }

        for (int i = 0; i < 0; i++) // extra islands
        {
            int gap = 200;

            auto new_island = std::make_shared<Cube>(*this);
            
            zge::Vector3 random_v{getRandomInt(-gap, gap), getRandomInt(-5, 5), getRandomInt(-gap, gap)};
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

        auto main_light = std::make_shared<zge::LightSource>();
        main_light->model = cube_model;
        addObject(main_light, "Main Light");

        s_m = std::make_shared<zge::Shadowmapper>();

        // particles 
        auto particle_shader = std::make_shared<zge::Shader>("./assets/shaders/particle.vert", "./assets/shaders/particle.frag");
        addAsset(particle_shader, "Particle Shader");

        auto water_particle_shader = std::make_shared<zge::Shader>("./assets/shaders/water.vert", "./assets/shaders/water.frag");
        addAsset(water_particle_shader, "Water Particle Shader");

        auto fire_particle_shader = std::make_shared<zge::Shader>("./assets/shaders/fire_particle.vert", "./assets/shaders/fire_particle.frag");
        addAsset(fire_particle_shader, "Fire Particle Shader");

        auto electricity_particle_shader = std::make_shared<zge::Shader>("./assets/shaders/electricity_particle.vert", "./assets/shaders/electricity_particle.frag");
        addAsset(electricity_particle_shader, "Electricity Particle Shader");

        auto star_particle_shader = std::make_shared<zge::Shader>("./assets/shaders/star_particle.vert", "./assets/shaders/star_particle.frag");
        addAsset(star_particle_shader, "Star Particle Shader");

        transforming_model = std::make_shared<zge::TransformingModel>("./assets/objs/spider_monkey.obj", "./assets/objs/spider_monkey_other.obj");

        Ball::caught_monsters.push_back( std::make_shared<WaterMonster>(*this) );
        Ball::caught_monsters.push_back( std::make_shared<FireMonster>(*this) );
        Ball::caught_monsters.push_back( std::make_shared<ElectricityMonster>(*this) );

        auto player = std::make_shared<zge::Player>(*this); // create player
        addObject(player, "Player");
    }

    void doUpdate() override
    {

        if (isKeyPressed(Key(ESCAPE)))
        {
           doExit(); 
        }

        if (isKeyPressed(Key(C)))
        {
           catching_monsters = !catching_monsters; 
#ifdef DEBUG
           std::clog << "CATCH: " << (catching_monsters ? "True" : "False") << '\n';
#endif
        }

        if (isKeyPressed(Key(T))) // throw ball 
        {
            auto new_ball = std::make_shared<Ball>(*this);

            if (catching_monsters)
            {
                new_ball->type = zge::Object::Type::CATCH_BALL;
            }

            new_ball->rigid_body->position = camera.position + glm::normalize(camera.view_direction) * 6.0f;
            new_ball->rigid_body->mass = 100.0f;
            new_ball->rigid_body->applyForce(camera.view_direction * 30000.0f);
            
            addObject(new_ball);
        }

        if (isKeyPressed(Key(1)))
        {
            Ball::monster_selected = 0;
        }
        if (isKeyPressed(Key(2)))
        {
            Ball::monster_selected = 1;
        }
        if (isKeyPressed(Key(3)))
        {
            Ball::monster_selected = 2;
        }
    }

    void doRender() override
    {
        // send texture to shadow mapper

        auto texture_shader = getAssetTyped("Texture Shader", zge::Shader);
        auto main_light = std::static_pointer_cast<zge::LightSource>(getObject("Main Light"));
        
        s_m->doRender(*this);
        s_m->d_t.doUse(10);
        
        texture_shader->doUse();

        texture_shader->sendUniform("shadowmap_sampler", s_m->d_t);
        texture_shader->sendUniform("light_vp", main_light->getProjection() * main_light->getView());

        transforming_model->doRender(*this);
    }

    std::shared_ptr<zge::Shadowmapper> s_m;
    std::shared_ptr<zge::TransformingModel> transforming_model;

    bool catching_monsters = false;
};


int main()
{
    TestingEngine engine{};

    std::string name{"Graphics Project - Pokemon"};

    if (engine.doConsturct(1920, 1080, name))
        engine.doStart();
}
