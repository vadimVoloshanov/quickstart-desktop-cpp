#pragma once

#include "offscreen_effect_player.hpp"
#include "../../interfaces/pixel_buffer.hpp"

namespace bnb
{
    class offscreen_effect_player;

    class pixel_buffer: public interfaces::pixel_buffer
    {
    public:
        pixel_buffer(std::shared_ptr<offscreen_effect_player> oep_ptr, uint32_t width, uint32_t height);

        bool get_ready() const override;
        void set_ready() override;

        void get_texture_id(std::function<void(uint32_t texture_id)> callback) override;

        void get_RGBA(std::function<void(full_image_t image)> callback) override;
        void get_NV12(std::function<void(full_image_t image)> callback) override;

    private:
        std::weak_ptr<offscreen_effect_player> m_oep_ptr;

        uint32_t m_width = 0;
        uint32_t m_height = 0;

        bool m_ready = false;
    };
} // bnb