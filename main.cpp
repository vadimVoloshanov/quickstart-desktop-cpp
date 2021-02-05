#include "offscreen_effect_player/include/offscreen_effect_player.hpp"
#include "offscreen_render_target/include/offscreen_render_target.hpp"

#include "glfw_window.hpp"
#include "render_thread.hpp"

#include <bnb/spal/camera/ocv_based.hpp>

#define BNB_CLIENT_TOKEN "WMHjeKZfQ8WJ0Z/zN2TAiTRKfEC8pkAjGbiOTzWJ+EKEs13Mtx58NWTUF6yDH+lup69EPkz0PqE="

int main()
{
    int32_t width = 1280;
    int32_t height = 720;

    std::shared_ptr<bnb::interfaces::offscreen_render_target> ort =
        std::make_shared<bnb::offscreen_render_target>(width, height);

    auto oep = bnb::offscreen_effect_player::create({ BNB_RESOURCES_FOLDER }, BNB_CLIENT_TOKEN,
                                               width, height, false, ort);
    oep->load_effect("effects/bg");

    std::atomic<bool> frame_used = false;


    //add coments. What only for show, only for us
    std::shared_ptr<glfw_window> window = std::make_shared<glfw_window>("");
    std::shared_ptr<render::RenderThread> render_t =
            std::make_shared<render::RenderThread>(window->get_window(), width, height);

    auto ef_cb = [&oep, &frame_used, render_t](bnb::full_image_t image) {
        auto image_ptr = std::make_shared<bnb::full_image_t>(std::move(image));
        auto pixel_bufer = oep->process_image(image_ptr);

        auto callback = [&frame_used, render_t](bnb::full_image_t image) {
            render_t->update_data(std::move(image));
            render_t->schedule([render_t, &frame_used]() mutable {
                render_t->update_context();
                frame_used = true;
            });
        };

        while (!pixel_bufer->get_ready()) {}
        pixel_bufer->get_NV12(callback);
        while (!frame_used) {}
        pixel_bufer.reset();
    };

    std::shared_ptr<bnb::camera_base> m_camera_ptr = bnb::create_camera_device(ef_cb, 0);

    //only for show
    window->show(width, height);
    window->run_main_loop();

    return 0;
}