#ifndef BALL_HPP_GH9CTJTS
#define BALL_HPP_GH9CTJTS

#include "Object.hpp"

class Ball : public zge::Object
{
public:
    Ball(zge::Engine& eng);
    
    void doUpdate(zge::Engine &eng) override;
    void doRender(zge::Engine &eng) override;

    int n_bounces = 0;

};

#endif /* BALL_HPP_GH9CTJTS */
