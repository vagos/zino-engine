#ifndef RIGIDBODY_HPP_MMT9XDFF
#define RIGIDBODY_HPP_MMT9XDFF

#include <vector>

#include "Common.hpp"


namespace zge 
{

class Engine;

class RigidBody
{

public:

    static const int n_states = 18;

    RigidBody();

    float mass;

    Vector3 position, 
            velocity, angular_velocity, 
            momentum, angular_momentum;
    Matrix3x3 rotation, inverse_inertia;

    std::vector<float> getStateVector() { return state_vector; }

    std::vector<float> state_vector;
    std::vector<float> force_vector;

    virtual void doUpdate(Engine& eng);

    void applyForce(float x, float y, float z);
    void applyForce(Vector3 force_direction);
    void applyGravity();

    void doCollision(Vector3& collision_direction, RigidBody& o_rb);

    void setPosition(Vector3 p) {position = p;}

private:

    void updateStateVector();
    void updateProperties();
    void updateProperties(std::vector<float> new_state_vector);

    std::vector<float> getStateVectorDerivative(const std::vector<float>& state_vector, float t);

    std::vector<float> RungeKuta4(const std::vector<float>& initial_state, float t, float dt);

};

}

#endif /* RIGIDBODY_HPP_MMT9XDFF */
