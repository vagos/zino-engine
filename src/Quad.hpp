#ifndef QUAD_HPP
#define QUAD_HPP

#include "Object.hpp"

namespace zge 
{

struct Quad : public Object 
{
   Quad()
   {
        std::vector<Vector3> quadVertices = {
         Vector3(0.5, 0.5, 0.0),
         Vector3(1.0, 0.5, 0.0),
         Vector3(1.0, 1.0, 0.0),
         Vector3(1.0, 1.0, 0.0),
         Vector3(0.5, 1.0, 0.0),
         Vector3(0.5, 0.5, 0.0)
       };

       std::vector<Vector2> quadUVs = {
         Vector2(0.0, 0.0),
         Vector2(1.0, 0.0),
         Vector2(1.0, 1.0),
         Vector2(1.0, 1.0),
         Vector2(0.0, 1.0),
         Vector2(0.0, 0.0)
       };

        model = std::make_shared<Model>(quadVertices, quadUVs);
   }

   void doRender(Engine &eng) override
   {
      model->doUse();
      model->doRender(eng);
   }
};

}

#endif /* QUAD_HPP */
