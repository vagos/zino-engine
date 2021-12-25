#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include "Common.hpp"

namespace zge 
{

struct Material 
{
    Vector3 ambient;
    Vector3 diffuse;
    Vector3 specular;
    float shininess;

    Material(Vector3 ambient, Vector3 diffuse, Vector3 specular, float shininess):
        ambient(ambient), diffuse(diffuse), specular(specular), shininess(shininess)
    {
    }
};


}

#endif /* MATERIAL_HPP */
