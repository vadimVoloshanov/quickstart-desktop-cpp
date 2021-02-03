#pragma once

#include <bnb/types/full_image.hpp>

namespace bnb::interfaces
{
    class pixel_buffer
    {
    public:
        virtual ~pixel_buffer() = default;

        virtual bool get_ready() const = 0;
        virtual void set_ready() = 0;

        virtual void get_texture_id(std::function<void(uint32_t texture_id)> callback) = 0;

        virtual void get_RGBA(std::function<void(full_image_t image)> callback) = 0;
        virtual void get_NV12(std::function<void(full_image_t image)> callback) = 0;

        //TODO: need add support i420 for full_image_t
        // virtual full_image_t get_I420() = 0;
    };
} // bnb::interfaces

