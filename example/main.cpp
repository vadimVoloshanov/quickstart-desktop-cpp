
#include "BanubaClientToken.hpp"

#include "../offscreen_effect_player/include/offscreen_effect_player.hpp"
#include "../offscreen_effect_player/interfaces/offscreen_effect_player.hpp"

#include "renderer/render_thread.hpp"

#include <libyuv.h>

using namespace std::chrono;

//full_image_t
int main()
{
    int32_t width = 1280;
    int32_t height = 720;

    std::unique_ptr<bnb::interfaces::offscreen_effect_player> oep =
            std::make_unique<bnb::offscreen_effect_player>(BNB_CLIENT_TOKEN, width, height, false);
    oep->load_effect("effects/virtual_bg", true);

    std::shared_ptr<GlfwWindow> window = std::make_shared<GlfwWindow>("");
    std::shared_ptr<render::RenderThread> render_t =
            std::make_shared<render::RenderThread>(window->get_window(), width, height);

    auto ef_cb = [&oep, render_t](bnb::full_image_t image) {
        auto call_back = [render_t](bnb::data_t data) {
            render_t->update_data(std::move(data));
            render_t->schedule([render_t]() mutable {
                render_t->update_context();
            });
        };
        auto image_ptr = std::make_shared<bnb::full_image_t>(std::move(image));
        oep->async_process_image(image_ptr, call_back);
    };

    std::shared_ptr<bnb::camera_base> m_camera_ptr = bnb::create_camera_device(ef_cb, 0);

    window->show(width, height);
    window->run_main_loop();

    return 0;
}

//NV12
// int main()
// {
//     int32_t width = 1280;
//     int32_t height = 720;

//     uint8_t* m_y_row_ptr;
//     uint8_t* m_uv_row_ptr;

//     m_y_row_ptr = (uint8_t*) malloc(width * height * 4 * sizeof(uint8_t));
//     m_uv_row_ptr = (uint8_t*) malloc(width * height * 2 * sizeof(uint8_t));

//     bnb::NV12_planes nv12;

//     nv12.y.reset(m_y_row_ptr);
//     nv12.uv.reset(m_uv_row_ptr);

//     std::unique_ptr<bnb::interfaces::offscreen_effect_player> oep =
//             std::make_unique<bnb::offscreen_effect_player>(BNB_CLIENT_TOKEN, width, height, false);
//     oep->load_effect("effects/virtual_bg", true);

//     std::shared_ptr<GlfwWindow> window = std::make_shared<GlfwWindow>("");
//     std::shared_ptr<render::RenderThread> render_t =
//             std::make_shared<render::RenderThread>(window->get_window(), width, height);

//     auto ef_cb = [&oep, render_t, width, height, &nv12](bnb::full_image_t image) {
//         auto call_back = [render_t](bnb::NV12_planes data) {
//             std::cout << "3" << std::endl;
//             render_t->update_data(std::move(data));
//             std::cout << "4" << std::endl;
//             render_t->schedule([render_t]() mutable {
//                 std::cout << "5" << std::endl;
//                 render_t->update_context();
//             });
//         };

//         bnb::image_format frm((uint32_t)width, (uint32_t)height, bnb::camera_orientation::deg_0, false, 0, std::nullopt);

//         const bnb::bpc8_image_t& bpc8 = image.get_data<bnb::bpc8_image_t>();

//         std::cout << "0" << std::endl;

//         uint8_t* yy;
//         uint8_t* uu;

//         yy = (uint8_t*) malloc(width * height * 4 * sizeof(uint8_t));
//         uu = (uint8_t*) malloc(width * height * 2 * sizeof(uint8_t));

//         libyuv::ARGBToNV12(bpc8.get_data(),
//                 width * 4,
//                 yy,
//                 width,
//                 uu,
//                 width,
//                 width,
//                 height);

//         std::cout << "1" << std::endl;
//         oep->async_process_image_NV12(nv12, frm, call_back);
//         std::cout << "2" << std::endl;
//     };

//     std::shared_ptr<bnb::camera_base> m_camera_ptr = bnb::create_camera_device(ef_cb, 0);

//     window->show(width, height);
//     window->run_main_loop();

//     return 0;
// }