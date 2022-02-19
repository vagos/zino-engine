#include <stdexcept>
#include <vector>
#include <cassert>
#include <algorithm>
#include <iostream>

#include "Object.hpp"
#include "Common.hpp"
#include "Engine.hpp"

namespace zge 
{


std::vector<std::shared_ptr<Object>>  Object::objects;

Object::Object(): exists(true), model_matrix(Matrix4x4(1)), base_model_matrix(Matrix4x4(1))
{
}

void Object::setModelMatrix(Matrix4x4 m)
{
    model_matrix = m;
}

void Object::applyTransofrmation(Matrix4x4 &&m)
{
    base_model_matrix = m * base_model_matrix;
    if (collider) collider->updatePoints(getModelMatrix());
}

void Object::removeNullObjects()
{
    objects.erase( std::remove_if(objects.begin(), objects.end(), 
            [](auto& o) {return !o->exists;}), objects.end() );
}

Matrix4x4 Object::getModelMatrix()
{
    return model_matrix * base_model_matrix;
}

void Object::doBasicRender(Engine &eng, Shader& shader)
{
    if (!model) return;
    
    shader.sendUniform("m", getModelMatrix());

    model->doUse();
    model->doRender(eng);

}

}
