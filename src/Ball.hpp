#ifndef BALL_HPP_GH9CTJTS
#define BALL_HPP_GH9CTJTS

#include "Object.hpp"

class Ball : public zge::Object
{
public:
    Ball()
    {
        rigid_body = std::make_shared<zge::RigidBody>();
    }

    
    void doUpdate(zge::Engine &eng) override;
    void doRender(zge::Engine &eng) override;
    

};

#endif /* BALL_HPP_GH9CTJTS */
