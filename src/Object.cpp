#include <glm/ext/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <iostream>


#include <stdexcept>
#include <vector>
#include <cassert>

#include "Object.hpp"
#include "Common.hpp"
#include "Engine.hpp"

namespace zge 
{


std::vector<std::shared_ptr<Object>>  Object::objects;

Object::Object(): exists(true)
{
}


ModeledObject::ModeledObject(std::shared_ptr<Model>& model): model_file(model), model_matrix(Matrix4x4(1))
{

}

ModeledObject::ModeledObject() :model_matrix(Matrix4x4(1))
{
}


void ModeledObject::doRender(Engine &eng)
{
    assert(shader_file); 
    shader_file->doUse();
    model_file->doUse();

    Matrix4x4 mvp = eng.camera.getProjection() * eng.camera.getView() * model_matrix;

    shader_file->sendUniform("mvp", mvp);
    glDrawArrays(GL_TRIANGLES, 0, model_file->vertices.size());
}

Plane::Plane(float w, float h): ModeledObject()
{
   auto plane_model = std::make_shared<Model>();

   float size = w*h;

   plane_model->setVertices(std::vector<Vector3>{
       Vector3(-size, 0, -size),
       Vector3(-size, 0, size),
       Vector3(size, 0, size),

       Vector3(-size, 0, -size),
       Vector3(size, 0, -size),
       Vector3(size, 0, size),
   });

   model_file = std::move(plane_model);
}

}
