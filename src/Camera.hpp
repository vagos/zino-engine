#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "Common.hpp"
#include "Object.hpp"

/*
   Simple 3D Camera.
*/


namespace zge
{
    struct Camera : public Object
    {

        Camera();
        
   public:

        void doUpdate(Engine& eng) override;

        Matrix4x4& getProjection() {return projection;}
        Matrix4x4& getView() {return look_at;}

        Vector3 position;
        Vector3 view_direction;
    
    private:

        Matrix4x4 look_at; // view Matrix.
        Matrix4x4 projection;

        float fov = 45.0f * 2.0f;

        float horizontal_view_angle;
        float vertical_view_angle;

        const float speed = 20.0f;
    };

};

#endif /* CAMERA_HPP */
