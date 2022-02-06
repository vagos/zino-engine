#include "Ball.hpp"
#include "Collider.hpp"
#include "Common.hpp"
#include "Engine.hpp"
#include "Object.hpp"
#include "Tree.hpp"
#include "Monster.hpp"
#include "Particles.hpp"

#include <glm/gtx/dual_quaternion.hpp>
#include <glm/gtx/string_cast.hpp>
#include <memory>

Ball::Ball(zge::Engine& eng) 
{
    model = eng_getAssetTyped("Pokeball Model", zge::Model);
    rigid_body = std::make_shared<zge::RigidBody>();
    collider = std::make_shared<zge::CubeCollider>(*model, rigid_body->position);
    
    type = Object::Type::BALL;

    // applyTransofrmation(glm::scale(zge::Matrix4x4(1), zge::Vector3(0.9)));
    // applyTransofrmation(glm::translate(zge::Matrix4x4(1), zge::Vector3(0.0f, 0.0f, 0.0f)));
}


void Ball::doRender(zge::Engine &eng) 
{
    std::shared_ptr<zge::Shader> basic_shader = eng_getAssetTyped("Texture Shader", zge::Shader);

    auto t = eng_getAssetTyped("Pokeball Texture", zge::Texture);

    basic_shader->doUse();

    t->doUse();
    
    model->doUse();
    
    zge::Matrix4x4 mvp = eng.camera.getProjection() * eng.camera.getView() * getModelMatrix();
    basic_shader->sendUniform("mvp", mvp);
    basic_shader->sendUniform("m", getModelMatrix());

    model->doRender(eng);
}

void Ball::spawnMonster(zge::Engine& eng, std::shared_ptr<Monster> monster)
{
    monster->rigid_body->setPosition(rigid_body->position);
    monster->exists = true;
    // auto particle_emitter = std::make_shared<zge::ParticleEmitter>(eng, 1, "Water Particle Shader", "Sphere Model");
    
    auto particle_emitter = std::make_shared<zge::ParticleEmitter>(eng, 50, "Particle Shader", "Cube Model");
    particle_emitter->total_time = 5.0f;
    particle_emitter->position = rigid_body->position;
//    eng.addObject(particle_emitter);
    eng.addObject(monster);
}

void Ball::doUpdate(zge::Engine &eng)
{
    rigid_body->applyGravity();
    rigid_body->doUpdate(eng);

    for (auto& obj : Object::objects)
    {
       if (obj.get() == this) continue; 

       zge::Vector3 n;

       if ( obj && obj->collider && obj->rigid_body && collider->isColliding(*obj->collider, n) ) 
       {
            rigid_body->doCollision(n, *obj->rigid_body);

            n_bounces++;

            if (type == zge::Object::Type::CATCH_BALL)
            {
                if (obj->type == zge::Object::Type::MONSTER) // catch a monster
                {
                    caught_monsters.push_back(std::static_pointer_cast<Monster>(obj)); 

                    auto particle_emitter = std::make_shared<zge::ParticleEmitter>(eng, 5, "Particle Shader", "Star Model");
                    particle_emitter->total_time = 5.0f;
                    particle_emitter->position = rigid_body->position;

                    eng.addObject(particle_emitter);

                    obj->exists = false; // remove the ball and  the monster from the game world.
                    exists = false;
                }
            }

            if (type == zge::Object::Type::BALL && n_bounces == 2 && !caught_monsters.empty()) // spawn a monster
            {
                exists = false;

                auto monster = caught_monsters.back();
                caught_monsters.pop_back();
                spawnMonster(eng, monster);
            }
       }
    }

    setModelMatrix(glm::translate(getModelMatrix(), rigid_body->position));
}

std::vector<std::shared_ptr<Monster>> Ball::caught_monsters;
