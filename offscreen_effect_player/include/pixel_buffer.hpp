#pragma once

#include "offscreen_effect_player.hpp"
#include "../interfaces/pixel_buffer.hpp"

namespace bnb
{
    class Offscreen_effect_player;

    class Pixel_buffer: public interfaces::Pixel_buffer
    {
    public:
        Pixel_buffer(std::shared_ptr<Offscreen_effect_player> oep_ptr);

        uint32_t get_texture_id() override;

        full_image_t get_RGBA() override;
        full_image_t get_NV12() override;

    private:
        std::weak_ptr<Offscreen_effect_player> m_oep_ptr;
    };
} // bnb