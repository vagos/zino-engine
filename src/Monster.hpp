#ifndef MONSTER_HPP
#define MONSTER_HPP

#include "Engine.hpp"
#include "Object.hpp"
#include "Texture.hpp"

struct Monster : public zge::Object
{
    Monster(zge::Engine& eng, std::string shader, std::string model, std::string texture);

    void doRender(zge::Engine &eng) override; 
    void doUpdate(zge::Engine &eng) override;

    void doThink(zge::Engine& eng);
    virtual void doAttack(zge::Engine& eng) {}

    std::shared_ptr<zge::Shader>  shader;
    std::shared_ptr<zge::Texture> texture;
};

struct WaterMonster : public Monster 
{
    WaterMonster(zge::Engine& eng) : Monster(eng, "Water-Texture Shader", "Suzanne Model", "Water Texture")
    {
        std::clog << "water!";
    }

    void doAttack(zge::Engine& eng) override {}

};


#endif /* MONSTER_HPP */
