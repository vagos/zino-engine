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

Object::Object(): exists(true), model_matrix(Matrix4x4(1))
{
}


/* Plane::Plane(float w, float h)
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
*/

}
