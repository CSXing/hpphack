#include "framework.h"

enum PlayersFeaturesList { PLAYER_ESP_GLOBALS, PLAYER_ESP_PLAYER, PLAYER_ESP_GLOW, PLAYER_ESP_SOUND, PLAYER_ESP_OUT_OF_FOV, PLAYER_ESP_RADAR };
enum ColoredModelsFeaturesList { COLORED_MODELS_GLOBALS, COLORED_MODELS_PLAYERS, COLORED_MODELS_BACKTRACK, COLORED_MODELS_HANDS, COLORED_MODELS_DLIGHT, COLORED_MODELS_ELIGHT };
enum OtherFeaturesList { OTHER_ESP_WORLD, OTHER_ESP_LOCAL };
enum EffectsFeaturesList { EFFECTS_EFFECTS, EFFECTS_REMOVALS };

static const char* PlayersFeatures[] = { "GLOBALS", "PLAYER", "GLOW", "SOUND", "OUT OF FOV", "RADAR" };
static const char* ColoredModelsFeatures[] = { "GLOBALS", "PLAYERS", "BACKTRACK", "HANDS", "DLIGHT", "ELIGHT" };
static const char* OtherFeatures[] = { "WORLD", "LOCAL" };
static const char* EffectsFeatures[] = { "EFFECTS", "REMOVALS" };

namespace Players
{
	static void DrawGlobals(const std::unique_ptr<CMenuGui>& m_pGui)
	{
		m_pGui->SetID("##visuals_players_globals");
		//m_pGui->Checkbox(&cvars::visuals.esp, "Enabled", u8"");
		//m_pGui->Key(&cvars::visuals.esp_key, "Player ESP key");
		//m_pGui->PushCondition(cvars::visuals.esp);
		//m_pGui->Checkbox(&cvars::visuals.esp_dormant, "Dormant", u8"");
		//m_pGui->PushCondition(cvars::visuals.esp_dormant);
		m_pGui->Slider(&cvars::visuals.esp_dormant_time, "Dormant time", 0.f, 5.f, "%.1f sec", 1.f, u8"ENG: Display time of dormant player in the last position.\nRUS: ����� ����������� ��������������� ������ � ��������� �������.");
		//m_pGui->PushCondition(cvars::visuals.esp_dormant_time);
		m_pGui->Checkbox(&cvars::visuals.esp_dormant_fadeout, "Dormant fade out", u8"ENG: Smooth fade out dormant player.\nRUS: ������� ��������� ��������������� ������.");
		m_pGui->Checkbox(&cvars::visuals.esp_dormant_update_by_sound, "Dormant update by sound", u8"ENG: Update dormant players by sound.\nRUS: ��������� �������������� ������� �� �����.\n");
		m_pGui->Checkbox(&cvars::visuals.esp_interpolate_history, "Interpolate history", u8"ENG: Smooth movements of dormant players.\nRUS: ���������� �������� �������������� �������.");
		//m_pGui->PopCondition(2);
		m_pGui->Slider(&cvars::visuals.esp_font_size, "Font size", 1, 25, "%i", u8"");
		//m_pGui->PopCondition();
		
	}

	static void DrawPlayer(const std::unique_ptr<CMenuGui>& m_pGui)
	{
		const char* player_box_types[] = { "Off", "Default", "Corner", "Round" };
		const char* player_pos[] = { "Off", "Left", "Right", "Top", "Bot" };
		const char* player_distance_measurement[] = { "Units", "Meters" };
		const char* player_hiboxes[] = { "Off", "Only head", "All hitboxes" };

		m_pGui->SetID("##visuals_players_player");
		m_pGui->Checkbox(&cvars::visuals.esp_player, "Enable", u8"ENG: Enable rendering of players.\nRUS: ��������� ����������� �������.");
		//m_pGui->PushCondition(cvars::visuals.esp_player);
		//m_pGui->Checkbox(&cvars::visuals.esp_player_local, "Local", u8"ENG: Enable local player display.\nRUS: �������� ����������� ���������� ������.");
		//m_pGui->Checkbox(&cvars::visuals.esp_player_enemies, "Enemies", u8"ENG: Enable display of enemies.\nRUS: �������� ����������� ������.");
		//m_pGui->Checkbox(&cvars::visuals.esp_player_teammates, "Teammates", u8"ENG: Enable display of teammates.\nRUS: �������� ����������� ��������� �� �������.");
		//m_pGui->PushCondition(cvars::visuals.esp_player_local || cvars::visuals.esp_player_enemies || cvars::visuals.esp_player_teammates);
		m_pGui->Combo(&cvars::visuals.esp_player_box, "Bound box", player_box_types, IM_ARRAYSIZE(player_box_types), u8"ENG: Draw a box on the players.\nRUS: �������� ������� �� �������.");
		//m_pGui->PushCondition(cvars::visuals.esp_player_box);
		m_pGui->Checkbox(&cvars::visuals.esp_player_box_outline, "Bound box outline", u8"ENG: Draw the outline of the boxes.\nRUS: �������� ������ �� ���������.");
		m_pGui->Slider(&cvars::visuals.esp_player_box_filled, "Bound box filled", 0, 255, cvars::visuals.esp_player_box_filled ? "%i" : "Off", u8"ENG: Transparency of the filled box\nRUS: ������������ ������������ ��������.");
		m_pGui->Text("Bound box colors");
		//m_pGui->ColorEdit(cvars::visuals.esp_player_box_color_t, "##Bound box (Terrorist)"); ImGui::SameLine();
		m_pGui->Text("T");
		//m_pGui->ColorEdit(cvars::visuals.esp_player_box_color_ct, "##Bound box (Counter-Terrorist)", 0); ImGui::SameLine();
		m_pGui->Text("CT");
		//m_pGui->PopCondition();
		m_pGui->Combo(&cvars::visuals.esp_player_health, "Health bar", player_pos, IM_ARRAYSIZE(player_pos), u8"ENG: Draw a health bar.\nRUS: �������� ������� ��������.");
		//m_pGui->PushCondition(cvars::visuals.esp_player_health);
		m_pGui->Checkbox(&cvars::visuals.esp_player_health_percentage, "Health percentage", u8"ENG: Show health percentage on the bar.\nRUS: �������� �������� � ��������� �� �������.");
		//m_pGui->ColorEdit(cvars::visuals.esp_player_health_percentage_color, "##Health percentage", GImGui->CurrentWindow->Size.x - 56);
		//m_pGui->PopCondition();
		//m_pGui->PushItemWidth(ITEM_WIDTH_WITH_COLOR);
		m_pGui->Combo(&cvars::visuals.esp_player_armor, "Armor bar", player_pos, IM_ARRAYSIZE(player_pos), u8"ENG: Draw a armor bar.\nRUS: �������� ������� �����.");
		//m_pGui->PopItemWidth();
		//m_pGui->ColorEdit(cvars::visuals.esp_player_armor_color, "##Armor bar", 0);
		//m_pGui->PushItemWidth(ITEM_WIDTH_WITH_COLOR);
		m_pGui->Combo(&cvars::visuals.esp_player_name, "Name", player_pos, IM_ARRAYSIZE(player_pos), u8"ENG: Display player name.\nRUS: ���������� ��� ������.");
		//m_pGui->PopItemWidth();
		//m_pGui->ColorEdit(cvars::visuals.esp_player_name_color, "##Name", 0);
		//m_pGui->PushItemWidth(ITEM_WIDTH_WITH_COLOR);
		m_pGui->Combo(&cvars::visuals.esp_player_weapon_text, "Weapon text", player_pos, IM_ARRAYSIZE(player_pos), u8"ENG: Display player's weapon in text.\nRUS: ���������� ������ ������ �������.");
		//m_pGui->PopItemWidth();
		//m_pGui->ColorEdit(cvars::visuals.esp_player_weapon_text_color, "##Weapon text", 0);
		//m_pGui->PushItemWidth(ITEM_WIDTH_WITH_COLOR);
		m_pGui->Combo(&cvars::visuals.esp_player_weapon_icon, "Weapon icon", player_pos, IM_ARRAYSIZE(player_pos), u8"ENG: Display player's weapon with icon.\nRUS: ���������� ������ ������ �������.");
		//m_pGui->PopItemWidth();
		//m_pGui->ColorEdit(cvars::visuals.esp_player_weapon_icon_color, "##Weapon icon", 0);
		//m_pGui->PushItemWidth(ITEM_WIDTH_WITH_COLOR);
		m_pGui->Combo(&cvars::visuals.esp_player_money, "Money", player_pos, IM_ARRAYSIZE(player_pos), u8"ENG: Display player money.\nRUS: ���������� ������ ������.");
		//m_pGui->PopItemWidth();
		//m_pGui->ColorEdit(cvars::visuals.esp_player_money_color, "##Money", 0);
		//m_pGui->PushItemWidth(ITEM_WIDTH_WITH_COLOR);
		m_pGui->Combo(&cvars::visuals.esp_player_distance, "Distance", player_pos, IM_ARRAYSIZE(player_pos), u8"ENG: Display player distance from you.\nRUS: ���������� ��������� ������ �� ���.");
		//m_pGui->ColorEdit(cvars::visuals.esp_player_distance_color, "##Distance", 0);
		//m_pGui->PopItemWidth();
		//m_pGui->PushCondition(cvars::visuals.esp_player_distance);
		m_pGui->Combo(&cvars::visuals.esp_player_distance_measurement, "Distance measurement", player_distance_measurement, IM_ARRAYSIZE(player_distance_measurement));
		//m_pGui->PopCondition();
		//m_pGui->PushItemWidth(ITEM_WIDTH_WITH_COLOR);
		m_pGui->Combo(&cvars::visuals.esp_player_actions, "Actions", player_pos, IM_ARRAYSIZE(player_pos), u8"ENG: Display player actions.\nRUS: ���������� �������� ������.");
		//m_pGui->PopItemWidth();
		//m_pGui->ColorEdit(cvars::visuals.esp_player_actions_color, "##Actions", 0);
		//m_pGui->PushCondition(cvars::visuals.esp_player_actions);
		m_pGui->Checkbox(&cvars::visuals.esp_player_actions_bar, "Actions bar", u8"");
		//m_pGui->ColorEdit(cvars::visuals.esp_player_actions_bar_color, "##Actions bar", GImGui->CurrentWindow->Size.x - 56);
		//m_pGui->PopCondition();
		//m_pGui->PushItemWidth(ITEM_WIDTH_WITH_COLOR);
		m_pGui->Combo(&cvars::visuals.esp_player_has_c4, "Has C4", player_pos, IM_ARRAYSIZE(player_pos), u8"ENG: Display player with c4.\nRUS: ���������� ������ � ������.");
		//m_pGui->PopItemWidth();
		//m_pGui->ColorEdit(cvars::visuals.esp_player_has_c4_color, "##Has C4", 0);
		//m_pGui->PushItemWidth(ITEM_WIDTH_WITH_COLOR);
		m_pGui->Combo(&cvars::visuals.esp_player_has_defusal_kits, "Has defusal kits", player_pos, IM_ARRAYSIZE(player_pos), u8"ENG: Display player with defusal kits.\nRUS: ���������� ������ � ������ �������.");
		//m_pGui->PopItemWidth();
		//m_pGui->ColorEdit(cvars::visuals.esp_player_has_defusal_kits_color, "##Has defusal kits", 0);
		//m_pGui->PushItemWidth(ITEM_WIDTH_WITH_COLOR);
		m_pGui->Slider(&cvars::visuals.esp_player_line_of_sight, "Line of sight", 0.f, 4096.f, cvars::visuals.esp_player_line_of_sight ? "%.0f units" : "Off", 2.f, u8"ENG: Draw player line of sight.\nRUS: �������� ����� ���� ������� �����.");
		//m_pGui->PopItemWidth();
		//m_pGui->ColorEdit(cvars::visuals.esp_player_line_of_sight_color, "##Line of sight", 0);
		//m_pGui->PushItemWidth(ITEM_WIDTH_WITH_COLOR);
		m_pGui->Combo(&cvars::visuals.esp_player_hitboxes, "Hitboxes", player_hiboxes, IM_ARRAYSIZE(player_hiboxes), u8"ENG: Draw player hitboxes.\nRUS: �������� �������� ������.");
		//m_pGui->PopItemWidth();
		//m_pGui->ColorEdit(cvars::visuals.esp_player_hitboxes_color, "##Hitboxes", 0);
		m_pGui->Checkbox(&cvars::visuals.esp_player_hitboxes_hit_position, "Hitboxes hit position", u8"ENG: .\nRUS: .");
		//m_pGui->ColorEdit(cvars::visuals.esp_player_hitboxes_hit_position_color, "##Hitboxes hit position", GImGui->CurrentWindow->Size.x - 96);
		//m_pGui->ColorEdit(cvars::visuals.esp_player_hitboxes_hit_position_color2, "##Hitboxes hit position 2", 0);
		//m_pGui->PushCondition(cvars::visuals.esp_player_hitboxes_hit_position);
		m_pGui->Slider(&cvars::visuals.esp_player_hitboxes_hit_position_time, "Hitboxes hit position time", 0.f, 5.f, "%.1f sec", 1.f, u8"");
		//m_pGui->PopCondition();
		m_pGui->Checkbox(&cvars::visuals.esp_player_skeleton, "Skeleton", u8"ENG: Draw player skeleton.\nRUS: �������� ������ ������.");
		//m_pGui->ColorEdit(cvars::visuals.esp_player_skeleton_color, "##Skeleton", GImGui->CurrentWindow->Size.x - 56);
		m_pGui->Checkbox(&cvars::visuals.esp_player_skeleton_backtrack, "Skeleton backtrack", u8"ENG: Draw player skeleton on backtrack position.\nRUS: �������� ������ ������ � ������� �������.");
		//m_pGui->ColorEdit(cvars::visuals.esp_player_skeleton_backtrack_color, "##Skeleton backtrack", GImGui->CurrentWindow->Size.x - 56);
		//m_pGui->PopCondition(2);
		
	}

	static void DrawGlow(const std::unique_ptr<CMenuGui>& m_pGui)
	{
		m_pGui->SetID("##visuals_players_glow");
		m_pGui->Slider(&cvars::visuals.esp_player_glow_amount, "Enable", 0, 25, cvars::visuals.esp_player_glow_amount ? "%i amount" : "Off", u8"ENG: Enable rendering of players.\nRUS: ��������� ����������� �������.");
		//m_pGui->PushCondition(cvars::visuals.esp_player_glow_amount);
		//m_pGui->Checkbox(&cvars::visuals.esp_player_glow_local, "Local", u8"ENG: Enable local player display.\nRUS: �������� ����������� ���������� ������.");
		//m_pGui->Checkbox(&cvars::visuals.esp_player_glow_enemies, "Enemies", u8"ENG: Enable display of enemies.\nRUS: �������� ����������� ������.");
		//m_pGui->Checkbox(&cvars::visuals.esp_player_glow_teammates, "Teammates", u8"ENG: Enable display of teammates.\nRUS: �������� ����������� ��������� �� �������.");
		////m_pGui->PushCondition(cvars::visuals.esp_player_glow_local || cvars::visuals.esp_player_glow_enemies || cvars::visuals.esp_player_glow_teammates);
		m_pGui->Text("Colors");
		//m_pGui->ColorEdit(cvars::visuals.esp_player_glow_color_t, "##Glow (Terrorrist)"); ImGui::SameLine();
		m_pGui->Text("T");
		//m_pGui->ColorEdit(cvars::visuals.esp_player_glow_color_ct, "##Glow (Counter-Terrorist)", 0); ImGui::SameLine();
		m_pGui->Text("CT");
		//m_pGui->PopCondition(2);
		
	}

	static void DrawSound(const std::unique_ptr<CMenuGui>& m_pGui)
	{
		m_pGui->SetID("##visuals_players_sound");
		m_pGui->Checkbox(&cvars::visuals.esp_player_sounds, "Enable", u8"ENG: Draw player sounds.\nRUS: �������� ����� �������.");
		//m_pGui->PushCondition(cvars::visuals.esp_player_sounds);
		//m_pGui->Checkbox(&cvars::visuals.esp_player_sounds_local, "Local", u8"ENG: Enable local player display.\nRUS: �������� ����������� ���������� ������.");
		//m_pGui->Checkbox(&cvars::visuals.esp_player_sounds_enemies, "Enemies", u8"ENG: Enable display of enemies.\nRUS: �������� ����������� ������.");
		//m_pGui->Checkbox(&cvars::visuals.esp_player_sounds_teammates, "Teammates", u8"ENG: Enable display of teammates.\nRUS: �������� ����������� ��������� �� �������.");
		////m_pGui->PushCondition(cvars::visuals.esp_player_sounds_local || cvars::visuals.esp_player_sounds_enemies || cvars::visuals.esp_player_sounds_teammates);
		m_pGui->Slider(&cvars::visuals.esp_player_sounds_time, "Fade out time", 0.f, 5.f, "%.1f sec", 1.f, u8"ENG: Time after which the player's sound disappears.\nRUS: �����, ����� �������� ���� ������ ��������.");
		//m_pGui->PushCondition(cvars::visuals.esp_player_sounds_time);
		m_pGui->Slider(&cvars::visuals.esp_player_sounds_circle_radius, "Circle radius", 5.f, 25.f, "%.0f", 1.f, u8"ENG: Circle radius of drawn sound.\nRUS: ������ ����� ��������� �����.");
		m_pGui->Text("Colors");
		//m_pGui->ColorEdit(cvars::visuals.esp_player_sounds_color_t, "##Visualize sounds (Terrorist)"); ImGui::SameLine();
		m_pGui->Text("T");
		//m_pGui->ColorEdit(cvars::visuals.esp_player_sounds_color_ct, "##Visualize sounds (Counter-Terrorist)", 0); ImGui::SameLine();
		m_pGui->Text("CT");
		//m_pGui->PopCondition(3);
		
	}

	static void DrawOutFOV(const std::unique_ptr<CMenuGui>& m_pGui)
	{
		const char* player_out_of_fov[] = { "Off", "Arrows", "Circles", "Rhombuses" };

		m_pGui->SetID("##visuals_players_out_of_fov");
		m_pGui->Combo(&cvars::visuals.esp_player_out_of_fov_draw_type, "Enable", player_out_of_fov, IM_ARRAYSIZE(player_out_of_fov), u8"ENG: Display players out of sight.\nRUS: ���������� ������� ����������� ��� ���� ������.");
		//m_pGui->PushCondition(cvars::visuals.esp_player_out_of_fov);
		//m_pGui->Checkbox(&cvars::visuals.esp_player_out_of_fov_enemies, "Enemies", u8"ENG: Enable display of enemies.\nRUS: �������� ����������� ������.");
		//m_pGui->Checkbox(&cvars::visuals.esp_player_out_of_fov_teammates, "Teammates", u8"ENG: Enable display of teammates.\nRUS: �������� ����������� ��������� �� �������.");
		//m_pGui->PushCondition(cvars::visuals.esp_player_out_of_fov_enemies || cvars::visuals.esp_player_out_of_fov_teammates);
		//m_pGui->PushCondition(cvars::visuals.esp_player_out_of_fov > ARROWS);
		//m_pGui->Checkbox(&cvars::visuals.esp_player_out_of_fov_name, "Name", u8"ENG: Display player name out of sight.\nRUS: ���������� ��� ������ ������������ ��� ���� ������.");
		//m_pGui->PopCondition();
		m_pGui->Slider(&cvars::visuals.esp_player_out_of_fov_size, "Size", 2.f, 25.f, "%.0f pixels", 1.f, u8"ENG: The size of the figures of the displayed players out of sight.\nRUS: ������ ����� ������������ ������� ����������� ��� ���� ������.");
		m_pGui->Slider(&cvars::visuals.esp_player_out_of_fov_radius, "Radius", 20.f, 100.f, "%.0f%%", 1.f, u8"ENG: The radius of the drawn objects from the center of the screen of the displayed players are out of sight.\nRUS: ������ ������������ �������� �� ������ ������ ������������ ������� ����������� ��� ���� ������.");
		m_pGui->Checkbox(&cvars::visuals.esp_player_out_of_fov_aspect_ratio, "Aspect ratio", u8"ENG: Follow aspect ratio.\nRUS: ��������� ����������� ������.");
		m_pGui->Text("Colors");
		//m_pGui->ColorEdit(cvars::visuals.esp_player_out_of_fov_color_t, "##Out of FOV (Terrorist)"); ImGui::SameLine();
		m_pGui->Text("T");
		//m_pGui->ColorEdit(cvars::visuals.esp_player_out_of_fov_color_ct, "##Out of FOV (Counter-Terrorist)", 0); ImGui::SameLine();
		m_pGui->Text("CT");
		m_pGui->Checkbox(&cvars::visuals.esp_player_out_of_fov_impulse_alpha, "Impulse alpha", u8"ENG: Enable impulsive transparency change.\nRUS: �������� ���������� ��������� ������������.");
		m_pGui->Checkbox(&cvars::visuals.esp_player_out_of_fov_outline, "Outline", u8"ENG: Draw the outline.\nRUS: �������� ������.");
		//m_pGui->PopCondition(2);
		
	}

	static void DrawRadar(const std::unique_ptr<CMenuGui>& m_pGui)
	{
		m_pGui->SetID("##visuals_players_radar");
		
	}
}

namespace ColoredModels
{
	static void DrawGlobals(const std::unique_ptr<CMenuGui>& m_pGui)
	{
		m_pGui->SetID("##visuals_colored_models_globals");
		//m_pGui->Checkbox(&cvars::visuals.colored_models, "Enabled", u8"ENG: Global on/off colored models. \nRUS: ���������� ���/���� ������� �������.");
		//m_pGui->Key(&cvars::visuals.colored_models_key, "Colored models key");
		//m_pGui->PushCondition(cvars::visuals.colored_models);
		//m_pGui->Checkbox(&cvars::visuals.colored_models_dormant, "Dormant", u8"ENG: Enable rendering dormant player in last position.\nRUS: �������� ��������� ��������������� ������ � ��������� �������.");
		//m_pGui->PushCondition(cvars::visuals.colored_models_dormant);
		m_pGui->Slider(&cvars::visuals.colored_models_dormant_time, "Dormant time", 0.f, 5.f, "%.1f sec", 1.f, u8"ENG: Display time of dormant player in the last position.\nRUS: ����� ����������� ��������������� ������ � ��������� �������.");
		//m_pGui->PushCondition(cvars::visuals.colored_models_dormant_time);
		m_pGui->Checkbox(&cvars::visuals.colored_models_dormant_fadeout, "Dormant fade out", u8"ENG: Enable smooth fade out dormant player.\nRUS: �������� ������� ��������� ��������������� ������.");
		//m_pGui->PopCondition(2);
		m_pGui->Checkbox(&cvars::visuals.colored_models_paint_players_weapons, "Paint players weapons", u8"ENG: Paint players weapons.\nRUS: ��������� ������ �������.");
		//m_pGui->PopCondition();
		

		ImGui::Spacing();

		ImVec2 button_size;

		button_size.x = (GImGui->CurrentWindow->Size.x - WINDOW_PADDING_SAMELINE * 2.f - GImGui->Style.ItemSpacing.x * 2.f) / 3.f;
		button_size.y = 22.f;

		/*if (ImGui::Button("Enable all local##colored_models", button_size))
		{
			cvars::visuals.colored_models_players_local = true;
			cvars::visuals.colored_models_dlight_local = true;
			cvars::visuals.colored_models_elight_local = true;
		}

		ImGui::SameLine();

		if (ImGui::Button("Enable all enemies##colored_models", button_size))
		{
			cvars::visuals.colored_models_players_enemies = true;
			cvars::visuals.colored_models_backtrack_enemies = true;
			cvars::visuals.colored_models_dlight_enemies = true;
			cvars::visuals.colored_models_elight_enemies = true;
		}

		ImGui::SameLine();

		if (ImGui::Button("Enable all teammates##colored_models", button_size))
		{
			cvars::visuals.colored_models_players_teammates = true;
			cvars::visuals.colored_models_backtrack_teammates = true;
			cvars::visuals.colored_models_dlight_teammates = true;
			cvars::visuals.colored_models_elight_teammates = true;
		}

		ImGui::Spacing();

		if (ImGui::Button("Disable all local##colored_models", button_size))
		{
			cvars::visuals.colored_models_players_local = false;
			cvars::visuals.colored_models_dlight_local = false;
			cvars::visuals.colored_models_elight_local = false;
		}

		ImGui::SameLine();

		if (ImGui::Button("Disable all enemies##colored_models", button_size))
		{
			cvars::visuals.colored_models_players_enemies = false;
			cvars::visuals.colored_models_backtrack_enemies = false;
			cvars::visuals.colored_models_dlight_enemies = false;
			cvars::visuals.colored_models_elight_enemies = false;
		}

		ImGui::SameLine();

		if (ImGui::Button("Disable all teammates##colored_models", button_size))
		{
			cvars::visuals.colored_models_players_teammates = false;
			cvars::visuals.colored_models_backtrack_teammates = false;
			cvars::visuals.colored_models_dlight_teammates = false;
			cvars::visuals.colored_models_elight_teammates = false;
		}*/
	}

	static void DrawPlayers(const std::unique_ptr<CMenuGui>& m_pGui)
	{
		const char* colored_models[] = { "Off", "Flat", "Darkened", "Ligthed", "Texture" };
		const char* colored_models_health_based[] = { "Off", "Visible", "Always" };

		m_pGui->SetID("##visuals_colored_models_players");
		//m_pGui->PushCondition(cvars::visuals.colored_models);
		m_pGui->Combo(&cvars::visuals.colored_models_players, "Enabled", colored_models, IM_ARRAYSIZE(colored_models), u8"ENG: Enable rendering of the players with the selected type of painting.\nRUS: �������� ��������� ������� ��������� ����� ��������.");
		//m_pGui->PushCondition(cvars::visuals.colored_models_players);
		m_pGui->Checkbox(&cvars::visuals.colored_models_players_wireframe, "Wireframe", u8"ENG: Enable rendering of the player in the grid.\nRUS: �������� ��������� ������ � �����.");
		//m_pGui->Checkbox(&cvars::visuals.colored_models_players_local, "Local", u8"ENG: Enable local player rendering.\nRUS: �������� ��������� ���������� ������.");	
		//m_pGui->Checkbox(&cvars::visuals.colored_models_players_enemies, "Enemies", u8"ENG: Enable rendering of enemies.\nRUS: �������� ��������� ������.");
		//m_pGui->Checkbox(&cvars::visuals.colored_models_players_teammates, "Teammates", u8"ENG: Enable rendering of teammates.\nRUS: �������� ��������� ��������� �� �������.");
		//m_pGui->PushCondition(cvars::visuals.colored_models_players_local || cvars::visuals.colored_models_players_enemies || cvars::visuals.colored_models_players_teammates);
		m_pGui->Checkbox(&cvars::visuals.colored_models_players_behind_wall, "Behind wall", u8"ENG: Enable rendering of players behind the wall.\nRUS: �������� ������ ������� �� ������.");
		m_pGui->Combo(&cvars::visuals.colored_models_players_color_health_based, "Color health based", colored_models_health_based, IM_ARRAYSIZE(colored_models_health_based), u8"ENG: Enable color change based on health.\nRUS: �������� ��������� ����� �� ������ ��������.");
		//m_pGui->PushCondition(cvars::visuals.colored_models_players_behind_wall && cvars::visuals.colored_models_players_color_health_based != ColorHealthBased_Always);
		m_pGui->Text("Colors hide");
		//m_pGui->ColorEdit(cvars::visuals.colored_models_players_color_t_hide, "##Players hide (Terrorrist)"); ImGui::SameLine();
		m_pGui->Text("T");
		//m_pGui->ColorEdit(cvars::visuals.colored_models_players_color_ct_hide, "##Players hide (Counter-Terrorist)", 0); ImGui::SameLine();
		m_pGui->Text("CT");
		//m_pGui->PopCondition();
		//m_pGui->PushCondition(!cvars::visuals.colored_models_players_color_health_based);
		m_pGui->Text("Colors visible");
		//m_pGui->ColorEdit(cvars::visuals.colored_models_players_color_t_vis, "##Players visible (Terrorrist)"); ImGui::SameLine();
		m_pGui->Text("T");
		//m_pGui->ColorEdit(cvars::visuals.colored_models_players_color_ct_vis, "##Players visible (Counter-Terrorist)", 0); ImGui::SameLine();
		m_pGui->Text("CT");
		//m_pGui->PopCondition();
		//m_pGui->PushItemWidth(ITEM_WIDTH_WITH_COLOR);
		m_pGui->Combo(&cvars::visuals.colored_models_players_on_the_dead, "On the dead", colored_models, IM_ARRAYSIZE(colored_models), u8"ENG: Enable rendering on dead players.\nRUS: �������� ������ �� ������� �������.");
		//m_pGui->PopItemWidth();
		//m_pGui->ColorEdit(cvars::visuals.colored_models_players_on_the_dead_color, "##On the dead", 0);
		//m_pGui->PushItemWidth(ITEM_WIDTH_WITH_COLOR);	
		//m_pGui->PopCondition(2);
		m_pGui->Combo(&cvars::visuals.colored_models_players_hit_position, "Hit position", colored_models, IM_ARRAYSIZE(colored_models), u8"ENG: Enable rendering of the model in the hit position.\nRUS: �������� ������ ������ � ������� ���������.");
		//m_pGui->PopItemWidth();
		//m_pGui->ColorEdit(cvars::visuals.colored_models_players_hit_position_color, "##Hit position", 0);
		//m_pGui->PushCondition(cvars::visuals.colored_models_players_hit_position);
		m_pGui->Slider(&cvars::visuals.colored_models_players_hit_position_time, "Hit position time", 0.f, 5.f, "%.1f sec", 1.f, u8"ENG: Display time of the hit position.\nRUS: ����� ����������� ������� ���������.");
		//m_pGui->PopCondition();
		//m_pGui->PushItemWidth(ITEM_WIDTH_WITH_COLOR);
		m_pGui->Combo(&cvars::visuals.colored_models_players_desync_aa, "Desync AA", colored_models, IM_ARRAYSIZE(colored_models), u8"ENG: Enable desync local player rendering.\nRUS: �������� ��������� ���������������� ���������� ������.");
		//m_pGui->PopItemWidth();
		//m_pGui->ColorEdit(cvars::visuals.colored_models_players_desync_aa_color, "##Desync AA", 0);
		//m_pGui->PopCondition();
		
	}

	static void DrawBacktrack(const std::unique_ptr<CMenuGui>& m_pGui)
	{
		const char* colored_models[] = { "Off", "Flat", "Darkened", "Ligthed", "Texture" };

		m_pGui->SetID("##visuals_colored_models_backtrack");
		//m_pGui->PushCondition(cvars::visuals.colored_models);
		m_pGui->Combo(&cvars::visuals.colored_models_backtrack, "Enabled", colored_models, IM_ARRAYSIZE(colored_models), u8"ENG: Enable rendering of the players in the backtrack position with the selected type of painting.\nRUS: �������� ��������� ������� � ������� ������� ��������� ����� ��������.");
		//m_pGui->PushCondition(cvars::visuals.colored_models_backtrack);
		m_pGui->Checkbox(&cvars::visuals.colored_models_backtrack_wireframe, "Wireframe", u8"ENG: Enable rendering of the player in the backtrack position in the grid.\nRUS: �������� ��������� ������ � ������� ������� � �����.");
		//m_pGui->Checkbox(&cvars::visuals.colored_models_backtrack_enemies, "Enemies", u8"ENG: Enable rendering of enemies in the backtrack position.\nRUS: �������� ��������� ������ � ������� �������.");
		//m_pGui->Checkbox(&cvars::visuals.colored_models_backtrack_teammates, "Teammates", u8"ENG: Enable rendering of teammates in the backtrack position.\nRUS: �������� ��������� ��������� �� ������� � ������� �������.");
		//m_pGui->PushCondition(cvars::visuals.colored_models_backtrack_enemies || cvars::visuals.colored_models_backtrack_teammates);
		m_pGui->Checkbox(&cvars::visuals.colored_models_backtrack_behind_wall, "Behind wall", u8"ENG: Enable rendering of players in the backtrack position behind the wall.\nRUS: �������� ������ ������� � ������� ������� �� ������.");
		//m_pGui->PushCondition(cvars::visuals.colored_models_backtrack_behind_wall);
		m_pGui->Text("Colors hide");
		//m_pGui->ColorEdit(cvars::visuals.colored_models_backtrack_color_t_hide, "##Backtrack hide (Terrorrist)"); ImGui::SameLine();
		m_pGui->Text("T");
		//m_pGui->ColorEdit(cvars::visuals.colored_models_backtrack_color_ct_hide, "##Backtrack hide (Counter-Terrorist)", 0); ImGui::SameLine();
		m_pGui->Text("CT");
		//m_pGui->PopCondition();
		m_pGui->Text("Colors visible");
		//m_pGui->ColorEdit(cvars::visuals.colored_models_backtrack_color_t_vis, "##Backtrack visible (Terrorrist)"); ImGui::SameLine();
		m_pGui->Text("T");
		//m_pGui->ColorEdit(cvars::visuals.colored_models_backtrack_color_ct_vis, "##Backtrack visible (Counter-Terrorist)", 0); ImGui::SameLine();
		m_pGui->Text("CT");
		//m_pGui->PopCondition(3);
		
	}

	static void DrawHands(const std::unique_ptr<CMenuGui>& m_pGui)
	{
		const char* colored_models[] = { "Off", "Flat", "Darkened", "Ligthed", "Texture" };

		m_pGui->SetID("##visuals_colored_models_hands");
		//m_pGui->PushCondition(cvars::visuals.colored_models);
		//m_pGui->PushItemWidth(ITEM_WIDTH_WITH_COLOR);
		m_pGui->Combo(&cvars::visuals.colored_models_hands, "Enabled", colored_models, IM_ARRAYSIZE(colored_models), u8"ENG: Enable hand rendering of the selected paint type.\nRUS: �������� ��������� ��� ��������� ����� ��������.");
		//m_pGui->PopItemWidth();
		//m_pGui->ColorEdit(cvars::visuals.colored_models_hands_color, "##Hands", 0);
		//m_pGui->PushCondition(cvars::visuals.colored_models_hands);
		m_pGui->Checkbox(&cvars::visuals.colored_models_hands_wireframe, "Wireframe", u8"ENG: Enable rendering of the hands in the grid.\nRUS: �������� ��������� ��� � �����.");
		m_pGui->Checkbox(&cvars::visuals.colored_models_hands_color_rainbow, "Rainbow color", u8"ENG: Enable rainbow color change.\nRUS: �������� �������� ��������� �����.");
		//m_pGui->PushCondition(cvars::visuals.colored_models_hands_color_rainbow);
		m_pGui->Slider(&cvars::visuals.colored_models_hands_color_rainbow_speed, "Rainbow speed", 0.1f, 2.f, "%.1f", 1.f, u8"ENG: Color change rate.\nRUS: �������� ��������� �����.");
		//m_pGui->PopCondition(3);
		
	}

	static void DrawDlight(const std::unique_ptr<CMenuGui>& m_pGui)
	{
		const char* origin[] = { "Legs", "Body", "Head" };

		m_pGui->SetID("##visuals_colored_models_dlight");
		//m_pGui->PushCondition(cvars::visuals.colored_models);
		m_pGui->Checkbox(&cvars::visuals.colored_models_dlight, "Enabled", u8"ENG: Enable Dlight.\nRUS: �������� Dlight.");
		//m_pGui->ColorEdit(cvars::visuals.colored_models_dlight_color, "##Dlight", GImGui->CurrentWindow->Size.x - 56);
		//m_pGui->PushCondition(cvars::visuals.colored_models_dlight);
		//m_pGui->Checkbox(&cvars::visuals.colored_models_dlight_local, "Local", u8"ENG: Enable local player lighting.\nRUS: �������� ��������� ���������� ������.");
		//m_pGui->Checkbox(&cvars::visuals.colored_models_dlight_enemies, "Enemies", u8"ENG: Enable lighting of enemies.\nRUS: �������� ��������� ������.");
		//m_pGui->Checkbox(&cvars::visuals.colored_models_dlight_teammates, "Teammates", u8"ENG: Enable lighting of teammates.\nRUS: �������� ��������� ��������� �� �������.");
		//m_pGui->PushCondition(cvars::visuals.colored_models_dlight_local || cvars::visuals.colored_models_dlight_enemies || cvars::visuals.colored_models_dlight_teammates);
		m_pGui->Combo(&cvars::visuals.colored_models_dlight_origin, "Origin", origin, IM_ARRAYSIZE(origin), u8"ENG: Light source.\nRUS: �������� ������������ �����.");
		m_pGui->Slider(&cvars::visuals.colored_models_dlight_radius, "Radius", 0.f, 250.f, "%.0f", 1.f, u8"ENG: Lighting radius.\nRUS: ������ ������������ �����.");
		m_pGui->Slider(&cvars::visuals.colored_models_dlight_minlight, "Minlight", 0.f, 250.f, "%.0f", 1.f, u8"ENG: Minimum light value.\nRUS: �������� ������������ �����.");
		m_pGui->Checkbox(&cvars::visuals.colored_models_dlight_fading_lighting, "Fading lighting", u8"ENG: Enable fading lighting.\nRUS: �������� ���������� ���������.");
		//m_pGui->PushCondition(cvars::visuals.colored_models_dlight_fading_lighting);
		m_pGui->Slider(&cvars::visuals.colored_models_dlight_fading_lighting_speed, "Fading lighting speed", 0.1f, 2.f, "%.1f", 1.f, u8"ENG: Fading lighting speed.\nRUS: �������� ����������� ���������.");
		//m_pGui->PopCondition(4);
		
	}

	static void DrawElight(const std::unique_ptr<CMenuGui>& m_pGui)
	{
		const char* origin[] = { "Legs", "Body", "Head" };

		m_pGui->SetID("##visuals_colored_models_elight");
		//m_pGui->PushCondition(cvars::visuals.colored_models);
		m_pGui->Checkbox(&cvars::visuals.colored_models_elight, "Enabled", u8"ENG: Enable Elight.\nRUS: �������� Elight.");
		//m_pGui->ColorEdit(cvars::visuals.colored_models_elight_color, "##Elight", GImGui->CurrentWindow->Size.x - 56);
		//m_pGui->PushCondition(cvars::visuals.colored_models_elight);
		//m_pGui->Checkbox(&cvars::visuals.colored_models_elight_local, "Local", u8"ENG: Enable local player lighting.\nRUS: �������� ��������� ���������� ������.");
		//m_pGui->Checkbox(&cvars::visuals.colored_models_elight_enemies, "Enemies", u8"ENG: Enable lighting of enemies.\nRUS: �������� ��������� ������.");
		//m_pGui->Checkbox(&cvars::visuals.colored_models_elight_teammates, "Teammates", u8"ENG: Enable lighting of teammates.\nRUS: �������� ��������� ��������� �� �������.");
		//m_pGui->PushCondition(cvars::visuals.colored_models_elight_local || cvars::visuals.colored_models_elight_enemies || cvars::visuals.colored_models_elight_teammates);
		//m_pGui->Combo(&cvars::visuals.colored_models_elight_origin, "Origin", origin, IM_ARRAYSIZE(origin), u8"ENG: Light source.\nRUS: �������� ������������ �����.");
		m_pGui->Slider(&cvars::visuals.colored_models_elight_radius, "Radius", 0.f, 250.f, "%.0f", 1.f, u8"ENG: Lighting radius.\nRUS: ������ ������������ �����.");
		//m_pGui->PopCondition(3);
		
	}
}

namespace Other
{
	static void DrawWorld(const std::unique_ptr<CMenuGui>& m_pGui)
	{
		m_pGui->SetID("##visuals_other_world");
		
	}

	static void DrawLocal(const std::unique_ptr<CMenuGui>& m_pGui)
	{
		m_pGui->SetID("##visuals_other_local");
		m_pGui->Checkbox(&cvars::visuals.esp_other_local_aa_side_arrows, "Anti-aimbot side arrows", u8"");
		m_pGui->Checkbox(&cvars::visuals.esp_other_local_sniper_crosshair, "Sniper crosshair", u8"ENG: Enable crosshair on sniper rifles.\nRUS: �������� ������ �� ����������� ���������.");
		m_pGui->Checkbox(&cvars::visuals.esp_other_local_recoil_point, "Recoil point", u8"");
		//m_pGui->ColorEdit(cvars::visuals.esp_other_local_recoil_point_color, "##Recoil point", GImGui->CurrentWindow->Size.x - 56);
		m_pGui->Checkbox(&cvars::visuals.esp_other_local_spread_point, "Spread point", u8"");
		//m_pGui->ColorEdit(cvars::visuals.esp_other_local_spread_point_color, "##Spread point", GImGui->CurrentWindow->Size.x - 56);
		m_pGui->Checkbox(&cvars::visuals.esp_other_local_spread_circle, "Spread circle", u8"ENG: .\nRUS: .");
		//m_pGui->ColorEdit(cvars::visuals.esp_other_local_spread_circle_color, "##Spread circle", GImGui->CurrentWindow->Size.x - 96);
		//m_pGui->ColorEdit(cvars::visuals.esp_other_local_spread_circle_color2, "##Spread circle 2", 0);
		m_pGui->Checkbox(&cvars::visuals.esp_other_local_aim_fov, "Aim FOV", u8"ENG: Display aimbot field of view.\nRUS: ���������� ���� ������ �������.");
		//m_pGui->ColorEdit(cvars::visuals.esp_other_local_aim_fov_color, "##Aim FOV", GImGui->CurrentWindow->Size.x - 96);
		//m_pGui->ColorEdit(cvars::visuals.esp_other_local_aim_fov_color2, "##Aim FOV 2", 0);
		m_pGui->Checkbox(&cvars::visuals.esp_other_local_toggle_status, "Toggle status", u8"");
		//m_pGui->ColorEdit(cvars::visuals.esp_other_local_toggle_status_color, "##Toggle status", GImGui->CurrentWindow->Size.x - 96);
		//m_pGui->ColorEdit(cvars::visuals.esp_other_local_toggle_status_color2, "##Toggle status 2", 0);
		
	}
}

namespace Effects
{
	static void DrawEffects(const std::unique_ptr<CMenuGui>& m_pGui)
	{
		m_pGui->SetID("##visuals_effects_effects");
		m_pGui->Checkbox(&cvars::visuals.effects_hud_clear, "HUD clear");
		m_pGui->Slider(&cvars::visuals.effects_thirdperson, "Thirdperson", 0, 512, "%i");
		//m_pGui->PushCondition(cvars::visuals.effects_thirdperson);
		//m_pGui->Key(&cvars::visuals.effects_thirdperson_key, "Thirdperson key");
		//m_pGui->PopCondition();
		m_pGui->Slider(&cvars::visuals.effects_custom_render_fov, "Custom render FOV", 90, 150, "%i", u8"");
		
	}

	static void DrawRemovals(const std::unique_ptr<CMenuGui>& m_pGui)
	{
		m_pGui->SetID("##visuals_effects_removals");
		m_pGui->Checkbox(&cvars::visuals.remove_scope, "Remove scope", u8"");
		
	}
}

void CMenu::DrawVisuals()
{
	static int selected_players, hovered_players;
	static int selected_colored_models, hovered_colored_models;
	static int selected_other, hovered_other;
	static int selected_effects, hovered_effects;

	static int selected, hovered;
	const char* labels[] = { "MAIN", "WEAPON" };

	//m_pGui->TabFeaturesList(labels, IM_ARRAYSIZE(labels), &selected, &hovered, GImGui->CurrentWindow->Size.x);

	/*ImVec2 child_size;

	child_size.x = Math::TranslateToEvenGreatest(GImGui->CurrentWindow->Size.x - WINDOW_PADDING_X * 3.f) * 0.5f;
	child_size.y = 100.f;

	const float features_list_item_width = child_size.x - WINDOW_PADDING_X * 2.f;

	ImGui::Spacing();
	ImGui::SameLine(WINDOW_PADDING_X);
	ImGui::BeginGroup();

	m_pGui->BeginChild("General", child_size);
	m_pGui->SetID("##visuals_general");
	m_pGui->Checkbox(&cvars::visuals.active, "Enabled", "Global on/off visuals stuff.");
	//m_pGui->PushCondition(cvars::visuals.active);
	//m_pGui->Key(&cvars::visuals.panic_key, "Globals visuals", true);
	m_pGui->Checkbox(&cvars::visuals.copyright, "Copyright", "");
	m_pGui->Checkbox(&cvars::visuals.antiscreen, "Antiscreen", "");
	//m_pGui->PushTextColor(ImColor(255, 225, 150));
	m_pGui->Checkbox(&cvars::visuals.streamer_mode, "Streamer mode", u8"ENG: All visual functions are not visible on the recording / broadcasting programs.\nRUS: ��� ���������� ������� �� ����� �� ���������� ��� ������ / ����������.");
	//m_pGui->PopTextColor();
	//m_pGui->PopCondition();
	
	m_pGui->EndChild();

	ImGui::Spacing(WINDOW_PADDING_Y - 1);

	child_size.y = (GImGui->CurrentWindow->Size.y - ImGui::GetCursorPosY() - WINDOW_PADDING_X) * 0.539f;

	m_pGui->BeginChild("Player ESP", child_size);
	m_pGui->FeaturesList(PlayersFeatures, IM_ARRAYSIZE(PlayersFeatures), &selected_players, &hovered_players, features_list_item_width);
	//m_pGui->PushCondition(cvars::visuals.active);

	switch (selected_players)
	{
	case PLAYER_ESP_GLOBALS:	 Players::DrawGlobals(m_pGui); break;
	case PLAYER_ESP_PLAYER:		 Players::DrawPlayer(m_pGui); break;
	case PLAYER_ESP_GLOW:		 Players::DrawGlow(m_pGui); break;
	case PLAYER_ESP_SOUND:		 Players::DrawSound(m_pGui); break;
	case PLAYER_ESP_OUT_OF_FOV:  Players::DrawOutFOV(m_pGui); break;
	case PLAYER_ESP_RADAR:		 Players::DrawRadar(m_pGui);
	}

	//m_pGui->PopCondition();
	m_pGui->EndChild();


	ImGui::Spacing(WINDOW_PADDING_Y - 1);

	child_size.y = GImGui->CurrentWindow->Size.y - ImGui::GetCursorPosY() - WINDOW_PADDING_X;

	m_pGui->BeginChild("Colored models", child_size);
	m_pGui->FeaturesList(ColoredModelsFeatures, IM_ARRAYSIZE(ColoredModelsFeatures), &selected_colored_models, &hovered_colored_models, features_list_item_width);
	//m_pGui->PushCondition(cvars::visuals.active);

	switch (selected_colored_models)
	{
	case COLORED_MODELS_GLOBALS:	ColoredModels::DrawGlobals(m_pGui); break;
	case COLORED_MODELS_PLAYERS:	ColoredModels::DrawPlayers(m_pGui); break;
	case COLORED_MODELS_BACKTRACK:  ColoredModels::DrawBacktrack(m_pGui); break;
	case COLORED_MODELS_HANDS:		ColoredModels::DrawHands(m_pGui); break;
	case COLORED_MODELS_DLIGHT:		ColoredModels::DrawDlight(m_pGui); break;
	case COLORED_MODELS_ELIGHT:		ColoredModels::DrawElight(m_pGui);
	}

	//m_pGui->PopCondition();
	m_pGui->EndChild();

	ImGui::EndGroup();
	ImGui::SameLine(0, WINDOW_PADDING_X);
	ImGui::BeginGroup();

	child_size.y = (GImGui->CurrentWindow->Size.y - ImGui::GetCursorPosY() - WINDOW_PADDING_X) * 0.60f;

	m_pGui->BeginChild("Other ESP", child_size);
	m_pGui->FeaturesList(OtherFeatures, IM_ARRAYSIZE(OtherFeatures), &selected_other, &hovered_other, features_list_item_width);
	//m_pGui->PushCondition(cvars::visuals.active);

	switch (selected_other)
	{
	case OTHER_ESP_WORLD: Other::DrawWorld(m_pGui); break;
	case OTHER_ESP_LOCAL: Other::DrawLocal(m_pGui);
	}

	//m_pGui->PopCondition();
	m_pGui->EndChild();

	ImGui::Spacing(WINDOW_PADDING_Y - 1);

	child_size.y = GImGui->CurrentWindow->Size.y - ImGui::GetCursorPosY() - WINDOW_PADDING_X;

	m_pGui->BeginChild("Effects", child_size);
	m_pGui->FeaturesList(EffectsFeatures, IM_ARRAYSIZE(EffectsFeatures), &selected_effects, &hovered_effects, features_list_item_width);
	//m_pGui->PushCondition(cvars::visuals.active);

	switch (selected_effects)
	{
	case EFFECTS_EFFECTS:  Effects::DrawEffects(m_pGui); break;
	case EFFECTS_REMOVALS: Effects::DrawRemovals(m_pGui);
	}

	//m_pGui->PopCondition();
	m_pGui->EndChild();

	ImGui::EndGroup();*/
}