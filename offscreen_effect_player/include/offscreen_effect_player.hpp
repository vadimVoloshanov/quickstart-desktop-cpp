#pragma once

#include "../interfaces/offscreen_effect_player.hpp"
#include "../../offscreen_render_target/interfaces/offscreen_render_target.hpp"

#include "pixel_buffer.hpp"

namespace bnb
{
    class offscreen_effect_player: public interfaces::offscreen_effect_player
    {
    private:
        offscreen_effect_player(const std::vector<std::string>& path_to_resources,
            const std::string& client_token,
            int32_t width, int32_t height, bool manual_audio,
            std::shared_ptr<interfaces::offscreen_render_target> offscreen_render_target);
    public:
        ~offscreen_effect_player();

        static std::unique_ptr<interfaces::offscreen_render_target> create(
            const std::vector<std::string>& path_to_resources,
            const std::string& client_token,
            int32_t width, int32_t height, bool manual_audio,
            std::shared_ptr<interfaces::offscreen_render_target> offscreen_render_target = nullptr);

        std::shared_ptr<interfaces::pixel_buffer> process_image(std::shared_ptr<full_image_t> image) override;

        void load_effect(const std::string& effect_path) override;
        void unload_effect() override;

        void pause() override;
        void resume() override;

        void enable_audio(bool enable) override;

        void call_js_method(const std::string& method, const std::string& param) override;

    private:
        bnb::utility m_utility;
        std::shared_ptr<interfaces::effect_player> m_effect_player;
        std::shared_ptr<interfaces::offscreen_render_target> m_offscreen_render_target;

        std::atomic<bool> m_cancellation_flag;

        std::thread m_render_thread;
        async::fifo_scheduler m_scheduler;
    };
} // bnb