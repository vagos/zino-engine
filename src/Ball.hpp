#ifndef BALL_HPP_GH9CTJTS
#define BALL_HPP_GH9CTJTS

#include "Object.hpp"

class Ball : public zge::Object
{
public:
    Ball();
    
    void doUpdate(zge::Engine &eng) override;
    void doRender(zge::Engine &eng) override;
    

};

#endif /* BALL_HPP_GH9CTJTS */
