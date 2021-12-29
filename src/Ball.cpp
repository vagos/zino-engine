#include "Ball.hpp"
#include "Collider.hpp"
#include "Common.hpp"
#include "Engine.hpp"
#include "Tree.hpp"

#include <glm/gtx/dual_quaternion.hpp>
#include <glm/gtx/string_cast.hpp>
#include <memory>

Ball::Ball(zge::Engine& eng)
{
    model = eng_getAssetTyped("Sphere Model", zge::Model);
    rigid_body = std::make_shared<zge::RigidBody>();
    collider = std::make_shared<zge::CubeCollider>(*model, rigid_body->position);
}


void Ball::doRender(zge::Engine &eng) 
{
    std::shared_ptr<zge::Shader> basic_shader = eng_getAssetTyped("Texture Shader", zge::Shader);

    auto t = eng_getAssetTyped("Fiery Texture", zge::Texture);

    basic_shader->doUse();

    t->doUse();
    
    model->doUse();
    
    zge::Matrix4x4 mvp = eng.camera.getProjection() * eng.camera.getView() * getModelMatrix();
    basic_shader->sendUniform("mvp", mvp);
    basic_shader->sendUniform("m", getModelMatrix());

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
            // std::clog << "collision!\n\n";
            // std::clog << "n: " << glm::to_string(n) << '\n';
            n_bounces++;

            if (n_bounces == 3) 
            {
                exists = false;

                auto monster = std::make_shared<Tree>(eng);
                monster->rigid_body->setPosition(rigid_body->position);
                eng.addObject(monster);
            }
       }
    }

    setModelMatrix(glm::translate(getModelMatrix(), rigid_body->position));
}
