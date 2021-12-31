#ifndef LIGHTING_HPP_KZRMKXLW
#define LIGHTING_HPP_KZRMKXLW

#include "Common.hpp"
#include "Object.hpp"
#include <glm/ext/matrix_transform.hpp>

namespace zge
{


class LightSource : public Object
{
public:

    static int n_lights;

    LightSource()
    {
        ambient =  zge::Vector3(0.2f, 0.2f, 0.2f);
        diffuse = zge::Vector3(0.5f, 0.5f, 0.5f); 
        specular = zge::Vector3(1.0f, 1.0f, 1.0f); 

        n_lights++;

        projection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 1.0f, 7.5f);

        look_at = glm::lookAt(glm::vec3(5.0f, 6.0f, 5.0f),
                              glm::vec3(0.0f, 0.0f, 0.0f),
                              glm::vec3(0.0f, 1.0f, 0.0f));
    }

    void doUpdate(Engine &eng) override;
    void doRender(Engine &eng) override;

    zge::Vector3 position;
         
    zge::Vector3 color;

    zge::Vector3 ambient;
    zge::Vector3 diffuse;
    zge::Vector3 specular;
    
    Matrix4x4 look_at;
    Matrix4x4 projection;

    Matrix4x4& getProjection() {return projection;}
    Matrix4x4& getView() {return look_at;}

};

};

#endif /* LIGHTING_HPP_KZRMKXLW */
