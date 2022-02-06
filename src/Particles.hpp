#ifndef PARTICLES_HPP
#define PARTICLES_HPP

#include "Object.hpp"
#include "Quad.hpp"
#include "Shader.hpp"
#include "Texture.hpp"

#include <cstddef>
#include <functional>

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

    ParticleEmitter(Engine& eng, int n_particles, std::string particle_shader, std::string particle_model, std::string particle_texture = "");

    std::shared_ptr<Shader> shader        = nullptr;
    std::shared_ptr<Texture> texture      = nullptr;
    std::shared_ptr<Model> particle_model = nullptr;

    std::vector<Particle> particles;
    int n_particles = 1;

    float total_time = 10;
    float creation_time;

    virtual void doParticleUpdate(Particle& p, float dt);

    std::size_t i_last_used_particle;

    void doRender(Engine &eng) override;
    void doUpdate(Engine &eng) override;

    std::size_t getUnusedParticle();

    Vector3 position;
};

}

#endif /* PARTICLES_HPP */
