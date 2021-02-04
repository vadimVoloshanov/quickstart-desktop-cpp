#pragma once

#include <bnb/types/base_types.hpp>

namespace bnb::interfaces
{
    class offscreen_render_target
    {
    public:
        virtual ~offscreen_render_target() = default;

        virtual void activate_context() = 0;

        virtual void prepare_rendering() = 0;

        virtual uint32_t get_active_texture_id() = 0;
        virtual bnb::data_t read_current_buffer() = 0;
    };
} // bnb::interfaces