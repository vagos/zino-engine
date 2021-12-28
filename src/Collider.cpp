#include "Collider.hpp"
#include "Common.hpp"
#include <glm/gtx/string_cast.hpp>

#define max(a, b) a > b ? a : b
#define min(a, b) a < b ? a : b

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
        collision_direction = glm::normalize(position - other.position);
        collision_direction = zge::Vector3(0, 1, 0);
        // if (has_collided) return false;
        // has_collided = true;

        return 
            
            ( position.x + min_x <= other.position.x + other.max_x && 
                position.x + max_x >= other.position.x + other.min_x ) 

            && 

            (position.y + min_y <= other.position.y + other.max_y &&
             position.y + max_y >= other.position.y + other.min_y) 

            && 

            (position.z + min_z <= other.position.z + other.max_z &&
             position.z + max_z >= other.position.z + other.min_z); 
    }

    void CubeCollider::setCorners(Model &model)
    {
        max_x = min_x = model.vertices[0].x;
        max_y = min_y = model.vertices[0].y;
        max_z = min_z = model.vertices[0].z;

        for (auto& v : model.vertices)
        {
            max_x = max(max_x, v.x);
            max_y = max(max_y, v.y);
            max_z = max(max_z, v.z);

            min_x = min(min_x, v.x);
            min_y = min(min_y, v.y);
            min_z = min(min_z, v.z);
        }

        max_x = max_x - position.x;
        min_x = min_x - position.x;

        max_y = max_y - position.y;
        min_y = min_y - position.y;

        max_z = max_z - position.z;
        min_z = min_z - position.z;
    }

    void CubeCollider::updatePoints(Matrix4x4 &&m)
    {
        Vector3 max_v{max_x, max_y, max_z};
        Vector3 min_v{min_x, min_y, min_z};

        // max_v = Vector3(m * Vector4(max_v, 1.0f));
        // min_v = Vector3(m * Vector4(min_v, 1.0f));

        max_v = Vector3(Matrix3x3(m) * max_v);
        min_v = Vector3(Matrix3x3(m) * min_v);

        max_x = max_v.x;
        max_y = max_v.y;
        max_z = max_v.z;

        min_x = min_v.x;
        min_y = min_v.y;
        min_z = min_v.z;
    }

}
