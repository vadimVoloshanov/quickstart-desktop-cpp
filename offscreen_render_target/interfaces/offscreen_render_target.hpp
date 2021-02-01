#pragma once

#include <bnb/types/base_types.hpp>

namespace bnb::interfaces
{
    class Offscreen_render_target
    {
    public:
        virtual ~Offscreen_render_target() = default;

        virtual void prepare_rendering() = 0;

        //???
        virtual void activate_context() = 0;

        virtual void surface_changed(uint32_t width, uint32_t height) = 0;

        virtual uint32_t get_active_texture_id() = 0;
        virtual bnb::data_t read_current_buffer() = 0;

        //???
        virtual void show_on_active_texture() = 0;
    };
} // bnb::interfaces