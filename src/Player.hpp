#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "Object.hpp"

namespace zge
{


struct Player : public Object 
{
   
    Player(Engine &eng);


    void doRender(Engine &eng) override;
    void doUpdate(Engine &eng) override;

    float speed = 10.0f;

};

}

#endif /* PLAYER_HPP */
