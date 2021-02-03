#include "../include/pixel_buffer.hpp"

#include <bnb/types/full_image.hpp>

#include <libyuv.h>

namespace bnb
{
    pixel_buffer::pixel_buffer(std::shared_ptr<offscreen_effect_player> oep_ptr, uint32_t width, uint32_t height)
        : m_oep_ptr(oep_ptr)
        , m_width(width)
        , m_height(height) {}

    bool pixel_buffer::get_ready() const
    {
        return m_ready;
    }

    void pixel_buffer::set_ready()
    {
        m_ready = true;
    }

    void pixel_buffer::get_texture_id(std::function<void(uint32_t texture_id)> callback)
    {
        if (auto oep_sp = m_oep_ptr.lock()) {
            oep_sp->get_active_texture_id(callback);
        }

        throw std::runtime_error("Offscreen effect player destroyed");
    }

    void pixel_buffer::get_RGBA(std::function<void(full_image_t image)> callback)
    {
        if (auto oep_sp = m_oep_ptr.lock()) {
            auto convert_callback = [this, callback](data_t data) {
                bnb::image_format frm(m_width, m_height, bnb::camera_orientation::deg_0, false, 0, std::nullopt);
                auto bpc8 = bpc8_image_t(color_plane_weak(data.data.get()), interfaces::pixel_format::rgba, frm);
                callback(full_image_t(std::move(bpc8)));
            };

            oep_sp->read_current_buffer(convert_callback);
        }

        throw std::runtime_error("Offscreen effect player destroyed");
    }

    void pixel_buffer::get_NV12(std::function<void(full_image_t image)> callback)
    {
        if (auto oep_sp = m_oep_ptr.lock()) {
            auto convert_callback = [this, callback](data_t data) {
                bnb::color_plane y_plane = nullptr;
                bnb::color_plane uv_plane = nullptr;

                bnb::image_format frm(m_width, m_height, bnb::camera_orientation::deg_0, false, 0, std::nullopt);

                libyuv::ABGRToNV12(data.data.get(),
                    m_width * 4,
                    y_plane.get(),
                    m_width,
                    uv_plane.get(),
                    m_width,
                    m_width,
                    m_height);

                callback(full_image_t(yuv_image_t(y_plane, uv_plane, frm)));
            };

            oep_sp->read_current_buffer(convert_callback);
        }

        throw std::runtime_error("Offscreen effect player destroyed");
    }
} // bnb