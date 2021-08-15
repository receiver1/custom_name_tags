#pragma once

#include <string>
#include <fstream>

struct s_options
{
	bool enable {true};
	bool afk_text {true};
	std::uint32_t health_color {0xFFFF0000};
	std::uint32_t armor_color {0xFFFFFFFF};
	std::uint32_t health_back_color {0xFF000000};
	std::uint32_t armor_back_color {0xFF000000};
	std::string font_face {"Arial"};
	int font_size {10};
	std::uint32_t font_flags {FCR_BOLD | FCR_SHADOW};
	float z_multi {1.0f}; // Height of text above player head
	float width_multi {1.0f}; // Multiplier width of bars
	bool white_id {true};

	void load(bool first_load = false)
	{
		// I add try coz, if user will set wrong value of any param, then cause game crash
		try
		{
			std::ifstream file {PROJECT_NAME ".ini"};
			std::string type;
			std::string value;

			if (file.is_open())
			{
				while (file >> type >> value)
				{
					// std::printf("%s: %s\n", type.c_str(), value.c_str());
					if (!type.compare("enable") && first_load) {
						enable = static_cast<bool>(std::stoi(value));
					}
					if (!type.compare("afk_text")) {
						afk_text = static_cast<bool>(std::stoi(value));
					}
					if (!type.compare("health_color")) {
						health_color = static_cast<std::uint32_t>(std::stoul(value, nullptr, 16));
					}
					if(!type.compare("health_back_color")) {
						health_back_color = static_cast<std::uint32_t>(std::stoul(value, nullptr, 16));
					}
					if (!type.compare("armor_color")) {
						armor_color = static_cast<std::uint32_t>(std::stoul(value, nullptr, 16));
					}
					if (!type.compare("armor_back_color")) {
						armor_back_color = static_cast<std::uint32_t>(std::stoul(value, nullptr, 16));
					}
					if (!type.compare("font_face")) {
						font_face = value;
					}
					if (!type.compare("font_size")) {
						font_size = std::stoi(value);
					}
					if (!type.compare("font_flags")) {
						font_flags = std::stoi(value);
					}
					if (!type.compare("z_multi")) {
						z_multi = std::stof(value);
					}
					if (!type.compare("width_multi")) {
						width_multi = std::stof(value);
					}
					if (!type.compare("white_id")) {
						white_id = static_cast<bool>(std::stoi(value));
					}
				}

				file.close();
			}
			else {
				save();
			}
		}
		catch (...) {}
	}

	void save()
	{
		try
		{
			std::ofstream file {PROJECT_NAME ".ini"};
			if (file.is_open())
			{
				file << "enable " << enable << std::endl;
				file << "afk_text " << afk_text << std::endl;
				file << "health_color " << std::hex << std::uppercase << health_color << std::dec << std::nouppercase << std::endl;
				file << "health_back_color " << std::hex << std::uppercase << health_back_color << std::dec << std::nouppercase << std::endl;
				file << "armor_color " << std::hex << std::uppercase << armor_color << std::dec << std::nouppercase << std::endl;
				file << "armor_back_color " << std::hex << std::uppercase << armor_back_color << std::dec << std::nouppercase << std::endl;
				file << "font_face " << font_face << std::endl;
				file << "font_size " << font_size << std::endl;
				file << "font_flags " << font_flags << std::endl;
				file << "z_multi " << z_multi << std::endl;
				file << "width_multi " << width_multi << std::endl;
				file << "white_id " << white_id << std::endl;

				file.close();
			}
		}
		catch (...) {}
	}
};

inline s_options g_options {};