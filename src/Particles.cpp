#include "Particles.hpp"
#include "Engine.hpp"
#include <cstddef>

namespace zge 
{
    ParticleEmitter::ParticleEmitter(Engine& eng)
    {
        particles.resize(n_particles, Particle());
        particle_model = eng_getAssetTyped("Cube Model", Model);
        shader = eng_getAssetTyped("Particle Shader", Shader);
        texture = eng_getAssetTyped("Grass Texture", Texture);
    }

    void ParticleEmitter::doUpdate(Engine &eng)
    {
       // const int n_new_particles = 0;    

       // for (int i = 0; i < n_new_particles; i++)
       // {
       //     int i_unused_p = getUnusedParticle(); 

       //     auto& new_p = particles[i_unused_p];

       // }
    
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
               p.position = position;
               p.velocity = Vector3(eng.getRandomInt(-10, 10), 1, 0);
               p.color = Vector4(1.0, 0.0, 0.0, 1.0);

            }
       }
    }

    void ParticleEmitter::doRender(Engine &eng)
    {
        // GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE));

        shader->doUse();

        for (auto& p : particles)
        {
            auto mvp = eng.camera.getProjection() * eng.camera.getView() * 
                glm::translate(Matrix4x4(1), p.position);
            
            shader->sendUniform("mvp", mvp);
            texture->doUse();
            shader->sendUniform("texture_sampler", *texture);

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
