#pragma once

#include "../interfaces/pixel_buffer.hpp"

#include <BanubaSdkManager.hpp>

namespace bnb
{
    class pixel_buffer: public interfaces::pixel_buffer
    {
    public:
        uint32_t get_texture_id() override;

        full_image_t get_RGBA() override;
        full_image_t get_NV12() override;
    };
} // bnb
