#include "Ball.hpp"
#include "Engine.hpp"


void Ball::doRender(zge::Engine &eng) 
{
    std::shared_ptr<zge::Shader> basic_shader = std::static_pointer_cast<zge::Shader>(eng.getAsset("Basic Shader"));
    
    model->doUse();
    
    zge::Matrix4x4 mvp = eng.camera.getProjection() * eng.camera.getView() * model_matrix;
    basic_shader->sendUniform("mvp", mvp);

    model->doRender(eng);
}

void Ball::doUpdate(zge::Engine &eng)
{
    rigid_body->applyGravity();
    rigid_body->doUpdate(eng);

    setModelMatrix(glm::translate(zge::Matrix4x4(1), rigid_body->position));
}
