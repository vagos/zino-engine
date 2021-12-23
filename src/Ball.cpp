#include "Ball.hpp"
#include "Collider.hpp"
#include "Common.hpp"
#include "Engine.hpp"
#include <glm/gtx/dual_quaternion.hpp>
#include <memory>

Ball::Ball()
{
    rigid_body = std::make_shared<zge::RigidBody>();
    collider = std::make_shared<zge::SphereCollider>(rigid_body->position);
    collider->radius = 1.0f;
}


void Ball::doRender(zge::Engine &eng) 
{
    std::shared_ptr<zge::Shader> basic_shader = eng_getAssetTyped("Texture Shader", zge::Shader);
    
    model->doUse();
    
    zge::Matrix4x4 mvp = eng.camera.getProjection() * eng.camera.getView() * model_matrix;
    basic_shader->sendUniform("mvp", mvp);

    model->doRender(eng);
}

void Ball::doUpdate(zge::Engine &eng)
{
    rigid_body->applyGravity();
    rigid_body->doUpdate(eng);

    for (auto& obj : Object::objects)
    {
       if (obj.get() == this) continue; 

       zge::Vector3 n;

       if ( obj->collider && obj->rigid_body && collider->isColliding(*obj->collider, n) ) 
       {
            rigid_body->doCollision(n, *obj->rigid_body);
            std::clog << "collision!\n";
       }
    }

    // rigid_body->velocity = glm::normalize(rigid_body->velocity);

    setModelMatrix(glm::scale(glm::translate(zge::Matrix4x4(1), rigid_body->position), zge::Vector3(collider->radius)));
}
