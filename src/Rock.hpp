#ifndef ROCK_HPP_RUJGK4HG
#define ROCK_HPP_RUJGK4HG

#include "Model.hpp"
#include "Object.hpp"
#include "Material.hpp"
#include "Engine.hpp"
#include "RigidBody.hpp"
#include "Shader.hpp"
#include "Texture.hpp"

struct Rock : public zge::Object
{
    std::shared_ptr<zge::Shader> texture_shader = nullptr;

    Rock(zge::Engine& eng)
    {
        std::vector<std::string> rock_types{"A", "G"};
        std::string rnd_type = rock_types[eng.getRandomInt(0, rock_types.size())];


        model = eng_getAssetTyped("Rock Model " + rnd_type, zge::Model);
        rigid_body = std::make_shared<zge::RigidBody>();
        model->texture = eng_getAssetTyped("Rock Texture", zge::Texture);

        texture_shader = eng_getAssetTyped("Texture Shader", zge::Shader);

        collider = std::make_shared<zge::CubeCollider>(*model, rigid_body->position);

        // applyTransofrmation(glm::scale(zge::Matrix4x4(1), zge::Vector3(0.2)));
    }

    void doUpdate(zge::Engine &eng) override
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
        // texture_shader->sendUniform("material", material);

        model->doRender(eng);
    }

};

#endif /* end of include guard: ROCK_HPP_RUJGK4HG */
