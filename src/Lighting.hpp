#ifndef LIGHTING_HPP_KZRMKXLW
#define LIGHTING_HPP_KZRMKXLW

#include "Common.hpp"
#include "Object.hpp"

namespace zge
{


class LightSource : public Object
{
public:

    LightSource()
    {
        ambient =  zge::Vector3(0.2f, 0.2f, 0.2f);
        diffuse = zge::Vector3(0.5f, 0.5f, 0.5f); 
        specular = zge::Vector3(1.0f, 1.0f, 1.0f); 
    }

    zge::Vector3 position;
         
    zge::Vector3 color;

    zge::Vector3 ambient;
    zge::Vector3 diffuse;
    zge::Vector3 specular;
};

};

#endif /* LIGHTING_HPP_KZRMKXLW */
