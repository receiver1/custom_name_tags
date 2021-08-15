#pragma once

#include <windows.h>

#include "../sdk/samp.hpp"
#include "../render/d3drender.h"
#include "../other/options.hpp"

class c_chat_bubble
{
public:
    std::string text {};
    std::uint32_t color {0xFFFFFFFF};
    float draw_distance {0.0f};
    int	life_span {0};
    std::uint64_t creation_tick {0};
};

class c_name_tags
{
    std::uint32_t playerid {0};
    s_remote_player *remote {nullptr};
    s_samp_ped *samp_ped {nullptr};
    s_vector2 screen_position {0.0f, 0.0f};
    CD3DRender *g_render {nullptr};
    CD3DFont *g_font {nullptr};
    float text_height {0.0f};
    std::uint32_t color {0xFFFFFFFF};
public:
    c_name_tags(std::uint32_t playerid, CD3DRender *render, CD3DFont *font);

   // void draw_chat_bubble(std::map<std::uint16_t, c_chat_bubble *> &chat_bubbles);
    void draw_bars();
    void draw_label();
    bool is_valid(float server_distance);
};