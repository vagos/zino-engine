#include "Monster.hpp"
#include "Collider.hpp"
#include "Common.hpp"
#include "Engine.hpp"
#include "Object.hpp"
#include "RigidBody.hpp"
#include "Shader.hpp"
#include "Texture.hpp"
#include "Particles.hpp"
#include <glm/gtx/string_cast.hpp>

Monster::Monster(zge::Engine& eng, std::string shader_n, std::string model_n, std::string texture_n)
    :view_direction(1.0f)
{
    shader  = eng_getAssetTyped(shader_n, zge::Shader);
    model   = eng_getAssetTyped(model_n, zge::Model);
    texture = eng_getAssetTyped(texture_n, zge::Texture);

    rigid_body = std::make_shared<zge::RigidBody>();
    collider = std::make_shared<zge::CubeCollider>(*model, rigid_body->position);

    rigid_body->mass = 1.0f;

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

    for (auto& obj : Object::objects)
    {
       if (obj.get() == this) continue; 

       zge::Vector3 n;

       if ( obj && obj->collider && obj->rigid_body && collider->isColliding(*obj->collider, n) ) 
       {
           rigid_body->doCollision(n, *obj->rigid_body);
       }
    }

    doThink(eng);
    rigid_body->applyGravity();

    rigid_body->doUpdate(eng);
    
    setModelMatrix(glm::translate(zge::Matrix4x4(1.0f), rigid_body->position) * glm::rotate(zge::Matrix4x4(1.0f), rotation_angle, zge::V_UP));


}

void Monster::doThink(zge::Engine& eng)
{
    auto& p_location = eng.camera.position;
    auto view_direction_old = view_direction;
    // view_direction = glm::normalize(p_location - rigid_body->position) * 2.0f;
    
    if (eng.getRandomFloat() < 0.01)
    {
        view_direction = zge::Vector3(eng.getRandomFloat(-1.0f, 1.0f), 0.0f, eng.getRandomFloat(-1.0f, 1.0f));
    }
    
    rotation_angle -= glm::angle(glm::normalize(zge::Vector2(view_direction.x, view_direction.z)), glm::normalize(zge::Vector2(view_direction_old.x, view_direction_old.z)));
    
    attack_cooldown -= eng.getElapsedTime();

    zge::Vector3 new_v = zge::Vector3(view_direction.x, 0, view_direction.z);

    if (eng.isKeyHeld(Key(V)))
    {
        doAttack(eng);
    }

    rigid_body->setVelocity(new_v + zge::Vector3(0.0f, rigid_body->velocity.y, 0.0f));

    if (attack_cooldown < 0.0f)
    {
        doAttack(eng);
        attack_cooldown = eng.getRandomFloat(0.5f, 10.0f);
    }
}

WaterMonster::WaterMonster(zge::Engine& eng) : Monster(eng, "Water Texture Shader", "Pointy Monster Model", "Water Texture")
{
    applyTransofrmation(glm::scale(zge::Matrix4x4(1), zge::Vector3(0.005f)));
    applyTransofrmation(glm::rotate(zge::Matrix4x4(1), zge::PI, zge::V_UP));
    // rigid_body->setPosition(rigid_body->position + zge::Vector3(0, 10, 0));
}

void WaterMonster::doAttack(zge::Engine &eng)
{
    auto particle_emitter = std::make_shared<zge::StarParticleEmitter>(eng, 1, "Water Particle Shader", "Sphere Model");
    particle_emitter->position = rigid_body->position;
    particle_emitter->total_time = 5.0f;
    eng.addObject(particle_emitter);
}

FireMonster::FireMonster(zge::Engine &eng) : Monster(eng, "Fire Texture Shader", "Spider Monkey Model", 
        "Spider Monkey Texture")
{
    applyTransofrmation(glm::scale(zge::Matrix4x4(1), zge::Vector3(0.05f)));
    applyTransofrmation(glm::rotate(zge::Matrix4x4(1), 3*glm::pi<float>()/2.0f, zge::Vector3(1.0f, 0.0f, 0.0f)));
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
    auto particle_emitter = std::make_shared<zge::FireParticleEmitter>(eng, 100, "Fire Particle Shader", "Cube Model", "Fire Particle Texture");
    particle_emitter->position = rigid_body->position;
    particle_emitter->total_time = 5.0f;
    eng.addObject(particle_emitter);
}

ElectricityMonster::ElectricityMonster(zge::Engine& eng) : 
    Monster(eng, "Texture Shader", "Cat Model", "Cat Texture")
{
    applyTransofrmation(glm::scale(zge::Matrix4x4(1), zge::Vector3(0.05f)));
    applyTransofrmation(glm::rotate(zge::Matrix4x4(1), 3*glm::pi<float>()/2.0f, zge::Vector3(1.0f, 0.0f, 0.0f)));
}

void ElectricityMonster::doAttack(zge::Engine &eng)
{
    auto particle_emitter = std::make_shared<zge::FireParticleEmitter>(eng, 5, "Electricity Particle Shader", "Sphere Model", "Fire Particle Texture");
    particle_emitter->position = rigid_body->position;
    particle_emitter->total_time = 5.0f;
    eng.addObject(particle_emitter);
}
void ElectricityMonster::doRender(zge::Engine &eng)
{
    Monster::doRender(eng);

}
