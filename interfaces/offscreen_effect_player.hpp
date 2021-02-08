#pragma once

#include <bnb/types/full_image.hpp>

#include "pixel_buffer.hpp"

namespace bnb::interfaces
{
    class offscreen_effect_player
    {
    public:
        virtual ~offscreen_effect_player() = default;

        virtual void process_image_async(std::shared_ptr<full_image_t> image,
                std::function<void(std::shared_ptr<interfaces::pixel_buffer> callback)>) = 0;
        // virtual std::shared_ptr<interfaces::pixel_buffer> process_image_sync(std::shared_ptr<full_image_t> image) = 0;

        virtual void load_effect(const std::string& effect_path) = 0;
        virtual void unload_effect() = 0;

        virtual void pause() = 0;
        virtual void resume() = 0;

        virtual void enable_audio(bool enable) = 0;

        virtual void call_js_method(const std::string& method, const std::string& param) = 0;
    };
} // bnb::interfaces