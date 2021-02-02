#include "../include/offscreen_effect_player.hpp"

#include "../../offscreen_render_target/include/offscreen_render_target.hpp"

namespace bnb
{
    std::unique_ptr<interfaces::Offscreen_effect_player> Offscreen_effect_player::create(
        const std::vector<std::string>& path_to_resources, const std::string& client_token,
        int32_t width, int32_t height, bool manual_audio,
        std::shared_ptr<interfaces::Offscreen_render_target> ort)
    {
        if (ort) {
            ort = std::shared_ptr<Offscreen_render_target>();
        }

        return std::unique_ptr<Offscreen_effect_player>(new Offscreen_effect_player(
                path_to_resources, client_token, width, height, manual_audio, ort));
    }

    Offscreen_effect_player::Offscreen_effect_player(
        const std::vector<std::string>& path_to_resources, const std::string& client_token,
        int32_t width, int32_t height, bool manual_audio,
        std::shared_ptr<interfaces::Offscreen_render_target> offscreen_render_target)
            : m_utility(path_to_resources, client_token)
            , m_ep(bnb::interfaces::effect_player::create( {
                width, height,
                bnb::interfaces::nn_mode::automatically,
                bnb::interfaces::face_search_mode::good,
                false, manual_audio }))
            , m_ort(offscreen_render_target)
            , m_render_thread([this]() { while (!m_cancellation_flag) { m_scheduler.run_all_tasks(); } })
            , m_cancellation_flag(false)
    {
        auto task = [this, width, height]() {
            m_ep->surface_created(720, 1280);
            m_ort->init(width, height);
        };

        async::spawn(m_scheduler, task);
    }

    Offscreen_effect_player::~Offscreen_effect_player()
    {
        m_cancellation_flag = true;
    }

    std::shared_ptr<interfaces::Pixel_buffer> Offscreen_effect_player::process_image(std::shared_ptr<full_image_t> image)
    {
        if (current_frame.use_count() > 1) {
            throw std::runtime_error("The interface for processing the previous frame is not destroyed");
        }
        current_frame.reset();
        current_frame = std::make_shared<Pixel_buffer>(shared_from_this());

        auto task = [this, image]() {
            m_ort->prepare_rendering();
            m_ep->push_frame(std::move(*image));
            m_ep->draw();
            current_frame->set_ready();
        };

        async::spawn(m_scheduler, task);

        return current_frame;
    }

    void Offscreen_effect_player::load_effect(const std::string& effect_path)
    {
        auto task = [this, &effect_path]() {
            m_ep->effect_manager()->load(effect_path);
        };

        async::spawn(m_scheduler, task);
    }

    void Offscreen_effect_player::unload_effect()
    {
        load_effect("");
    }

    void Offscreen_effect_player::pause()
    {
        m_ep->playback_pause();
    }

    void Offscreen_effect_player::resume()
    {
        m_ep->playback_play();
    }

    void Offscreen_effect_player::enable_audio(bool enable)
    {
        m_ep->enable_audio(enable);
    }

    void Offscreen_effect_player::call_js_method(const std::string& method, const std::string& param)
    {
        m_ep->effect_manager()->current()->call_js_method(method, param);
    }

    void Offscreen_effect_player::get_active_texture_id(std::function<void(uint32_t texture_id)> callback)
    {
        auto task = [this, callback]() {
            callback(m_ort->get_active_texture_id());
        };

        async::spawn(m_scheduler, task);
    }

    void Offscreen_effect_player::read_current_buffer(std::function<void(bnb::data_t data)> callback)
    {
        auto task = [this, callback]() {
            callback(m_ort->read_current_buffer());
        };

        async::spawn(m_scheduler, task);
    }

} // bnb