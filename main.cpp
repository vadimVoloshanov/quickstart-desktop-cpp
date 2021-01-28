#include "offscreen_effect_player/include/offscreen_effect_player.hpp"

#include "offscreen_render_target/interfaces/offscreen_render_target.hpp"

#include <bnb/spal/camera/ocv_based.hpp>

#define BNB_CLIENT_TOKEN "WMHjeKZfQ8WJ0Z/zN2TAiTRKfEC8pkAjGbiOTzWJ+EKEs13Mtx58NWTUF6yDH+lup69EPkz0PqE="

template<typename F>
auto schedule(const F& f, async::fifo_scheduler scheduler)
{
    return async::spawn(scheduler, f);
}

int main()
{
    int32_t width = 1280;
    int32_t height = 720;

    auto oep = offscreen_effect_player::create(BNB_CLIENT_TOKEN, width, height, false);
    oep->load_effect("effects/virtual_bg");

    // std::shared_ptr<GlfwWindow> window = std::make_shared<GlfwWindow>("");
    // std::shared_ptr<render::RenderThread> render_t =
    //         std::make_shared<render::RenderThread>(window->get_window(), width, height);

    // auto ef_cb = [&oep, render_t](bnb::full_image_t image) {
    //     auto call_back = [render_t](bnb::data_t data) {
    //         render_t->update_data(std::move(data));
    //         render_t->schedule([render_t]() mutable {
    //             render_t->update_context();
    //         });
    //     };
    //     auto image_ptr = std::make_shared<bnb::full_image_t>(std::move(image));
    //     oep->async_process_image(image_ptr, call_back);
    // };

    // std::shared_ptr<bnb::camera_base> m_camera_ptr = bnb::create_camera_device(ef_cb, 0);

    // window->show(width, height);
    // window->run_main_loop();

    return 0;
}

// 1. Создаём объект планировщика.
// 2. Создаём поток и запускаем там выполнение тасок поступающих в планировщик.
// 3. Создаём указатель на interface::offscreen_render_target инициализируя либо своим
// дефолтным классом, либо пользовательским.
// 4. Создаём указатель на oep, предоставляю пользователю только интерфейс для вызовов, инициализуруем
// передавая указатель на offscreen_render_target, путь к эффектам, токен, высоту, ширину. Содержит
// в себе указатель на offscreen_render_target и effect_player.
// 5. Создаём коллбек для камеры, который будет принимать full_image_t от камеры и процессить
// фрейм дальше в oep.
// 6. Добавляем в палнировщик таску с вызывом
//     a) у offscreen_render_target prepareRendering() который подготавливает активирует контекст
// и байндит фрейм буфер.
//     b) process_image для oep, который вызывает push_frame у эффект плеера.
//     c) вызываем у oep get_texture_id()/get_current_buffer()/get_I420_planes()/get_NV12_planes() в
//     зависимости от целей.