#ifndef SHADOWMAPPER_HPP
#define SHADOWMAPPER_HPP

#include "Framebuffer.hpp"
#include "Engine.hpp"
#include "Model.hpp"

namespace zge 
{

struct Shadowmapper 
{
    Framebuffer f_b; // frame buffer 
    Texture d_t;     // depth texture 

    Shadowmapper();

    void doRender(Engine& eng);
    
};

}
#endif /* SHADOWMAPPER_HPP */
