#include "Monster.hpp"
#include "Collider.hpp"
#include "Engine.hpp"
#include "Object.hpp"
#include "RigidBody.hpp"
#include "Shader.hpp"
#include "Texture.hpp"

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
           if (obj->type == zge::Object::Type::BALL)
           {
                exists = false;

                std::clog << "ball!";
           }
       }
    }

    doThink(eng);
    rigid_body->position += rigid_body->velocity * eng.getElapsedTime();
    setModelMatrix(glm::translate(zge::Matrix4x4(1), rigid_body->position));


}

void Monster::doThink(zge::Engine& eng)
{
    auto& p_location = eng.camera.position;
    auto n_v = glm::normalize(p_location - rigid_body->position) * 2.0f;
    rigid_body->setVelocity(zge::Vector3(n_v.x, 0, n_v.z));
}

