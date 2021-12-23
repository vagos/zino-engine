#ifndef COLLIDER_HPP_PNN6JZXU
#define COLLIDER_HPP_PNN6JZXU

#include "Common.hpp"
#include "RigidBody.hpp"

namespace zge 
{


class Collider 
{
public:
    // virtual bool isColliding(Collider& other) {}
    
    Collider(Vector3& position): position(position), position_offset(0.0f)
    {
    }

    Vector3& position;
    Vector3 position_offset;

};

class CubeCollider : public Collider
{
public:
    float max_x, max_y, max_z, min_x, min_y, min_z; 

    CubeCollider(Vector3& position): Collider(position)
    {
    }

    bool isColliding(CubeCollider& other, Vector3& collision_direction);
};

class SphereCollider : public Collider 
{
public:

    SphereCollider(Vector3& position): Collider(position)
    {
    }

    bool isColliding(SphereCollider& other, Vector3& collision_direction);
    bool isColliding(CubeCollider& other, Vector3& collision_direction);

    float radius = 1.0f;
};

}

#endif /* COLLIDER_HPP_PNN6JZXU */
