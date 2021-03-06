#include "BanubaSdkManager.hpp"

#include <bnb/effect_player/utility.hpp>
#include <bnb/spal/camera/base.hpp>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

#include <chrono>
#include <thread>

using namespace std::chrono_literals;

BanubaSdkManager::BanubaSdkManager(
    const std::string& window_title,
    const std::vector<std::string>& paths_to_resources,
    const std::string& client_token)
    : m_utility(paths_to_resources, client_token)
    , m_window(window_title)
    , m_effect_player(bnb::interfaces::effect_player::create({
          720 /*fx_width*/,
          1280 /*fx_height*/,
          bnb::interfaces::nn_mode::automatically /*nn_enable*/,
          bnb::interfaces::face_search_mode::good /*face_search*/,
          false /*js_debugger_enable*/,
          false /*manual_audio*/
      }))
    , m_window_is_shown(false)
{
    glfwSetWindowUserPointer(m_window.get_window(), this);
    // Window size changed
    glfwSetFramebufferSizeCallback(m_window.get_window(), [](GLFWwindow* window, int width, int height) {
        auto sdk = static_cast<BanubaSdkManager*>(glfwGetWindowUserPointer(window));
        sdk->m_render_thread->update_surface_size(width, height);
    });
    // Pause on minimizing
    glfwSetWindowIconifyCallback(m_window.get_window(), [](GLFWwindow* window, int iconify) {
        auto sdk = static_cast<BanubaSdkManager*>(glfwGetWindowUserPointer(window));
        if (iconify == GLFW_TRUE)
            sdk->m_effect_player->playback_pause();
        else
            sdk->m_effect_player->playback_play();
    });
    start_render_thread();
}

BanubaSdkManager::~BanubaSdkManager()
{
}

void BanubaSdkManager::load_effect(const std::string& effectPath, bool synchronous)
{
    if (synchronous) {
        m_render_thread->schedule([this, effectPath]() {
            m_effect_player->effect_manager()->load(effectPath);
        });
    } else {
        m_effect_player->effect_manager()->load_async(effectPath);
    }
}

void BanubaSdkManager::process_image(const path& path)
{
    m_render_thread->schedule([this, path]() {
                       auto name = path.filename().string();
                       auto img = bnb::full_image_t::load(path.string());
                       auto fmt = img.get_format();
                       auto result_img = m_effect_player->process_image(
                           std::move(img),
                           bnb::interfaces::pixel_format::rgba,
                           bnb::interfaces::process_image_params(false, std::nullopt, {}));
                       stbi_write_jpg(name.c_str(), fmt.width, fmt.height, 4, result_img.data.get(), 90);
                   })
        .wait();
}

bnb::data_t BanubaSdkManager::sync_process_frame(std::shared_ptr<bnb::full_image_t> image)
{
    bool ready = false;
    const auto& format = image.get()->get_format();
    m_render_thread->schedule([this, image, &ready, &format]() {
        if (last_frame_size.first != format.width || last_frame_size.second != format.height) {
            m_render_thread->update_surface_size(format.width, format.height);
            last_frame_size.first = format.width;
            last_frame_size.second = format.height;
        }
        m_effect_player->push_frame(std::move(*image));
        auto read_pixels_callback = [&ready]() {
            ready = true;
        };
        m_render_thread->add_read_pixels_callback(read_pixels_callback);
    });

    while (!ready)
    {
        std::this_thread::sleep_for(1us);
    }

    return m_effect_player->read_pixels(format.width, format.height);
}

void BanubaSdkManager::async_process_frame(std::shared_ptr<bnb::full_image_t> image, std::function<void(bnb::data_t data)> callback)
{
    m_render_thread->schedule([this, image, callback]() {
        const auto& format = image.get()->get_format();
        if (last_frame_size.first != format.width || last_frame_size.second != format.height) {
            m_render_thread->update_surface_size(format.width, format.height);
            last_frame_size.first = format.width;
            last_frame_size.second = format.height;
        }
        m_effect_player->push_frame(std::move(*image));

        auto read_pixels_callback = [this, format, callback]() {
            callback(m_effect_player->read_pixels(format.width, format.height));
        };
        m_render_thread->add_read_pixels_callback(read_pixels_callback);
    });
}

void BanubaSdkManager::process_camera(int camera_id)
{
    // Callback for new camera image
    auto ef_cb = [this](bnb::full_image_t image) {
        if (!m_window_is_shown) {
            const auto& format = image.get_format();
            m_window.show(format.width, format.height);
            m_render_thread->update_surface_size(format.width, format.height);
            m_window_is_shown = true;
        }
        m_effect_player->push_frame(std::move(image));
    };
    m_camera_ptr = bnb::create_camera_device(ef_cb, camera_id);
}

void BanubaSdkManager::start_render_thread()
{
    m_render_thread = std::make_unique<RenderThread>(m_window.get_window(), *m_effect_player);
}