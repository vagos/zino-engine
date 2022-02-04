#include "Framebuffer.hpp"
#include "Engine.hpp"

namespace zge 
{
    
    void Framebuffer::attachTexture(Texture &t)
    {
    
        GLCall(glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_object));

        GLCall(glBindTexture(GL_TEXTURE_2D, t.texture_id));
        GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, Engine::width, Engine::height, 0,
		GL_DEPTH_COMPONENT, GL_FLOAT, NULL));

        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

        float border_color[] = { 1.0f, 1.0f, 1.0f, 1.0f };
        GLCall(glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border_color));

        // Attaching the texture to the framebuffer, so that it will monitor the depth component
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, t.texture_id, 0);

        // Since the depth buffer is only for the generation of the depth texture,
        // there is no need to have a color output
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) 
        {
            glfwTerminate();
            throw std::runtime_error("Frame buffer not initialized correctly.");
        }

        glBindFramebuffer(GL_FRAMEBUFFER, 0); // Bind the default framebuffer.
    }



} // namespace zge 
