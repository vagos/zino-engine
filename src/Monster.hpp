#ifndef MONSTER_HPP
#define MONSTER_HPP

#include "Engine.hpp"
#include "Object.hpp"
#include "Texture.hpp"

struct Monster : public zge::Object
{
    Monster(zge::Engine& eng, std::string shader, std::string model, std::string texture);

    virtual void doRender(zge::Engine &eng) override; 
    void doUpdate(zge::Engine &eng) override;

    void doThink(zge::Engine& eng);
    virtual void doAttack(zge::Engine& eng) {}

    std::shared_ptr<zge::Shader>  shader;
    std::shared_ptr<zge::Texture> texture;

    float attack_cooldown = 1.0f;

    zge::Vector3 view_direction;
    float rotation_angle = 0.0f;
};

struct WaterMonster : public Monster 
{
    WaterMonster(zge::Engine& eng); 

    void doAttack(zge::Engine& eng) override;
};

struct FireMonster : public Monster 
{
    FireMonster(zge::Engine& eng); 

    void doAttack(zge::Engine& eng) override;
    void doRender(zge::Engine& eng) override;
};

struct ElectricityMonster : public Monster 
{
    ElectricityMonster(zge::Engine& eng); 

    void doAttack(zge::Engine& eng) override;
    void doRender(zge::Engine& eng) override;
};

#endif /* MONSTER_HPP */
