#ifndef LIGHTING_HPP_KZRMKXLW
#define LIGHTING_HPP_KZRMKXLW

#include "Common.hpp"
#include "Object.hpp"

namespace zge
{


class LightSource : public ModeledObject
{
    public:

    LightSource()
    {
        model_matrix = glm::translate(model_matrix, Vector3(1.2f, 1.0f, 2.0f)); 
        model_matrix = glm::scale(model_matrix, Vector3(0.2f));
        setModelMatrix(model_matrix);
    }

    private:
        
    zge::Vector3 color;

};

};

#endif /* LIGHTING_HPP_KZRMKXLW */
