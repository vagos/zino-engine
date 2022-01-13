#ifndef BALL_HPP_GH9CTJTS
#define BALL_HPP_GH9CTJTS

#include "Object.hpp"

class Monster;

class Ball : public zge::Object
{
public:
    Ball(zge::Engine& eng);
    
    void doUpdate(zge::Engine &eng) override;
    void doRender(zge::Engine &eng) override;

    int n_bounces = 0;

    void spawnMonster(zge::Engine& eng, std::shared_ptr<Monster> monster);

    static std::vector<std::shared_ptr<Monster>> caught_monsters;

};

#endif /* BALL_HPP_GH9CTJTS */
