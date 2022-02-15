#include "RigidBody.hpp"
#include "Common.hpp"
#include "Engine.hpp"
#include <glm/gtx/dual_quaternion.hpp>

namespace zge 
{

RigidBody::RigidBody(): rotation(1.0f), mass(1.0f), position(0.0f), velocity(0.0f), 
    angular_velocity(0.0f), momentum(0.0f), angular_momentum(0.0f), state_vector(n_states, 0.0f), force_vector(6, 0.0f)
{

}

void RigidBody::doCollision(Vector3 &collision_direction, RigidBody &o_rb)
{
    auto v = velocity - 2.0f * glm::dot(velocity, collision_direction) * collision_direction;
    setVelocity(-v);
//    collision_direction = V_UP;
//    setVelocity(-velocity);
}

void RigidBody::applyGravity()
{
    const float g = 10.0f;

    force_vector[1] += -g * mass;
}

void RigidBody::doUpdate(Engine& eng)
{
   updateProperties( RungeKuta4(state_vector, eng.getTime(), eng.getElapsedTime()) );

   force_vector.clear();
   force_vector.resize(6, 0.0f);
}


void RigidBody::updateStateVector()
{
    int k = 0;

    state_vector[k++] = position.x;
    state_vector[k++] = position.y;
    state_vector[k++] = position.z;

    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            state_vector[k++] = rotation[i][j];
        }

    }

    state_vector[k++] = momentum.x;
    state_vector[k++] = momentum.y;
    state_vector[k++] = momentum.z;

    state_vector[k++] = angular_momentum.x;
    state_vector[k++] = angular_momentum.y;
    state_vector[k++] = angular_momentum.z;
}

void RigidBody::updateProperties()
{
    int k = 0;

    position.x = state_vector[k++];
    position.y = state_vector[k++];
    position.z = state_vector[k++];

    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            rotation[i][j] = state_vector[k++];
        }
    }
    
    momentum.x = state_vector[k++];
    momentum.y = state_vector[k++];
    momentum.z = state_vector[k++];

    angular_momentum.x = state_vector[k++];
    angular_momentum.y = state_vector[k++];
    angular_momentum.z = state_vector[k++];

    velocity = momentum / mass;
    
    inverse_inertia = rotation * inverse_inertia * glm::transpose(rotation);

    angular_velocity = inverse_inertia * angular_momentum;
}

std::vector<float> RigidBody::getStateVectorDerivative(const std::vector<float>& state_vector, float dt)
{
    updateStateVector();
    std::vector<float> initial_state = state_vector;
    updateProperties();

    std::vector<float> state_vector_derivative(n_states);

    int k = 0;

    state_vector_derivative[k++] = velocity.x;
    state_vector_derivative[k++] = velocity.y;
    state_vector_derivative[k++] = velocity.z;

    Matrix3x3 angular_velocity_dtat(0.0f, -angular_velocity.z, angular_velocity.y, 
                                   angular_velocity.z, 0.0f, -angular_velocity.x,
                                    -angular_velocity.y, angular_velocity.x, 0.0f);

    Matrix3x3 rotation_derivative = angular_velocity_dtat * rotation;

    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            state_vector_derivative[k++] = rotation_derivative[i][j];
        }
    }

    state_vector_derivative[k++] = force_vector[0];
    state_vector_derivative[k++] = force_vector[1];
    state_vector_derivative[k++] = force_vector[2];

    state_vector_derivative[k++] = force_vector[3];
    state_vector_derivative[k++] = force_vector[4];
    state_vector_derivative[k++] = force_vector[5];

    // Restore initial state
    updateProperties(initial_state);

    return state_vector_derivative;
}

void RigidBody::applyForce(Vector3 force_d)
{
    applyForce(force_d.x, force_d.y, force_d.z);
}

void RigidBody::applyForce(float x, float y, float z)
{
    force_vector[0] += x;
    force_vector[1] += y;
    force_vector[2] += z;
}

void RigidBody::updateProperties(std::vector<float> new_state_vector)
{
    state_vector = new_state_vector;
    updateProperties();
}

std::vector<float> RigidBody::RungeKuta4(const std::vector<float>& state_0, float t, float dt)
{
    std::vector<float> derivative_0 = getStateVectorDerivative(state_0, t);
    
    std::vector<float> state_1(n_states);
    for (int i = 0; i < n_states; i++)
    {
        state_1[i] = state_0[i] + dt * derivative_0[i] / 2.0f;
    }
    std::vector<float> derivative_1 = getStateVectorDerivative(state_1, t + dt / 2.0f);

    std::vector<float> state_2(n_states);
    for (int i = 0; i < n_states; i++)
    {
        state_2[i] = state_1[i] + dt * derivative_1[i] / 2.0f;
    }
    std::vector<float> derivative_2 = getStateVectorDerivative(state_2, t + dt / 2.0f);

    std::vector<float> state_3(n_states);
    for (int i = 0; i < n_states; i++)
    {
        state_3[i] = state_2[i] + dt * derivative_2[i] / 2.0f;
    }
    std::vector<float> derivative_3 = getStateVectorDerivative(state_3, t);

    std::vector<float> state_4(n_states);
    for (int i = 0; i < n_states; i++)
    {
        state_4[i] = state_0[i] + dt * ( derivative_0[i] + derivative_1[i] * 2.0f +derivative_2[i] * 2.0f + derivative_3[i] ) / 6.0f;
    }

    return state_4;
}

} // nameespace zge
