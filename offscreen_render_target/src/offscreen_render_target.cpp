#include "../include/offscreen_render_target.hpp"

#include "../include/opengl.hpp"

namespace bnb
{
    void offscreen_render_target::init(uint32_t width, uint32_t height)
    {
        std::cout << "ORT init" << std:: endl;

        m_width = width;
        m_height = height;

        std::cout << "ORT activate_context" << std:: endl;
        activate_context();
        std::cout << "ORT gen and bind frame bufer" << std:: endl;

        // glGenFramebuffers(1, &m_framebuffer);
        // glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);
        std::cout << "ORT init finish" << std:: endl;
    }

    void offscreen_render_target::prepare_rendering()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);

        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,  m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    }

    //create_context
    void offscreen_render_target::activate_context()
    {
        glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
        renderer_context.reset();
        renderer_context = std::shared_ptr<GLFWwindow>(glfwCreateWindow(m_width, m_height, "", nullptr, nullptr));

        glfwMakeContextCurrent(renderer_context.get());
    }

    //activate context

    uint32_t offscreen_render_target::get_active_texture_id()
    {
        return texture;
    }

    data_t offscreen_render_target::read_current_buffer()
    {
        size_t size = m_width * m_height * 4;
        data_t data = data_t{ std::make_unique<uint8_t[]>(size), size };
        glGetTexImage(GL_TEXTURE_2D, 0, GL_BGRA, GL_UNSIGNED_BYTE, data.data.get());

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glDeleteFramebuffers(1, &m_framebuffer);

        return data;
    }
} // bnb