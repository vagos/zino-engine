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
    
    LightSource();

    void doUpdate(Engine &eng) override;
    void doRender(Engine &eng) override;

    zge::Vector3 position;
    zge::Vector3 direction;
    zge::Vector3 target_position;

    zge::Vector3 color;

    zge::Vector3 ambient;
    zge::Vector3 diffuse;
    zge::Vector3 specular;
    
    Matrix4x4 look_at;
    Matrix4x4 projection;

    Matrix4x4& getProjection() {return projection;}
    Matrix4x4& getView() {return look_at;}

    float speed = 0.1f;

};

};

#endif /* LIGHTING_HPP_KZRMKXLW */
