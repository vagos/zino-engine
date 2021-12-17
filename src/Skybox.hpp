#ifndef SKYBOX_HPP
#define SKYBOX_HPP

#include "Object.hpp"
#include "Texture.hpp"
#include "Common.hpp"
#include "Engine.hpp"

namespace zge
{

struct Skybox : public Object
{
    Shader s_shader;
    Skybox();
    void doRender(zge::Engine &eng) override;
};

} // namespace zge

#endif /* SKYBOX_HPP */
