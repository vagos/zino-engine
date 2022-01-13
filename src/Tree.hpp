#ifndef TREE_HPP
#define TREE_HPP

#include "Object.hpp"
#include "Material.hpp"
#include "Engine.hpp"
#include "Shader.hpp"

struct Tree : public zge::Object
{

    std::shared_ptr<zge::Shader> texture_shader = nullptr;
    zge::Material material;

    Tree(zge::Engine& eng): material(zge::Vector3(1.0f, 0.5, 0.31f), 
            zge::Vector3(1.0f, 0.5f, 0.31f), zge::Vector3(0.5f, 0.5f, 0.5f), 32.0f)
    {
        model = std::static_pointer_cast<zge::Model>(eng.getAsset("Tree Model"));
        rigid_body = std::make_shared<zge::RigidBody>();

        model->texture = eng_getAssetTyped("Moss Texture", zge::Texture);
        texture_shader = eng_getAssetTyped("Texture Shader", zge::Shader);

        // rigid_body->setPosition(zge::Vector3(10, 10, 10));
        collider = std::make_shared<zge::CubeCollider>(*model, rigid_body->position);

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
    }

};

#endif /* TREE_HPP */

