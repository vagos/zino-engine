#include <glm/ext/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <iostream>


#include <stdexcept>
#include <vector>
#include <cassert>
#include <algorithm>

#include "Object.hpp"
#include "Common.hpp"
#include "Engine.hpp"

namespace zge 
{


std::vector<std::shared_ptr<Object>>  Object::objects;

Object::Object(): exists(true), model_matrix(Matrix4x4(1))
{
}

void Object::removeNullObjects()
{
    objects.erase( std::remove_if(objects.begin(), objects.end(), 
            [](auto& o) {return !o->exists;}), objects.end() );
}


}
