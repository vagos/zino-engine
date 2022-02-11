#include "Common.hpp"

zge::Vector3 Cylindrical2Cartesian(zge::Vector3 v) // x = r z = theta, y = y
{
    float x = v.x * glm::cos(v.z);
    float y = v.y;
    float z = v.x * glm::sin(v.z);

    return zge::Vector3(x, y, z);
}

