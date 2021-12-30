#include "Particles.hpp"
#include "Engine.hpp"
#include <cstddef>

#define rnd eng.getRandomFloat() * 2 - 1

namespace zge 
{
    ParticleEmitter::ParticleEmitter(Engine& eng)
    {
        particles.resize(n_particles, Particle());
        particle_model = eng_getAssetTyped("Cube Model", Model);
        shader = eng_getAssetTyped("Particle Shader", Shader);
        texture = eng_getAssetTyped("Grass Texture", Texture);

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
               p.life = 10.0f;
               p.position = position + Vector3(rnd, rnd, rnd) * 2.0f; 
               p.velocity = Vector3(0, 1, 0);
               // p.color = Vector4(1.0, 0.0, 0.0, 1.0);
            }
       }

       total_time -= dt;

       if (total_time < 0) exists = false;
    }

    void ParticleEmitter::doRender(Engine &eng)
    {
        // GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE));

        shader->doUse();

        shader->sendUniform("time", eng.getTime() - creation_time);

        for (auto& p : particles)
        {
            auto mvp = eng.camera.getProjection() * eng.camera.getView() * 
                glm::translate(Matrix4x4(1), p.position);

            particle_model->doUse(); // You need to USE the model before RENDERING
            
            shader->sendUniform("mvp", mvp);
            // texture->doUse();
            // shader->sendUniform("texture_sampler", *texture);

            particle_model->doRender(eng);
        }
        
        // GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
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
