#include "Player.hpp"
#include "Collider.hpp"
#include "Common.hpp"
#include "Model.hpp"
#include "Object.hpp"
#include "RigidBody.hpp"
#include "Shader.hpp"
#include "Engine.hpp"

namespace zge 
{
    Player::Player(Engine &eng)
    {
        model = eng_getAssetTyped("Tree Model", zge::Model);
        rigid_body = std::make_shared<RigidBody>();

        collider = std::make_shared<zge::CubeCollider>(*model, rigid_body->position);

        rigid_body->position = Vector3(0.0f, 10.0f, 0.0f);
        applyTransofrmation(glm::scale(zge::Matrix4x4(1), zge::Vector3(0.2)));

        rigid_body->mass = 1e10;

    }

    void Player::doRender(Engine &eng)
    {
        auto shader = eng_getAssetTyped("Texture Shader", zge::Shader);

        zge::Matrix4x4 mvp = eng.camera.getProjection() * eng.camera.getView() * getModelMatrix();

        shader->sendUniform("mvp", mvp);
        shader->sendUniform("m", getModelMatrix());
        // shader->sendUniform("texture_sampler", *model->texture);

        model->doUse();
        // model->doRender(eng);
    }

    void Player::doUpdate(Engine &eng)
    {
        auto& position = rigid_body->position;

        if (eng.isKeyHeld(Key(W))) position += glm::normalize(eng.camera.view_direction) * eng.getElapsedTime() * speed;
        if (eng.isKeyHeld(Key(S))) position -= glm::normalize(eng.camera.view_direction) * eng.getElapsedTime() * speed;
        if (eng.isKeyHeld(Key(D))) position += glm::normalize(eng.camera.right_to_view)  * eng.getElapsedTime() * speed;
        if (eng.isKeyHeld(Key(A))) position -= glm::normalize(eng.camera.right_to_view)  * eng.getElapsedTime() * speed;

        if (eng.isKeyHeld(Key(Q))) position += Vector3(0, 1, 0) * eng.getElapsedTime() * speed;
        if (eng.isKeyHeld(Key(E))) position -= Vector3(0, 1, 0) * eng.getElapsedTime() * speed;

        if (eng.isKeyHeld(Key(SPACE)))
        {
            rigid_body->setVelocity(Vector3(0.0f, 1.0f, 0.0f) * 10.0f);
            // rigid_body->applyForce(Vector3(0.0f, 1.0f, 0.0f) * 50000.0f);
        }

        eng.camera.position = position + Vector3(0.0f, 2.0f, 0.0f);
        setModelMatrix(glm::translate(zge::Matrix4x4(1), position));

        rigid_body->doUpdate(eng);

        zge::Vector3 n;

        for (auto& obj : Object::objects)
        {
           if (obj.get() == this) continue; 

           if ( obj && obj->collider && obj->rigid_body && collider->isColliding(*obj->collider, n) ) 
           {
                rigid_body->setVelocity(Vector3(0.0f));
                return;
           }
        }
        
        rigid_body->applyGravity();
    }
}
