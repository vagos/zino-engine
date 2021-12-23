#include "Collider.hpp"
#include "Common.hpp"

namespace zge 
{

    bool SphereCollider::isColliding(SphereCollider& other, Vector3& collision_direction)
    {
       auto m_pos = position + position_offset; // my position
       auto o_pos = other.position + other.position_offset; // other position
        
       if ( glm::length(m_pos - o_pos) < radius + other.radius ) 
       {
           // calculate collision response
           collision_direction = glm::normalize( m_pos - o_pos );
           return true; 
       }

       return false;

    }

    bool SphereCollider::isColliding(CubeCollider& other, Vector3& collision_direction)
    {
       auto m_pos = position + position_offset;
       auto o_pos = other.position + other.position_offset;

       return false;
    }
   
    bool CubeCollider::isColliding(CubeCollider& other, Vector3& collision_direction)
    {

    }

}
