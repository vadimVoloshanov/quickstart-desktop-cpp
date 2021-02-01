#pragma once

#include <bnb/types/base_types.hpp>

#include "../interfaces/offscreen_render_target.hpp"

namespace bnb
{
    class Offscreen_render_target : public interfaces::Offscreen_render_target
    {
    public:
        Offscreen_render_target() = default;

        void prepare_rendering() override;

        //???
        void activate_context() override;

        void surface_changed(uint32_t width, uint32_t height) override;

        uint32_t get_active_texture_id() override;
        bnb::data_t read_current_buffer() override;

        //???
        void show_on_active_texture() override;
    };
} // bnb