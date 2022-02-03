#include "Cube.hpp"
#include "Common.hpp"
#include "Engine.hpp"
#include "Model.hpp"
#include "RigidBody.hpp"
#include <glm/gtx/string_cast.hpp>

Cube::Cube(zge::Engine& eng)
{
    model = eng_getAssetTyped("Cube Model", zge::Model);
    model->texture = eng_getAssetTyped("Grass Texture", zge::Texture);
    rigid_body = std::make_shared<zge::RigidBody>();
    collider = std::make_shared<zge::CubeCollider>(*model, rigid_body->position);

}

void Cube::doRender(zge::Engine& eng)
{
    std::shared_ptr<zge::Shader> basic_shader = eng_getAssetTyped("Texture Shader", zge::Shader);

    basic_shader->doUse();
    
    model->doUse();
    
    zge::Matrix4x4 mvp = eng.camera.getProjection() * eng.camera.getView() * getModelMatrix();
    basic_shader->sendUniform("mvp", mvp);
    basic_shader->sendUniform("m", getModelMatrix());
    basic_shader->sendUniform("texture_sampler", *model->texture);

    model->doRender(eng);
}

void Cube::doUpdate(zge::Engine& eng)
{
    setModelMatrix(glm::translate(zge::Matrix4x4(1), rigid_body->position));
}
