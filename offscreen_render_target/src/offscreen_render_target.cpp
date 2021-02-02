#include "../include/offscreen_render_target.hpp"

#include "../include/opengl.hpp"

namespace bnb
{
    void Offscreen_render_target::init(uint32_t width, uint32_t height)
    {
        m_width = width;
        m_height = height;

        activate_context();
        set_surface_size(width, height);

        // m_program = std::make_unique<bnb::program>("OrientationChange", vs_default_base, ps_default_base);
        // m_frame_surface_handler = std::make_unique<oep_frame_surface_handler>(bnb::camera_orientation::deg_0, false);

        // glGenFramebuffers(1, &m_framebuffer);
        // glGenFramebuffers(1, &m_postProcessingFramebuffer);

        // glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);

        // setupTextureCache();

        // setupOffscreenPixelBuffer();

        // setupOffscreenRenderTarget();
    }

    void Offscreen_render_target::prepare_rendering()
    {
        // glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);
        // glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, CVOpenGLESTextureGetTarget(m_offscreenRenderTexture), CVOpenGLESTextureGetName(m_offscreenRenderTexture), 0);

        // if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        //     GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        //     NSLog(@"Failed to make complete framebuffer object %d", status);
        // }
    }

    void Offscreen_render_target::activate_context()
    {
        glfwMakeContextCurrent(renderer_context.get());

        //need?
        glfwSwapInterval(1);

        //add logic
    }

    void Offscreen_render_target::set_surface_size(uint32_t width, uint32_t height)
    {
        m_width = width;
        m_height = height;
        GL_CALL(glViewport(0, 0, width, height));
    }

    uint32_t Offscreen_render_target::get_active_texture_id()
    {
        return 0;
    }

    data_t Offscreen_render_target::read_current_buffer()
    {
        return data_t();
    }

    //???
    void Offscreen_render_target::show_on_active_texture()
    {
        glfwSwapBuffers(renderer_context.get());
    }
} // bnb