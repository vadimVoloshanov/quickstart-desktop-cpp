#include "../include/offscreen_effect_player.hpp"

#include <libyuv.h>
#include <opencv2/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <iostream>

namespace bnb
{
    offscreen_effect_player::offscreen_effect_player(const std::vector<std::string>& path_to_resources,
            const std::string& client_token, int32_t width, int32_t height, bool manual_audio,
            std::shared_ptr<interfaces::offscreen_render_target> offscreen_render_target)
        : m_utility(path_to_resources, client_token)
        , m_effect_player(bnb::interfaces::effect_player::create( {
            width, height,
            bnb::interfaces::nn_mode::automatically,
            bnb::interfaces::face_search_mode::good,
            false, manual_audio }))
        , m_offscreen_render_target(offscreen_render_target)
        , m_render_thread([this]() { while (!m_cancellation_flag) { m_scheduler.run_all_tasks(); } })
        , m_cancellation_flag(false) {}

    offscreen_effect_player::~offscreen_effect_player()
    {
        m_cancellation_flag = true;
    }

    std::shared_ptr<interfaces::pixel_buffer> offscreen_effect_player::process_image(std::shared_ptr<full_image_t> image)
    {
        auto task = [this, image]() {
            m_offscreen_render_target->prepare_rendering();
            m_effect_player->push_frame(std::move(*image));
            m_effect_player->draw();
        };

        async::spawn(m_scheduler, task);

        // return
    }

    void offscreen_effect_player::load_effect(const std::string& effect_path)
    {
        m_effect_player->effect_manager()->load(effect_path);
    }

    void offscreen_effect_player::unload_effect()
    {
        load_effect("");
    }

    void offscreen_effect_player::pause()
    {
        m_effect_player->playback_pause();
    }

    void offscreen_effect_player::resume()
    {
        m_effect_player->playback_play();
    }

    void offscreen_effect_player::enable_audio(bool enable)
    {
        m_effect_player->enable_audio(enable);
    }

    void offscreen_effect_player::call_js_method(const std::string& method, const std::string& param)
    {
        m_effect_player->effect_manager()->current()->call_js_method(method, param);
    }

} // bnb