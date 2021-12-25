#include "Cube.hpp"
#include "Engine.hpp"
#include "Model.hpp"
#include "RigidBody.hpp"

Cube::Cube(zge::Engine& eng)
{
    model = eng_getAssetTyped("Cube Model", zge::Model);
    model->texture = eng_getAssetTyped("Grass Texture", zge::Texture);
    rigid_body = std::make_shared<zge::RigidBody>();
    
    model_matrix = glm::scale(model_matrix, zge::Vector3(100.0f, 1.0f, 100.0f));

    for (int i = 0; i < model->vertices.size(); i++)
    {
        auto& v = model->vertices[i];
        v = zge::Vector3(model_matrix * zge::Vector4(v, 1.0f));
    }

    collider = std::make_shared<zge::CubeCollider>(*model, rigid_body->position);
}

void Cube::doRender(zge::Engine& eng)
{
    std::shared_ptr<zge::Shader> basic_shader = eng_getAssetTyped("Texture Shader", zge::Shader);

    basic_shader->doUse();
    
    model->doUse();
    
    zge::Matrix4x4 mvp = eng.camera.getProjection() * eng.camera.getView() * model_matrix;
    basic_shader->sendUniform("mvp", mvp);
    basic_shader->sendUniform("texture_sampler", model->texture->getTextureUnit());

    model->doRender(eng);
}

void Cube::doUpdate(zge::Engine& eng)
{

}
