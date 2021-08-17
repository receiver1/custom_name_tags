
#include "hooks.h"

CD3DRender *g_render {new CD3DRender {128}};
CD3DFont *g_font {nullptr};

bool is_render_ready {false};
bool is_timer_initializated {false};

c_call tags_call, bars_call, bubble_call;

HRESULT __stdcall hook::device_reset(IDirect3DDevice9 *device, D3DPRESENT_PARAMETERS *present_params)
{
    if (is_render_ready)
    {
        g_font->Invalidate();
        g_render->Invalidate();

        is_render_ready = false;
    }
    return original::device_reset(device, present_params);
}

HRESULT __stdcall hook::device_present(IDirect3DDevice9 *device, 
    const RECT *src_rect, const RECT *dst_rect, HWND window, const RGNDATA *dirty_region)
{
    if (!is_render_ready)
    {
        g_render->Initialize(device);
        g_font->Initialize(device);

        is_render_ready = true;
    }
    else if (SUCCEEDED(g_render->BeginRender()))
    {
        if (g_options.enable && !g_game.is_on_pause())
         {
             s_server_settings *server_settings = g_samp.get_info()->settings;
             if (server_settings->show_name_tags)
             {
                 const auto p_players {g_samp.get_info()->pools->players};
                 const std::uint32_t max_playerid = p_players->max_playerid;
                 for (std::uint32_t i {0}; i <= max_playerid; ++i)
                 {
                     c_name_tags tag {i, g_render, g_font};
                     if (tag.is_valid(server_settings->name_tags_distance))
                     {
                         tag.draw_label();
                         tag.draw_bars();
                     }
                }
             }
         }

        g_render->EndRender();
    }
    return original::device_present(device, src_rect, dst_rect, window, dirty_region);
}

void hook::tags_draw()
{
    if (g_options.enable) {
        return;
    }
    reinterpret_cast<prototype::void_t>(tags_call.original())();
}

void hook::bars_draw()
{
    if (g_options.enable) {
        return;
    }
    reinterpret_cast<prototype::void_t>(bars_call.original())();
}

void hook::bubbles_draw()
{
    if (g_options.enable) {
        return;
    }
    reinterpret_cast<prototype::void_t>(bubble_call.original())();
}

void hook::timer_update()
{
    if (!is_timer_initializated)
    {
        if (g_samp.is_initializated())
        {
            IDirect3DDevice9 *device {*reinterpret_cast<IDirect3DDevice9 **>(0xC97C28U)};
            if (device != nullptr)
            {
                g_font = new CD3DFont {g_options.font_face.c_str(), g_options.font_size, g_options.font_flags};

                void **device_table {*reinterpret_cast<void ***>(device)};

                MH_CreateHook(device_table[16], &hook::device_reset,
                    reinterpret_cast<void **>(&original::device_reset));
                MH_EnableHook(device_table[16]);

                MH_CreateHook(device_table[17], &hook::device_present,
                    reinterpret_cast<void **>(&original::device_present));
                MH_EnableHook(device_table[17]);

                tags_call.install(g_samp.get_address(0x71195U), &hook::tags_draw);
                bars_call.install(g_samp.get_address(0x71190U), &hook::bars_draw);
#if defined(_DEBUG_MODE)
                bubble_call.install(g_samp.get_address(0x71458U), &hook::bubbles_draw);
#endif
                //bubble_add_call.install(g_samp.get_address(0xCB76U), &bubble_add_hooked);

                g_samp.register_chat_command("customtags", [](const char *args)
                {
                    g_options.enable = !g_options.enable;
                    if (g_options.enable)
                    {
                        g_options.load();

                        // Set new prop's and reinitialize font
                        g_font->set_font_name(g_options.font_face.c_str());
                        g_font->set_font_height(g_options.font_size);
                        g_font->set_font_flags(g_options.font_flags);
                        g_font->Invalidate();
                        is_render_ready = false;
                    }
                });

#if defined(_DEBUG_MODE)
                g_samp.register_chat_command("hidetag", [](const char *args)
                {
                    std::uint32_t i = std::stoi(args);

                    if (!g_samp.is_player_exist(i)) {
                        return;
                    }

                    auto remote {g_samp.get_info()->pools->players->remote[i]};
                    auto &shit {*(uint8_t *)((uintptr_t)remote->data + 0xB3)};

                    std::printf("%d: %d %p\n", i, shit, remote);

                    shit ^= true;
                });
#endif

                is_timer_initializated = true;
            }
        }
    }
    original::timer_update();
}