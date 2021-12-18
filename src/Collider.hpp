#ifndef COLLIDER_HPP_PNN6JZXU
#define COLLIDER_HPP_PNN6JZXU

#include "Common.hpp"
class Object;

namespace zge 
{


class Collider 
{
    virtual bool isColliding(Collider& other);

};

class SphereCollider : public Collider 
{
    float radius;
    Vector3 position_offset;

    bool isColliding(Object& other)
    {
        
    }


};

}

#endif /* COLLIDER_HPP_PNN6JZXU */
