#include "Monster.hpp"
#include "Collider.hpp"
#include "Engine.hpp"
#include "Object.hpp"
#include "RigidBody.hpp"
#include "Shader.hpp"
#include "Texture.hpp"
#include "Particles.hpp"

Monster::Monster(zge::Engine& eng, std::string shader_n, std::string model_n, std::string texture_n)
{
    shader  = eng_getAssetTyped(shader_n, zge::Shader);
    model   = eng_getAssetTyped(model_n, zge::Model);
    texture = eng_getAssetTyped(texture_n, zge::Texture);

    rigid_body = std::make_shared<zge::RigidBody>();
    collider = std::make_shared<zge::CubeCollider>(*model, rigid_body->position);

    type = zge::Object::Type::MONSTER;
}

void Monster::doRender(zge::Engine &eng) 
{
    shader->doUse();
    model->doUse();
    texture->doUse();

    zge::Matrix4x4 mvp = eng.camera.getProjection() * eng.camera.getView() * getModelMatrix();

    shader->sendUniform("mvp", mvp);
    shader->sendUniform("m", getModelMatrix());
    shader->sendUniform("texture_sampler", *texture);
    shader->sendUniform("time", eng.getTime());

    model->doRender(eng);
}

void Monster::doUpdate(zge::Engine &eng)
{

    rigid_body->doUpdate(eng);

    for (auto& obj : Object::objects)
    {
       if (obj.get() == this) continue; 

       zge::Vector3 n;

       if ( obj && obj->collider && obj->rigid_body && collider->isColliding(*obj->collider, n) ) 
       {
           // rigid_body->doCollision(n, *obj->rigid_body);
           if (obj->type == zge::Object::Type::CATCH_BALL)
           {
                exists = false;
           }
       }
    }
    doThink(eng);
    // rigid_body->position += rigid_body->velocity * eng.getElapsedTime();
    setModelMatrix(glm::translate(zge::Matrix4x4(1), rigid_body->position));


}

void Monster::doThink(zge::Engine& eng)
{
    auto& p_location = eng.camera.position;
    auto n_v = glm::normalize(p_location - rigid_body->position) * 2.0f;
    // rigid_body->setVelocity(zge::Vector3(n_v.x, 0, n_v.z));

    attack_cooldown -= eng.getElapsedTime();

    if (attack_cooldown < 0.0f)
    {
        doAttack(eng);
        attack_cooldown = 5.0f;
    }
}

WaterMonster::WaterMonster(zge::Engine& eng) : Monster(eng, "Water Texture Shader", "Pointy Monster Model", "Water Texture")
{
    applyTransofrmation(glm::scale(zge::Matrix4x4(1), zge::Vector3(0.005f)));
    rigid_body->setPosition(rigid_body->position + zge::Vector3(0, 10, 0));
}

void WaterMonster::doAttack(zge::Engine &eng)
{
    auto particle_emitter = std::make_shared<zge::ParticleEmitter>(eng, 1, "Particle Shader", "Sphere Model");
    particle_emitter->position = rigid_body->position;
    eng.addObject(particle_emitter);
}

FireMonster::FireMonster(zge::Engine &eng) : Monster(eng, "Fire Texture Shader", "Spider Monkey Model", 
        "Spider Monkey Texture")
{
    applyTransofrmation(glm::scale(zge::Matrix4x4(1), zge::Vector3(3.0f)));
}

void FireMonster::doRender(zge::Engine &eng)
{
    shader->doUse();
    model->doUse();
    texture->doUse();

    auto fire_texture = eng_getAssetTyped("Fiery Texture", zge::Texture);
    fire_texture->doUse(5);

    zge::Matrix4x4 mvp = eng.camera.getProjection() * eng.camera.getView() * getModelMatrix();

    shader->sendUniform("mvp", mvp);
    shader->sendUniform("m", getModelMatrix());
    shader->sendUniform("texture_sampler", *texture);
    shader->sendUniform("texture_sampler_overlay", *fire_texture);
    shader->sendUniform("time", eng.getTime());

    model->doRender(eng);
}

void FireMonster::doAttack(zge::Engine &eng)
{
}
