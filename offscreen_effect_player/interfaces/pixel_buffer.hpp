#pragma once

#include <bnb/types/full_image.hpp>

namespace bnb::interfaces
{
    class Pixel_buffer
    {
    public:
        virtual ~Pixel_buffer() = default;

        virtual uint32_t get_texture_id() = 0;

        virtual full_image_t get_RGBA() = 0;
        virtual full_image_t get_NV12() = 0;

        //TODO: need add support i420 for full_image_t
        // virtual full_image_t get_I420() = 0;
    };
} // bnb::interfaces

