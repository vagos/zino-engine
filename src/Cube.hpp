#ifndef CUBE_HPP_FEXYNUDV
#define CUBE_HPP_FEXYNUDV

// Simple cube object. Can be used as a platform.

#include "Object.hpp"

struct Cube : public zge::Object
{

    Cube(zge::Engine& eng);

    void doRender(zge::Engine &eng) override;
    void doUpdate(zge::Engine &eng) override;


};

#endif /* CUBE_HPP_FEXYNUDV */
