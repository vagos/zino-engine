#ifndef FRAMEBUFFER_HPP
#define FRAMEBUFFER_HPP

#include "Common.hpp"
#include "Texture.hpp"

namespace zge 
{

struct Framebuffer 
{

    Framebuffer() { GLCall(glGenFramebuffers(1, &framebuffer_object)); }
    ~Framebuffer() { GLCall( glDeleteFramebuffers(1, &framebuffer_object) ); }

    unsigned int framebuffer_object;
    void doUse() { GLCall( glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_object) ); }
    void attachTexture(Texture& t);
    void doUnuse() { GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0)); }

};

}
#endif /* FRAMEBUFFER_HPP */
