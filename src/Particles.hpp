#ifndef PARTICLES_HPP
#define PARTICLES_HPP

#include "Common.hpp"
#include "Engine.hpp"
#include "Object.hpp"
#include "Quad.hpp"
#include "Shader.hpp"
#include "Texture.hpp"

#include <cstddef>
#include <functional>
#include <glm/gtx/dual_quaternion.hpp>

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

    virtual void doParticleUpdate(Engine& eng, Particle& p, float dt);
    virtual void doParticleInit(Engine& eng, Particle& p);

    std::size_t i_last_used_particle;

    void doRender(Engine &eng) override;
    void doUpdate(Engine &eng) override;

    void doBasicRender(Engine& eng, Shader& shader) override;

    std::size_t getUnusedParticle();

    Vector3 position;
};

struct StarParticleEmitter : public ParticleEmitter
{
    StarParticleEmitter(Engine& eng, int n_particles, std::string particle_shader, std::string particle_model, std::string particle_texture="")
        :ParticleEmitter(eng, n_particles, particle_shader, particle_model, particle_texture)
    {
    }

    void doParticleUpdate(Engine& eng, Particle& p, float dt) override
    {
        p.position += p.velocity * dt;
    }
    
    void doParticleInit(Engine& eng, Particle& p) override
    {
        p.life = 5.0f;

        p.velocity = Vector3(
                ( 2.0f * eng.getRandomFloat() - 1.0f) * 5.0f,
                1.0f, 
                ( 2.0f * eng.getRandomFloat() - 1.0f) * 5.0f
                );

        p.position = position + 
            Vector3( ( 2.0f * eng.getRandomFloat() - 1.0f) * 2.0f,
                     ( 2.0f * eng.getRandomFloat() - 1.0f) * 2.0f,
                     ( 2.0f * eng.getRandomFloat() - 1.0f) * 2.0f
                    );
    }
};

struct SmokeParticleEmitter : public ParticleEmitter
{
    SmokeParticleEmitter(Engine& eng, int n_particles, std::string particle_shader, std::string particle_model, std::string particle_texture="")
        :ParticleEmitter(eng, n_particles, particle_shader, particle_model, particle_texture)
    {
    }

    void doParticleUpdate(Engine& eng, Particle& p, float dt) override
    {
        p.position += p.velocity * dt;
    }
    
    void doParticleInit(Engine& eng, Particle& p) override
    {
        p.life = eng.getRandomFloat(1.0f, 5.0f);
        p.velocity = Vector3(
                ( 2.0f * eng.getRandomFloat() - 1.0f) * 5.0f,
                1.0f, 
                ( 2.0f * eng.getRandomFloat() - 1.0f) * 5.0f
                );

        p.position = position + 
            Vector3( ( 2.0f * eng.getRandomFloat() - 1.0f) * 2.0f,
                     ( 2.0f * eng.getRandomFloat() - 1.0f) * 2.0f,
                     ( 2.0f * eng.getRandomFloat() - 1.0f) * 2.0f
                    );
    }
};

struct FireParticleEmitter : public ParticleEmitter
{
    Vector3 aim_direction;

    FireParticleEmitter(Engine& eng, int n_particles, std::string particle_shader, std::string particle_model, std::string particle_texture="")
        :ParticleEmitter(eng, n_particles, particle_shader, particle_model, particle_texture)
    {
    }

    void doParticleUpdate(Engine& eng, Particle& p, float dt) override
    {
        Vector3 acceleration = Vector3(0.0f, -1.0f, 0.0f) * 9.8f;
        p.position += p.velocity*dt + acceleration * (dt * dt) / 2.0f;
        p.velocity = p.velocity + acceleration * dt;
    }
    
    void doParticleInit(Engine& eng, Particle& p) override
    {
        float k = 1.0f, f = 10.0f;

        aim_direction = - glm::normalize(eng.camera.view_direction) * 10.0f;

        p.life = 5.0f;

        p.velocity = aim_direction + Vector3(
                ( 2.0f * eng.getRandomFloat() - 1.0f) * k,
                1.0f * f, 
                ( 2.0f * eng.getRandomFloat() - 1.0f) * k
                );

        p.position = position +
            Vector3( ( 2.0f * eng.getRandomFloat() - 1.0f) * k,
                     ( 2.0f * eng.getRandomFloat() - 1.0f) * k,
                     ( 2.0f * eng.getRandomFloat() - 1.0f) * k
                    )
            + p.velocity * eng.getRandomFloat();
    }
};


}

#endif /* PARTICLES_HPP */
