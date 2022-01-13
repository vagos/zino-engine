#include "Particles.hpp"
#include "Engine.hpp"
#include "Shader.hpp"
#include <cstddef>

#define rnd eng.getRandomFloat() * 2 - 1

namespace zge 
{
    ParticleEmitter:: ParticleEmitter(Engine& eng, int n_particles, std::string particle_shader_n, std::string particle_model_n, std::string particle_texture_n)
        :n_particles(n_particles)
    {
        particles.resize(n_particles, Particle());

        shader = eng_getAssetTyped(particle_shader_n, Shader);
        particle_model = eng_getAssetTyped(particle_model_n, Model);
        if (particle_texture_n.size()) texture = eng_getAssetTyped(particle_texture_n, Texture);

        creation_time = eng.getTime(); 

        std::clog << "PE: " << creation_time << '\n';
    }

    void ParticleEmitter::doUpdate(Engine &eng)
    {
       float dt = eng.getElapsedTime();

       for (auto& p : particles)
       {
            p.life -= dt;

            if (p.life > 0.0f) // particle is alive
            {
               p.position += p.velocity * dt; 
            }
            else
            {
               p.life = total_time;
               p.position = position + Vector3(rnd, rnd, rnd) * 3.0f; 
               p.velocity = Vector3(0, 1, 0);
            }
       }

       total_time -= dt;

       if (total_time < 0) exists = false;
    }

    void ParticleEmitter::doRender(Engine &eng)
    {
        shader->doUse();

        shader->sendUniform("time", eng.getTime() - creation_time);

        for (int i = 0; i < particles.size(); i++)
        {
            auto& p = particles[i];

            auto mvp = eng.camera.getProjection() * eng.camera.getView() * 
                glm::translate(Matrix4x4(1), p.position);

            shader->sendUniform(Uniform_I("mvp", i), mvp);
            // texture->doUse();
            // shader->sendUniform("texture_sampler", *texture);

            // particle_model->doUse();
            // particle_model->doRender(eng);
        }

        particle_model->doUse(); // You need to USE the model before RENDERING
        glDrawArraysInstanced(GL_TRIANGLES, 0, particle_model->vertices.size(), n_particles); 
    }

    std::size_t ParticleEmitter::getUnusedParticle()
    {
        for (auto i = i_last_used_particle; i < particles.size(); i++)
        {
           if (particles[i].life <= 0.0f)
           {
                return i;
           }
        }

        for (auto i = 0; i < particles.size(); i++)
        {
           if (particles[i].life <= 0.0f)
           {
                return i;
           }
        }

        i_last_used_particle = 0;

        return i_last_used_particle;
    }

} // namespace zge

#undef rnd
