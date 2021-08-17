#pragma once

#include <cstdint>

#include "game.hpp"

#pragma pack(push, 1)

struct s_server_settings
{
	uint8_t byteCJWalk;
	uint8_t byteUnk0[4];
	float fWorldBoundaries[4];
	uint8_t byteUnk1;
	float fGravity;
	uint8_t byteDisableInteriorEnterExits;
	unsigned int ulVehicleFriendlyFire;
	uint8_t byteUnk2[4];
	int iClassesAvailable;
	float name_tags_distance;
	uint8_t byteUnk3;
	uint8_t byteWorldTime_Hour;
	uint8_t byteWorldTime_Minute;
	uint8_t byteWeather;
	uint8_t byteNoNametagsBehindWalls;
	uint8_t bytePlayerMarkersMode;
	uint8_t byteUnk4[3];
	float fGlobalChatRadiusLimit;
	uint8_t	show_name_tags;
};

struct s_samp_ped
{
	char padding_00[0x40];
	c_entity *gta_entity;
	char padding_01[0x260];
	c_ped *gta_ped;
};

struct s_remote_player_data
{
	s_samp_ped *samp_ped;
	char padding_01[0xAF];
	bool show_name_tag; // 0xB3
	char padding_02[0x104];
	float actor_armor; // 0x1B8
	float actor_health;
	char padding_03[0x11];
	int afk_state;
};

struct s_remote_player 
{
	s_remote_player_data *data;
	int	is_npc;
	void *text_handler;
	union
	{
		char name[16];
		char *p_name;
	};
	int name_length;
	int	name_allocated;
	int	score;
	int	ping;
};

struct s_player_pool
{
	uint32_t max_playerid;
	uint16_t local_playerid;
	void *text_handler;
	union 
	{
		char local_name[16];
		char *p_local_name;
	};
	int local_name_length;
	int	local_name_allocated;
	void *local_player;
	int	local_ping;
	int	local_score;
	s_remote_player *remote[1004];
	int	is_listed[1004];
};

struct s_pools
{
	void *actors;
	void *objects;
	void *gangzones;
	void *labels_3d;
	void *textdraws;
	void *player_labels;
	s_player_pool *players;
	void *vehicles;
	void *pickups;
};

struct s_samp_info
{
	void *unknown;
	void *server_info;
	uint8_t padding[24];
	char ip[257];
	char hostname[259];
	bool nametag_status;
	uint32_t port;
	uint32_t map_icons[100];
	int lan_mode;
	int	game_state;
	uint32_t connection_ticks;
	s_server_settings *settings;
	void *rak_client;
	s_pools *pools;
};

#pragma pack(pop)

using cmd_proc_t = void(__cdecl *)(const char *);
class c_samp
{
	unsigned long base_address {0};
	s_samp_info *samp_info;
	class c_samp_input *samp_input {nullptr};
public:
	unsigned long get_address(unsigned long offset)
	{
		return base_address + offset;
	}

	bool is_initializated()
	{
		base_address = reinterpret_cast<unsigned long>(GetModuleHandleA("samp.dll"));
		if (!base_address) return false;

		samp_info = *reinterpret_cast<s_samp_info **>(base_address + 0x21A0F8U);
		if (!samp_info) return false;

		samp_input = *reinterpret_cast<c_samp_input **>(base_address + 0x21A0E8U);
		if (!samp_input) return false;

		return true;
	}

	s_samp_info *get_info() {
		return samp_info;
	}

	const char *get_player_name(int iPlayerID)
	{
		const auto p_players = get_info()->pools->players;
		if (p_players == NULL || iPlayerID < 0 || iPlayerID > 1004)
			return NULL;

		if (iPlayerID < 0 || iPlayerID > 1004)
			return NULL;

		if (iPlayerID == p_players->local_playerid)
		{
			if (p_players->local_name_allocated <= 0xF)
				return p_players->local_name;
			return p_players->p_local_name;
		}

		if (p_players->remote[iPlayerID] == NULL)
			return NULL;

		if (p_players->remote[iPlayerID]->name_allocated <= 0xF)
			return p_players->remote[iPlayerID]->name;

		return p_players->remote[iPlayerID]->p_name;
	}

	std::uint32_t get_player_color(s_remote_player_data *remote_data) {
		return reinterpret_cast<std::uint32_t(__thiscall *)(s_remote_player_data *)>(base_address + 0x12A00)(remote_data);
	}
	
	double get_actor_health(s_samp_ped *samp_ped) {
		return reinterpret_cast<double(__thiscall *)
			(s_samp_ped *)>(base_address + 0x97F50)(samp_ped);
	}

	bool is_player_exist(std::uint32_t playerid) {
		return (samp_info->pools->players->remote[playerid]
			&& samp_info->pools->players->remote[playerid]->data
			&& samp_info->pools->players->remote[playerid]->data->samp_ped
			&& samp_info->pools->players->remote[playerid]->data->samp_ped->gta_ped);
	}

	s_samp_ped *get_samp_ped_by_playerid(int playerid)
	{
		const auto p_players = get_info()->pools->players;
		if (!p_players->is_listed[playerid] || !p_players->remote[playerid])
			return nullptr;

		if (!p_players->remote[playerid]->data)
			return nullptr;

		return p_players->remote[playerid]->data->samp_ped;
	}

	void register_chat_command(const char *command, cmd_proc_t function) {
		reinterpret_cast<void(__thiscall *)(c_samp_input *, const char *, cmd_proc_t)>
			(base_address + 0x65AD0U)(samp_input, command, function);
	}
};

inline c_samp g_samp {};