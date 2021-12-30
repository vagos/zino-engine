#include "Monster.hpp"
#include "Collider.hpp"
#include "Engine.hpp"
#include "RigidBody.hpp"
#include "Shader.hpp"
#include "Texture.hpp"

Monster::Monster(zge::Engine& eng)
{
    shader = eng_getAssetTyped("Texture Shader", zge::Shader);
    model = eng_getAssetTyped("Suzanne Model", zge::Model);
    model->texture = eng_getAssetTyped("Fiery Texture", zge::Texture);

    rigid_body = std::make_shared<zge::RigidBody>();
    collider = std::make_shared<zge::CubeCollider>(*model, rigid_body->position);
}

void Monster::doRender(zge::Engine &eng) 
{
    shader->doUse();
    model->doUse();

    zge::Matrix4x4 mvp = eng.camera.getProjection() * eng.camera.getView() * getModelMatrix();

    shader->sendUniform("mvp", mvp);
    shader->sendUniform("m", getModelMatrix());
    shader->sendUniform("texture_sampler", *model->texture);

    model->doRender(eng);
}

void Monster::doUpdate(zge::Engine &eng)
{
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

