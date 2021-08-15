
#include "nametags.h"

c_name_tags::c_name_tags(std::uint32_t playerid, CD3DRender *render, CD3DFont *font)
    : playerid{playerid}, g_render {render}, g_font {font}, text_height {font->DrawHeight()} {
    samp_ped = g_samp.get_samp_ped_by_playerid(playerid);
}

/*void c_name_tags::draw_chat_bubble(std::map<std::uint16_t, c_chat_bubble *> &chat_bubbles)
{
    auto it = chat_bubbles.find(playerid);

    if (it != chat_bubbles.end())
    {
        if (GetTickCount64() > it->second->creation_tick + it->second->life_span) {
            chat_bubbles.erase(it);
        }
        else {
            float width = g_font->DrawLength(it->second->text.c_str());
            g_font->PrintShadow(screen_position.x - width / 2.0f, screen_position.y - text_size - 2, it->second->color, it->second->text.c_str());
        }

    }
}*/

void c_name_tags::draw_bars()
{
    // Health
    g_render->D3DBoxBorder(screen_position.x - 24.0f * g_options.width_multi, screen_position.y + 
        text_height + 4.0f, 100.0f / 2.0f * g_options.width_multi, 6.0f, 0xFF000000, g_options.health_back_color);
    g_render->D3DBoxBorder(screen_position.x - 24.0f * g_options.width_multi, screen_position.y +
        text_height + 4.0f, remote->data->actor_health / 2.0f * g_options.width_multi, 6.0f, 0xFF00000, g_options.health_color);

    // Armor
    if (remote->data->actor_armor > 0.0f)
    {
        g_render->D3DBoxBorder(screen_position.x - 24.0f * g_options.width_multi, screen_position.y + 
            text_height + 12.0f, 100.0f / 2.0f * g_options.width_multi, 6.0f, 0xFF000000, g_options.armor_back_color);
        g_render->D3DBoxBorder(screen_position.x - 24.0f * g_options.width_multi, screen_position.y +
            text_height + 12.0f, remote->data->actor_armor / 2.0f * g_options.width_multi, 6.0f, 0xFF000000, g_options.armor_color);
    }
}

void c_name_tags::draw_label()
{
    char text[128] {0};

    if (remote->data->afk_state && g_options.afk_text) {
        strcat_s(text, "[AFK] ");
    }

    const char *name {g_samp.get_player_name(playerid)};
    strcat_s(text, name);

    if (g_options.white_id) {
        strcat_s(text, "{ffffff}");
    }

    sprintf_s(text, "%s (%d)", text, playerid);

    std::uint32_t color {g_samp.get_player_color(remote->data)};
    float text_width {g_font->DrawLength(text)};

    if (g_options.font_flags & FCR_SHADOW) {
        g_font->PrintShadow(screen_position.x - text_width / 2.0f, screen_position.y, color, text);
    }
    else {
        g_font->Print(text, color, screen_position.x - text_width / 2.0f, screen_position.y, false, false);
    }
}

bool c_name_tags::is_valid(float server_distance)
{
    if (!g_samp.is_player_exist(playerid)) {
        return false;
    }

    s_samp_ped *samp_ped {g_samp.get_samp_ped_by_playerid(playerid)};
    if (!samp_ped->gta_entity->is_on_screen()) {
        return false;
    }

    remote = g_samp.get_info()->pools->players->remote[playerid];
    if (remote->data->hide_name_tag) {
        return false;
    }

    s_vector3 bone_position, local_position {g_game.get_camera_position()};
    samp_ped->gta_ped->get_bone_position(&bone_position, 8, true);

    float distance {local_position.distance(bone_position)};
    if (distance > server_distance) {
        return false;
    }

    bone_position.z = distance * 0.047f * g_options.z_multi + bone_position.z + 0.2f;

    bool line_of_sight {g_game.is_line_of_sight_clear(&local_position, 
        &bone_position, true, true, false, true, true, true, true)};
    if (!line_of_sight) {
        return false;
    }

    screen_position = g_game.world_to_screen(bone_position);
    return true;
}