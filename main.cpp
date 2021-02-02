#include "offscreen_effect_player/include/offscreen_effect_player.hpp"
#include "offscreen_render_target/include/offscreen_render_target.hpp"

#include <bnb/spal/camera/ocv_based.hpp>

#include <iostream>

#define BNB_CLIENT_TOKEN "WMHjeKZfQ8WJ0Z/zN2TAiTRKfEC8pkAjGbiOTzWJ+EKEs13Mtx58NWTUF6yDH+lup69EPkz0PqE="

int main()
{
    int32_t width = 1280;
    int32_t height = 720;

    std::shared_ptr<bnb::interfaces::Offscreen_render_target> ort =
        std::make_shared<bnb::Offscreen_render_target>();

    auto oep = bnb::Offscreen_effect_player::create({ BNB_RESOURCES_FOLDER }, BNB_CLIENT_TOKEN,
                                               width, height, false, ort);
    oep->load_effect("effects/virtual_bg");

    static std::atomic<bool> frame_used = false;

    auto ef_cb = [&oep](bnb::full_image_t image) {
        auto image_ptr = std::make_shared<bnb::full_image_t>(std::move(image));
        auto pixel_bufer = oep->process_image(image_ptr);

        auto callback = [](bnb::full_image_t image) {
            // render_t->update_data(std::move(image));
            // render_t->schedule([]() mutable {
            //     render_t->update_context();
            // });
            frame_used = true;
        };

        while (!pixel_bufer->get_ready()) {}
        pixel_bufer->get_NV12(callback);
        while (!frame_used) {}
        pixel_bufer.reset();
    };

    std::shared_ptr<bnb::camera_base> m_camera_ptr = bnb::create_camera_device(ef_cb, 0);

    return 0;
}