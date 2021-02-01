#include "../include/pixel_buffer.hpp"

namespace bnb
{
    Pixel_buffer::Pixel_buffer(std::shared_ptr<Offscreen_effect_player> oep_ptr)
        : m_oep_ptr(oep_ptr) {}

    uint32_t Pixel_buffer::get_texture_id()
    {
        return 0;
    }

    full_image_t Pixel_buffer::get_RGBA()
    {
        return full_image_t();
    }

    full_image_t Pixel_buffer::get_NV12()
    {
        return full_image_t();
    }
} // bnb