#ifndef PARTICLES_HPP
#define PARTICLES_HPP

#include "Object.hpp"
#include "Quad.hpp"
#include "Shader.hpp"
#include "Texture.hpp"
#include <cstddef>

namespace zge 
{

struct Particle 
{
    Vector3 position, velocity;
    Vector4 color;
    float life;

    Particle(): position(0.0f), velocity(0.0f), color(1.0f), life(0.0f)
    {
    }
};

struct ParticleEmitter : public Object
{
    std::shared_ptr<Shader> shader;
    std::shared_ptr<Texture> texture;

    std::shared_ptr<Model> particle_model;

    std::vector<Particle> particles;
    int n_particles = 100;

    std::size_t i_last_used_particle;

    ParticleEmitter(Engine& eng);

    void doRender(Engine &eng) override;
    void doUpdate(Engine &eng) override;

    std::size_t getUnusedParticle();

    Vector3 position;

};

}

#endif /* PARTICLES_HPP */
