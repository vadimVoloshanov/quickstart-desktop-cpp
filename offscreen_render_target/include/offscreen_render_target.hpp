#pragma once

#include <bnb/types/base_types.hpp>

#include "program.hpp"

#include "../interfaces/offscreen_render_target.hpp"

#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

namespace bnb
{
    class Offscreen_render_target : public interfaces::Offscreen_render_target
    {
    public:
        Offscreen_render_target() = default;

        void init(uint32_t width, uint32_t height) override;

        void prepare_rendering() override;

        //???
        void activate_context() override;

        uint32_t get_active_texture_id() override;
        bnb::data_t read_current_buffer() override;

        //???
        void show_on_active_texture() override;

    private:
        void set_surface_size(uint32_t width, uint32_t height);

        uint32_t m_width;
        uint32_t m_height;

        std::unique_ptr<program> m_program;

        std::shared_ptr<GLFWwindow> renderer_context;
    };
} // bnb