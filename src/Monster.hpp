#ifndef MONSTER_HPP
#define MONSTER_HPP

#include "Engine.hpp"
#include "Object.hpp"
#include "Texture.hpp"

struct Monster : public zge::Object
{
    Monster(zge::Engine& eng);

    void doRender(zge::Engine &eng) override; 
    void doUpdate(zge::Engine &eng) override;

    virtual void doThink(zge::Engine& eng);

    std::shared_ptr<zge::Shader>  shader;
};


#endif /* MONSTER_HPP */
