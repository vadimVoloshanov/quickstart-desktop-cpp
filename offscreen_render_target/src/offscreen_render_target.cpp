#include "../include/offscreen_render_target.hpp"

#include "../include/opengl.hpp"

#include <bnb/effect_player/utility.hpp>
#include <bnb/postprocess/interfaces/postprocess_helper.hpp>

namespace bnb
{
    offscreen_render_target::offscreen_render_target(uint32_t width, uint32_t height)
        : m_width(width)
        , m_height(height)
    {
        glfwInit();

        create_context();

        glGenFramebuffers(1, &m_framebuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);
    }

    void offscreen_render_target::activate_context()
    {
        glfwMakeContextCurrent(renderer_context.get());
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
    void offscreen_render_target::create_context()
    {
        glfwWindowHint(GLFW_VISIBLE, GLFW_TRUE);

    //     glfwWindowHint(GLFW_DEPTH_BITS, 0);
    //     glfwWindowHint(GLFW_STENCIL_BITS, 0);
    //     glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
    //     glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
    //     glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
    //     glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //     glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);

    // #if BNB_OS_WINDOWS
    //     glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    //     glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // #elif BNB_OS_MACOS || BNB_OS_LINUX
    //     glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    //     glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    // #endif

        renderer_context.reset();
        renderer_context = std::shared_ptr<GLFWwindow>(glfwCreateWindow(m_width, m_height, "", nullptr, nullptr));

        glfwMakeContextCurrent(renderer_context.get());
        load_glad_functions();
        glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
        glfwMakeContextCurrent(nullptr);

        glfwSetWindowSize(renderer_context.get(), m_width, m_height);
        glfwSetWindowPos(renderer_context.get(), 0, 0);
        glfwShowWindow(renderer_context.get());
    }

    void offscreen_render_target::load_glad_functions()
    {
    #if BNB_OS_WINDOWS || BNB_OS_MACOS
        // it's only need for use while working with dynamic libs
        utility::load_glad_functions((GLADloadproc) glfwGetProcAddress);
        bnb::interfaces::postprocess_helper::load_glad_functions(reinterpret_cast<int64_t>(glfwGetProcAddress));
    #endif

        if (0 == gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
            throw std::runtime_error("gladLoadGLLoader error");
        }
    }

    //activate context

    uint32_t offscreen_render_target::get_active_texture_id()
    {
        return texture;
    }

    data_t offscreen_render_target::read_current_buffer()
    {
        activate_context();
        size_t size = m_width * m_height * 4;
        data_t data = data_t{ std::make_unique<uint8_t[]>(size), size };
        // glfwSwapBuffers(renderer_context.get());

        glBindTexture(GL_TEXTURE_2D, texture);
        glReadPixels(0, 0, m_width, m_height, GL_BGRA, GL_UNSIGNED_BYTE, data.data.get());
        //glGetTexImage(GL_TEXTURE_2D, 0, GL_BGRA, GL_UNSIGNED_BYTE, data.data.get());

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glDeleteFramebuffers(1, &m_framebuffer);

        return data;
    }
} // bnb