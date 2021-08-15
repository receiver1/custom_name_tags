#pragma once

#include "vector.hpp"

class c_entity
{
public:
	bool is_on_screen() {
		return reinterpret_cast<bool(__thiscall *)(c_entity *)>(0x534540U)(this);
	}
};

class c_navigation
{
	char padding[0x30];
public:
	s_vector3 position;
};

class c_ped
{
	char padding_00[0x14];
public:
	c_navigation *navigation;
private:
	char padding_01[0x528];
public:
	float health;
	float max_health;
	float armor;


	void get_bone_position(s_vector3 *position, int bone, bool include_animation)
	{
		return reinterpret_cast<void(__thiscall *)(c_ped *, s_vector3 *, int, bool)>(0x5E4280U)(this, position, bone, include_animation);
	}
};

class c_game
{
public:
	bool is_line_of_sight_clear(s_vector3 *begin, s_vector3 *end, bool buildings, bool vehicles, bool peds, bool objects, bool dummies, bool through_stuff, bool ignore_camera_objects)
	{
		return reinterpret_cast<bool(__cdecl *)(s_vector3 *, s_vector3 *, bool, bool, bool, bool, bool, bool, bool)>(0x56A490U)(begin, end, buildings, vehicles, peds, objects, dummies, through_stuff, ignore_camera_objects);
	}

	s_vector3 get_camera_position()
	{
		s_vector3 position;

		position.x = *reinterpret_cast<float *>(0xB6F9CCU);
		position.y = *reinterpret_cast<float *>(0xB6F9D0U);
		position.z = *reinterpret_cast<float *>(0xB6F9D4U);

		return position;
	}

	s_vector2 world_to_screen(s_vector3 world)
	{
		s_vector3 screen_3x;
		float unused[2] {0};

		reinterpret_cast<bool(__cdecl *)(s_vector3 *, s_vector3 *, float *, float *)>(0x71DA00U)(&world, &screen_3x, &unused[0], &unused[1]);

		return {screen_3x.x, screen_3x.y};
	}

	c_ped *get_local_ped()
	{
		return *reinterpret_cast<c_ped **>(0xB6F5F0U);
	}

	bool is_on_pause() {
		return *reinterpret_cast<bool *>(0xBA67A4U);
	}
};

inline c_game g_game {};