#include "menu.h"

#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"
#include "draw_manager.h"
#include "render.h"
#include "values.h"
#include "keys.h"

#include <mutex>
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

namespace FGInternal {
	namespace TAB {
		int tab = 0;
		bool main_tab_0 = true;
		bool main_tab_1 = false;
		bool main_tab_2 = false;
		bool main_tab_3 = false;
		bool main_tab_4 = false;
		bool main_tab_5 = false;
	};

	namespace GENERAL {
		bool hide_corner_text = false;
		bool show_enabled_features = false;
		bool enable_unhook_key = true;
		bool enable_panic_key = true;
		bool disable_safe_values = false;
		float min_unsafe_value = 0.f;
		float max_unsafe_value = 2137.f;
		bool instant_win = false;
		bool reset_button = false;
	};

	namespace ESP {
		bool enable_players_esp = false;
		bool correct_doors_enabled = true;
		bool correct_path_enabled = true;
		bool non_jinxed_players_enabled = true;
		bool non_jinxed_enemies_enabled = true;
		bool non_jinxed_allies_enabled = false;
		bool show_all_platforms_enabled = true;
		bool show_player_with_tail = true;

		// COLORS

		unsigned int players_esp_color = 0xFF7230DB;
		unsigned int correct_doors_color = 0xFF589C21;
		unsigned int correct_path_color = 0xFFDE3E5B;
		unsigned int jinxed_enemies_color = 0xFF7230DB;
		unsigned int jinxed_allies_color = 0xFF94DB30;
		unsigned int player_with_tail_color = 0xFF94DB30;

		float col_test[] = { 0.45f, 0.55f, 0.60f, 1.00f };
	};

	namespace MOVEMENT {
		bool fly_enabled = false;
		float fly_speed = 15.f;

		bool speed_enabled = false;
		float ms_normal_boost = 9.5f;
		float ms_carry_boost = 8.f;
		float ms_grabbing_boost = 5.f;
		float ms_rolling_boost = 7.f;

		bool dive_enabled = false;
		float normalDive_speed = 16.5f;
		float airDive_speed = 7.f;

		bool gravity_enabled = false;
		float gravity_scale = 1.5f;
	};

	namespace JUMP {
		bool no_jump_delay = false;
		bool auto_bunny_hop = false;
	}

	namespace COLLISIONS {
		bool stun_collision = false;
		bool object_collision = false;
		bool player_collision = false;
		bool falling_down = false;

		bool unintentional_move_speed = false;
		float unintentional_move_speed_value = 0.75f;
		float unintentional_move_speed_emote_value = 1.5f;
	};

	namespace CARRY {
		bool carryDropForce = false;
		float carryNormalDropBoost = 50.f;
		float carryDiveDropBoost = 90.f;
		bool carryTussleChances = false;
	};

	namespace GRAB {
		bool supergrabfeature_enabled = false;

		bool grabDetectRadius = false;
		float grabDetectRadiusBoost = 6.f;
		
		bool grabCheckDistance = false;
		float grabCheckDistanceBoost = 2.f;

		bool grabMaxForce = false;
		float grabMaxForceBoost = 0.6f;

		bool grabBreakTime = false;
		float grabBreakTimeBoost = 1.2f;
		float grabBreakTimeJumpBoost = 0.01f;

		bool grabCheckPredictionBase = false;
		float grabCheckPredictionBaseBoost = 0.1f;

		bool grabImmediateVelocityReduction = false;
		float grabImmediateVelocityReductionBoost = 0.5f;

		bool grabDragDirectionContribution = false;
		float grabDragDirectionContributionBoost = 0.5f;

		bool grabCooldown = false;
		float grabCooldownBoost = 0.5f;

		bool grabRegrabDelay = false;
		float grabRegrabDelayBoost = 2.f;

		bool grabReleaseRegrabCooldown = false;
		float grabReleaseRegrabCooldownBoost = 1.f;

		bool grabBreakAngle = false;
		float grabBreakAngleBoost = 75;

		bool grabSeparationForce = false;
		float grabSeparationForceBoost = 7.f;

		bool grabberVelocity = false;
		float grabberVelocityBoost = 0.1f;
	};
};

namespace menu {
	bool main_tab_active = true;

	void push_color_for_button(bool active, const char* type) {
		if (type == "tab") {
			if (active) {
				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 222.f / 255.f, 164.f / 255.f, 40.f / 255.f, 1.f });
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 222.f / 255.f, 164.f / 255.f, 40.f / 255.f, 1.f });
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 222.f / 255.f, 164.f / 255.f, 40.f / 255.f, 1.f });
			} else {
				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 54.f / 255.f, 54.f / 255.f, 54.f / 255.f, 1.f });
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 237.f / 255.f, 177.f / 255.f, 47.f / 255.f, 1.f });
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 255.f, 194.f / 255.f, 64.f / 255.f, 1.f });
			}
		} else if (type == "normal") {
			if (active) {
				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 222.f / 255.f, 164.f / 255.f, 40.f / 255.f, 1.f });
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 237.f / 255.f, 177.f / 255.f, 47.f / 255.f, 1.f });
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 255.f, 194.f / 255.f, 64.f / 255.f, 1.f });
			} else {
				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 54.f / 255.f, 54.f / 255.f, 54.f / 255.f, 1.f });
				ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 237.f / 255.f, 177.f / 255.f, 47.f / 255.f, 1.f });
				ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 255.f, 194.f / 255.f, 64.f / 255.f, 1.f });
			}
		}
	}

	void draw_buttonTab(const char* name, bool& config_key, float width, int tab, bool* change_opositive = nullptr) {
		push_color_for_button(config_key, "tab");
		if (ImGui::Button(name, { width, 20 }))
		{
			if (config_key == true)
				return;

			FGInternal::TAB::tab = tab;

			if (config_key == FGInternal::TAB::main_tab_0) {
				FGInternal::TAB::main_tab_1 = false;
				FGInternal::TAB::main_tab_2 = false;
				FGInternal::TAB::main_tab_3 = false;
				FGInternal::TAB::main_tab_4 = false;
				FGInternal::TAB::main_tab_5 = false;
			} else if (config_key == FGInternal::TAB::main_tab_1) {
				FGInternal::TAB::main_tab_0 = false;
				FGInternal::TAB::main_tab_2 = false;
				FGInternal::TAB::main_tab_3 = false;
				FGInternal::TAB::main_tab_4 = false;
				FGInternal::TAB::main_tab_5 = false;
			} else if (config_key == FGInternal::TAB::main_tab_2) {
				FGInternal::TAB::main_tab_0 = false;
				FGInternal::TAB::main_tab_1 = false;
				FGInternal::TAB::main_tab_3 = false;
				FGInternal::TAB::main_tab_4 = false;
				FGInternal::TAB::main_tab_5 = false;
			} else if (config_key == FGInternal::TAB::main_tab_3) {
				FGInternal::TAB::main_tab_0 = false;
				FGInternal::TAB::main_tab_1 = false;
				FGInternal::TAB::main_tab_2 = false;
				FGInternal::TAB::main_tab_4 = false;
				FGInternal::TAB::main_tab_5 = false;
			} else if (config_key == FGInternal::TAB::main_tab_4) {
				FGInternal::TAB::main_tab_0 = false;
				FGInternal::TAB::main_tab_1 = false;
				FGInternal::TAB::main_tab_2 = false;
				FGInternal::TAB::main_tab_3 = false;
				FGInternal::TAB::main_tab_5 = false;
			} else if (config_key == FGInternal::TAB::main_tab_5) {
				FGInternal::TAB::main_tab_0 = false;
				FGInternal::TAB::main_tab_1 = false;
				FGInternal::TAB::main_tab_2 = false;
				FGInternal::TAB::main_tab_3 = false;
				FGInternal::TAB::main_tab_4 = false;
			}

			config_key = !config_key;
			if (change_opositive)
				*change_opositive = !config_key;
		}

		ImGui::PopStyleColor(3);
	}

	void draw_button(const char* name, bool& config_key, bool* change_opositive = nullptr) {
		push_color_for_button(config_key, "normal");
		if (ImGui::Button(name, { 359, 20 }))
		{
			config_key = !config_key;
			if (change_opositive)
				*change_opositive = !config_key;
		}

		ImGui::PopStyleColor(3);
	}

	void draw_buttonReset(const char* name, bool& config_key, bool* change_opositive = nullptr) {
		push_color_for_button(config_key, "normal");
		if (ImGui::Button(name, { 359, 20 }))
		{
			config_key = !config_key;
			if (change_opositive)
				*change_opositive = !config_key;
			// MOVEMENT
			FGInternal::MOVEMENT::fly_speed = 15.f;
			FGInternal::MOVEMENT::ms_normal_boost = VALUES::DEFAULT_VALUES::default_max_speed;
			FGInternal::MOVEMENT::ms_carry_boost = VALUES::DEFAULT_VALUES::default_carryMaxSpeed;
			FGInternal::MOVEMENT::ms_grabbing_boost = VALUES::DEFAULT_VALUES::default_grabbingMaxSpeed;
			FGInternal::MOVEMENT::ms_rolling_boost = VALUES::DEFAULT_VALUES::default_rollingMaxSpeed;
			FGInternal::MOVEMENT::normalDive_speed = VALUES::DEFAULT_VALUES::default_diveForce;
			FGInternal::MOVEMENT::airDive_speed = VALUES::DEFAULT_VALUES::default_airDiveForce;
			FGInternal::MOVEMENT::gravity_scale = VALUES::DEFAULT_VALUES::default_gravityScale;
			// COLLISIONS
			FGInternal::COLLISIONS::unintentional_move_speed_value = VALUES::DEFAULT_VALUES::default_unintentionalMoveSpeedThreshold;
			FGInternal::COLLISIONS::unintentional_move_speed_emote_value = VALUES::DEFAULT_VALUES::default_unintentionalMoveSpeedThresholdDuringEmote;
			// CARRY
			FGInternal::CARRY::carryNormalDropBoost = VALUES::DEFAULT_VALUES::default_carryNormalDropForce;
			FGInternal::CARRY::carryDiveDropBoost = VALUES::DEFAULT_VALUES::default_carryDiveDropForce;
			// GRAB
			FGInternal::GRAB::grabDetectRadiusBoost = VALUES::DEFAULT_VALUES::default_playerGrabDetectRadius;
			FGInternal::GRAB::grabCheckDistanceBoost = VALUES::DEFAULT_VALUES::default_playerGrabCheckDistance;
			FGInternal::GRAB::grabMaxForceBoost = VALUES::DEFAULT_VALUES::default_playerGrabberMaxForce;
			FGInternal::GRAB::grabBreakTimeBoost = VALUES::DEFAULT_VALUES::default_playerGrabBreakTime;
			FGInternal::GRAB::grabBreakTimeJumpBoost = VALUES::DEFAULT_VALUES::default_playerGrabBreakTimeJumpInfluence;
			FGInternal::GRAB::grabCheckPredictionBaseBoost = VALUES::DEFAULT_VALUES::default_playerGrabCheckPredictionBase;
			FGInternal::GRAB::grabImmediateVelocityReductionBoost = VALUES::DEFAULT_VALUES::default_playerGrabImmediateVelocityReduction;
			FGInternal::GRAB::grabDragDirectionContributionBoost = VALUES::DEFAULT_VALUES::default_playerGrabberDragDirectionContribution;
			FGInternal::GRAB::grabCooldownBoost = VALUES::DEFAULT_VALUES::default_grabCooldown;
			FGInternal::GRAB::grabRegrabDelayBoost = VALUES::DEFAULT_VALUES::default_playerGrabRegrabDelay;
			FGInternal::GRAB::grabReleaseRegrabCooldownBoost = VALUES::DEFAULT_VALUES::default_forceReleaseRegrabCooldown;
			FGInternal::GRAB::grabBreakAngleBoost = VALUES::DEFAULT_VALUES::default_breakGrabAngle;
			FGInternal::GRAB::grabberVelocityBoost = VALUES::DEFAULT_VALUES::default_playerGrabberVelocityComponent;
			FGInternal::GRAB::grabSeparationForceBoost = VALUES::DEFAULT_VALUES::default_playerGrabBreakSeparationForce;
			// CHECK
			FGInternal::GENERAL::reset_button = false;
		}

		ImGui::PopStyleColor(3);
	}

	void draw_buttonDisabled(const char* name, bool config_key = false, bool* change_opositive = nullptr) {
		push_color_for_button(config_key, "normal");
		ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
		if (!ImGui::Button(name, { 359, 25 }))
			return;

		ImGui::PopItemFlag();
		ImGui::PopStyleColor(3);
	}

	void draw_sliderDisabled(const char* name, float* val, float min, float max, const char* display_format = "%.2f") {
		ImGui::PushStyleColor(ImGuiCol_SliderGrab, ImVec4{ 125.f / 255.f, 122.f / 255.f, 115.f / 255.f, 1.f });
		ImGui::PushStyleColor(ImGuiCol_SliderGrabActive, ImVec4{ 125.f / 255.f, 122.f / 255.f, 115.f / 255.f, 1.f });

		if (!display_format)
			display_format = "%.2f";

		ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
		ImGui::SliderFloat(name, val, min, max, display_format);

		ImGui::PopItemFlag();
		ImGui::PopStyleColor(2);
	}

	void draw_slider(const char* name, float* val, float min, float max, const char* display_format = "%.2f") {
		ImGui::PushStyleColor(ImGuiCol_SliderGrab, ImVec4{ 222.f / 255.f, 164.f / 255.f, 40.f / 255.f, 1.f });
		ImGui::PushStyleColor(ImGuiCol_SliderGrabActive, ImVec4{ 237.f / 255.f, 177.f / 255.f, 47.f / 255.f, 1.f });

		if (!display_format)
			display_format = "%.2f";

		ImGui::SliderFloat(name, val, min, max, display_format);

		ImGui::PopStyleColor(2);
	}

	void draw_tab(const char* name, bool& active) {
		std::string tab_name = name;
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (ImGui::GetColumnWidth() / 2) - (ImGui::CalcTextSize(tab_name.c_str()).x / 2)
			- ImGui::GetScrollX() - 2 * (ImGui::GetStyle().ItemSpacing.x / 2) + 8);
		ImGui::Text(name);
		ImGui::SameLine(745);

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 222.f / 255.f, 164.f / 255.f, 40.f / 255.f, 1.f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 237.f / 255.f, 177.f / 255.f, 47.f / 255.f, 1.f });
		//if (ImGui::Button("", { 10, 10 }))
		//	active = !active;

		ImGui::PopStyleColor(2);
	}

	std::once_flag init_style;
	void draw() {
		std::call_once(init_style, [&]() {
			auto& style = ImGui::GetStyle();
			style.WindowRounding = 0.f;
			style.FrameRounding = 0.f;
			style.ItemSpacing = ImVec2(8, 2);
			style.WindowPadding = ImVec2(3.f, 3.f);
			style.Colors[ImGuiCol_FrameBg] = ImVec4{ 35.f / 255.f, 35.f / 255.f, 35.f / 255.f, 1.f };
			style.Colors[ImGuiCol_FrameBgActive] = ImVec4{ 46.f / 255.f, 46.f / 255.f, 46.f / 255.f, 1.f };
			style.Colors[ImGuiCol_FrameBgHovered] = ImVec4{ 46.f / 255.f, 46.f / 255.f, 46.f / 255.f, 1.f };
			style.Colors[ImGuiCol_SliderGrabActive] = ImVec4{ 222.f / 255.f, 164.f / 255.f, 40.f / 255.f, 1.f };
			style.Colors[ImGuiCol_SliderGrab] = ImVec4{ 237.f / 255.f, 177.f / 255.f, 47.f / 255.f, 1.f };
			style.Colors[ImGuiCol_CheckMark] = ImVec4{ 222.f / 255.f, 164.f / 255.f, 40.f / 255.f, 1.f };
			});

		ImGui::Begin("main_tab", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar);
		ImGui::SetWindowSize({ 758, 450 }, ImGuiCond_Always);

		if (GetAsyncKeyState(VK_PRIOR)) {
			ImGui::SetWindowPos(ImVec2(0, 0), true);
		}

		draw_tab(u8"» FALL GUYS PREMIUM «", main_tab_active);
		if (main_tab_active) {
			ImGui::NewLine();
			draw_buttonTab("GENERAL", FGInternal::TAB::main_tab_0, 144, 0);
			ImGui::SameLine();
			draw_buttonTab("MOVEMENT", FGInternal::TAB::main_tab_1, 144, 1);
			ImGui::SameLine();
			draw_buttonTab("GRAB FEATURES", FGInternal::TAB::main_tab_2, 144, 2);
			ImGui::SameLine();
			draw_buttonTab("MISCELLANEOUS", FGInternal::TAB::main_tab_3, 144, 3);
			ImGui::SameLine();
			draw_buttonTab("CONFIG", FGInternal::TAB::main_tab_4, 144, 4);
			ImGui::Dummy(ImVec2(0.0f, 2.5f));

			// visuals
			if (FGInternal::TAB::tab == 0) {
				ImGui::BeginChild("1_Tab", ImVec2(752, 402), true);
				ImGui::Columns(2);
				//-->
				std::string GENERAL = "GENERAL";
				ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (ImGui::GetColumnWidth() / 2) - (ImGui::CalcTextSize(GENERAL.c_str()).x / 2)
					- ImGui::GetScrollX() - 2 * (ImGui::GetStyle().ItemSpacing.x / 2));
				ImGui::Text("%s", GENERAL);
				ImGui::NextColumn();
				std::string VISUALS = "VISUALS";
				ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (ImGui::GetColumnWidth() / 2) - (ImGui::CalcTextSize(VISUALS.c_str()).x / 2)
					- ImGui::GetScrollX() - 2 * (ImGui::GetStyle().ItemSpacing.x / 2));
				ImGui::Text("%s", VISUALS);
				ImGui::NextColumn();
				ImGui::Separator();
				//-->


				ImGui::Dummy(ImVec2(0.0f, 10.0f));

				ImGui::Checkbox(u8"HOME » Hide Corner Menu", &FGInternal::GENERAL::hide_corner_text);
				if (ImGui::IsItemHovered())
					ImGui::SetTooltip("Checking this option disables the visibility of\nthe menu displayed on the left side of the screen.");

				ImGui::Dummy(ImVec2(0.0f, 7.5f));

				ImGui::Checkbox(u8"PAGE UP » Show Enabled Features", &FGInternal::GENERAL::show_enabled_features);
				if (ImGui::IsItemHovered())
					ImGui::SetTooltip("Enabling this option will display information about\nthe currently enabled functions in the corner of the screen.");

				ImGui::Dummy(ImVec2(0.0f, 7.5f));

				ImGui::Checkbox(u8"Unsafe Values", &FGInternal::GENERAL::disable_safe_values);
				if (ImGui::IsItemHovered())
					ImGui::SetTooltip("Checking this option enables the ability\nto customize all available cheats above\nthe default recommended value.");

				ImGui::Dummy(ImVec2(0.0f, 2.5f));

				if (FGInternal::GENERAL::disable_safe_values) {
					draw_slider("Min. Unsafe Value", &FGInternal::GENERAL::min_unsafe_value, 0, FGInternal::GENERAL::max_unsafe_value - 1);
					draw_slider("Max. Unsafe Value", &FGInternal::GENERAL::max_unsafe_value, FGInternal::GENERAL::min_unsafe_value + 1, 9999);
				} else {
					draw_sliderDisabled(u8"DISABLED » Min. Unsafe Value", &FGInternal::GENERAL::min_unsafe_value, 0, FGInternal::GENERAL::max_unsafe_value - 1);
					draw_sliderDisabled(u8"DISABLED » Max. Unsafe Value", &FGInternal::GENERAL::max_unsafe_value, FGInternal::GENERAL::min_unsafe_value + 1, 9999);
				}

				ImGui::Dummy(ImVec2(0.0f, 7.5f));

				draw_buttonReset("Reset All Values", FGInternal::GENERAL::reset_button);

				ImGui::Dummy(ImVec2(0.0f, 15.f));

				draw_buttonTab("Changelog", FGInternal::TAB::main_tab_5, 359, 5);

				ImGui::NextColumn();
				//-->

				ImGui::Dummy(ImVec2(0.0f, 10.0f));

				//ImGui::ColorPicker3("test", FGInternal::ESP::col_test);
				//ImGui::SameLine();
				ImGui::Checkbox(u8"F5 » Players ESP", &FGInternal::ESP::enable_players_esp);
				if (ImGui::IsItemHovered())
					ImGui::SetTooltip("Activating this function will show\nthe outline on all players on the map!");

				ImGui::Dummy(ImVec2(0.0f, 7.5f));

				ImGui::Checkbox(u8"F6 » Reveal Correct Doors", &FGInternal::ESP::correct_doors_enabled);
				if (ImGui::IsItemHovered())
					ImGui::SetTooltip("Activating this function will show the correct\ndoor that can be destroyed in Doors Rush minigame!");

				ImGui::Dummy(ImVec2(0.0f, 7.5f));

				ImGui::Checkbox(u8"F7 » Reveal TipToe Path", &FGInternal::ESP::correct_path_enabled);
				if (ImGui::IsItemHovered())
					ImGui::SetTooltip("Activating this function will show valid\nfields to go through in TipToe minigame!");

				ImGui::Dummy(ImVec2(0.0f, 7.5f));

				ImGui::Checkbox(u8"F8 » Non-Jinxed Players", &FGInternal::ESP::non_jinxed_players_enabled);
				if (ImGui::IsItemHovered())
					ImGui::SetTooltip("Activating this feature will show players\nthat were not infected in Jinxed!");

				ImGui::Dummy(ImVec2(0.0f, 2.5f));

				if (FGInternal::ESP::non_jinxed_players_enabled) {
					ImGui::Checkbox(u8"   » Non-Jinxed Enemies", &FGInternal::ESP::non_jinxed_enemies_enabled);
					if (ImGui::IsItemHovered())
						ImGui::SetTooltip("Activating this feature will show enemies\nthat were not infected in Jinxed!");

					ImGui::Dummy(ImVec2(0.0f, 2.5f));

					ImGui::Checkbox(u8"   » Non-Jinxed Allies", &FGInternal::ESP::non_jinxed_allies_enabled);
					if (ImGui::IsItemHovered())
						ImGui::SetTooltip("Activating this feature will show allies\nthat were not infected in Jinxed!");
				} else {
					ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
					ImGui::Checkbox(u8"   » Non-Jinxed Enemies", &FGInternal::ESP::non_jinxed_enemies_enabled);
					if (ImGui::IsItemHovered())
						ImGui::SetTooltip("Activating this feature will show enemies\nthat were not infected in Jinxed!");

					ImGui::Dummy(ImVec2(0.0f, 2.5f));

					ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
					ImGui::Checkbox(u8"   » Non-Jinxed Allies", &FGInternal::ESP::non_jinxed_allies_enabled);
					if (ImGui::IsItemHovered())
						ImGui::SetTooltip("Activating this feature will show allies\nthat were not infected in Jinxed!");

					ImGui::PushItemFlag(ImGuiItemFlags_Disabled, false);
				}

				ImGui::Dummy(ImVec2(0.0f, 7.5f));

				ImGui::Checkbox(u8"F9 » Reveal All Platforms", &FGInternal::ESP::show_all_platforms_enabled);
				if (ImGui::IsItemHovered())
					ImGui::SetTooltip("Activating this function will show all\nfruit picture boxes in Match Fall mode.");

				ImGui::Dummy(ImVec2(0.0f, 7.5f));

				ImGui::Checkbox(u8"F10 » Show Player with Tail", &FGInternal::ESP::show_player_with_tail);
				if (ImGui::IsItemHovered())
					ImGui::SetTooltip("Activating this feature will show\nthe player who has the tail needed\nto win the game in Royal Rumble!");

				ImGui::Dummy(ImVec2(0.0f, 146.f));

				ImGui::EndChild();
			}

			// movement
			if (FGInternal::TAB::tab == 1) {
				ImGui::BeginChild("2_Tab", ImVec2(752, 402), true);
				ImGui::Columns(2);
				//-->
				std::string MOVEMENT = "MOVEMENT";
				ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (ImGui::GetColumnWidth() / 2) - (ImGui::CalcTextSize(MOVEMENT.c_str()).x / 2)
					- ImGui::GetScrollX() - 2 * (ImGui::GetStyle().ItemSpacing.x / 2));
				ImGui::Text("%s", MOVEMENT);
				ImGui::NextColumn();
				std::string JUMP = "JUMP_&_COLLISIONS";
				ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (ImGui::GetColumnWidth() / 2) - (ImGui::CalcTextSize(JUMP.c_str()).x / 2)
					- ImGui::GetScrollX() - 2 * (ImGui::GetStyle().ItemSpacing.x / 2));
				ImGui::Text("JUMP & COLLISIONS");
				ImGui::NextColumn();
				ImGui::Separator();
				//-->

				if (!FGInternal::GENERAL::disable_safe_values) {
					ImGui::Dummy(ImVec2(0.0f, 10.0f));

					ImGui::Checkbox(u8"F1 » Flying Mode", &FGInternal::MOVEMENT::fly_enabled);
					if (ImGui::IsItemHovered())
						ImGui::SetTooltip("Turning on the flying mode will raise your\ncharacter above the map level, you can\nfreely adjust the flight speed, and you use the\nMOUSE, W, SPACE and SHIFT to control.\n\nSPACE [PS4 - X | XBOX - A] - Fly up,\nSHIFT [PS4 - O | XBOX - B] - Fly down,\nSHIFT+C [PS4 - O+R2 | XBOX - B+RT] - Grab without flying down.");
					ImGui::Dummy(ImVec2(0.0f, 2.5f));
					if (FGInternal::MOVEMENT::fly_enabled) {
						draw_slider("Flying Speed", &FGInternal::MOVEMENT::fly_speed, VALUES::SAFE_VALUES::MOVEMENT::fly_speed_min, VALUES::SAFE_VALUES::MOVEMENT::fly_speed_max);
					} else {
						draw_sliderDisabled(u8"DISABLED » Flying Speed", &FGInternal::MOVEMENT::fly_speed, VALUES::SAFE_VALUES::MOVEMENT::fly_speed_min, VALUES::SAFE_VALUES::MOVEMENT::fly_speed_max);
					}

					ImGui::Dummy(ImVec2(0.0f, 7.5f));

					ImGui::Checkbox(u8"F2 » Speed Hack", &FGInternal::MOVEMENT::speed_enabled);
					if (ImGui::IsItemHovered())
						ImGui::SetTooltip("Speedhack will speed up or slow\ndown your character, you decide!");
					ImGui::Dummy(ImVec2(0.0f, 2.5f));
					if (FGInternal::MOVEMENT::speed_enabled) {
						draw_slider("Normal Speed", &FGInternal::MOVEMENT::ms_normal_boost, VALUES::SAFE_VALUES::MOVEMENT::ms_normal_min, VALUES::SAFE_VALUES::MOVEMENT::ms_normal_max);
						draw_slider("Carrying Speed", &FGInternal::MOVEMENT::ms_carry_boost, VALUES::SAFE_VALUES::MOVEMENT::ms_carry_min, VALUES::SAFE_VALUES::MOVEMENT::ms_carry_max);
						draw_slider("Grabbing Speed", &FGInternal::MOVEMENT::ms_grabbing_boost, VALUES::SAFE_VALUES::MOVEMENT::ms_grabbing_min, VALUES::SAFE_VALUES::MOVEMENT::ms_grabbing_max);
						draw_slider("Rolling Speed", &FGInternal::MOVEMENT::ms_rolling_boost, VALUES::SAFE_VALUES::MOVEMENT::ms_rolling_min, VALUES::SAFE_VALUES::MOVEMENT::ms_rolling_max);
					} else {
						draw_sliderDisabled(u8"DISABLED » Normal Speed", &FGInternal::MOVEMENT::ms_normal_boost, VALUES::SAFE_VALUES::MOVEMENT::ms_normal_min, VALUES::SAFE_VALUES::MOVEMENT::ms_normal_max);
						draw_sliderDisabled(u8"DISABLED » Carrying Speed", &FGInternal::MOVEMENT::ms_carry_boost, VALUES::SAFE_VALUES::MOVEMENT::ms_carry_min, VALUES::SAFE_VALUES::MOVEMENT::ms_carry_max);
						draw_sliderDisabled(u8"DISABLED » Grabbing Speed", &FGInternal::MOVEMENT::ms_grabbing_boost, VALUES::SAFE_VALUES::MOVEMENT::ms_grabbing_min, VALUES::SAFE_VALUES::MOVEMENT::ms_grabbing_max);
						draw_sliderDisabled(u8"DISABLED » Rolling Speed", &FGInternal::MOVEMENT::ms_rolling_boost, VALUES::SAFE_VALUES::MOVEMENT::ms_rolling_min, VALUES::SAFE_VALUES::MOVEMENT::ms_rolling_max);
					}

					ImGui::Dummy(ImVec2(0.0f, 7.5f));

					ImGui::Checkbox(u8"F3 » Dive", &FGInternal::MOVEMENT::dive_enabled);
					if (ImGui::IsItemHovered())
						ImGui::SetTooltip("During the dive, the speed depends on the\nposition you are in, you can adjust it as you see\nfit by decreasing or increasing the value.");
					ImGui::Dummy(ImVec2(0.0f, 2.5f));
					if (FGInternal::MOVEMENT::dive_enabled) {
						draw_slider("Normal Dive", &FGInternal::MOVEMENT::normalDive_speed, VALUES::SAFE_VALUES::MOVEMENT::normalDive_speed_min, VALUES::SAFE_VALUES::MOVEMENT::normalDive_speed_max);
						draw_slider("Air Dive", &FGInternal::MOVEMENT::airDive_speed, VALUES::SAFE_VALUES::MOVEMENT::airDive_speed_min, VALUES::SAFE_VALUES::MOVEMENT::airDive_speed_max);
					} else {
						draw_sliderDisabled(u8"DISABLED » Normal Dive", &FGInternal::MOVEMENT::normalDive_speed, VALUES::SAFE_VALUES::MOVEMENT::normalDive_speed_min, VALUES::SAFE_VALUES::MOVEMENT::normalDive_speed_max);
						draw_sliderDisabled(u8"DISABLED » Air Dive", &FGInternal::MOVEMENT::airDive_speed, VALUES::SAFE_VALUES::MOVEMENT::airDive_speed_min, VALUES::SAFE_VALUES::MOVEMENT::airDive_speed_max);
					}

					ImGui::Dummy(ImVec2(0.0f, 7.5f));

					ImGui::Checkbox(u8"F4 » Gravitation [Higher Jumps]", &FGInternal::MOVEMENT::gravity_enabled);
					if (ImGui::IsItemHovered())
						ImGui::SetTooltip("Gravity will increase or decrease the height\nof your jump, it works perfectly with\nhe speed of movement of the character.\n\n\nLOWER VALUE = LOWER GRAVITATION");
					ImGui::Dummy(ImVec2(0.0f, 2.5f));
					if (FGInternal::MOVEMENT::gravity_enabled) {
						draw_slider("Gravity Boost", &FGInternal::MOVEMENT::gravity_scale, VALUES::SAFE_VALUES::MOVEMENT::gravity_scale_min, VALUES::SAFE_VALUES::MOVEMENT::gravity_scale_max);
					}
					else {
						draw_sliderDisabled(u8"DISABLED » Gravity Boost", &FGInternal::MOVEMENT::gravity_scale, VALUES::SAFE_VALUES::MOVEMENT::gravity_scale_min, VALUES::SAFE_VALUES::MOVEMENT::gravity_scale_max);
					}

					ImGui::NextColumn();
					//-->

					ImGui::Dummy(ImVec2(0.0f, 10.0f));


					ImGui::Checkbox(u8"Auto Bunny Hop", &FGInternal::JUMP::auto_bunny_hop);
					if (ImGui::IsItemHovered())
						ImGui::SetTooltip("Holding the M3 (scroll) button will cause\nthe character to jump continuously.\n\nRequires Disable Jump Delay to be enabled!");

					ImGui::Dummy(ImVec2(0.0f, 7.5f));

					ImGui::Checkbox(u8"Disable Jump Delay", &FGInternal::JUMP::no_jump_delay);
					if (ImGui::IsItemHovered())
						ImGui::SetTooltip("Removing the delay on the jump will allow\nyou to jump without limit, all the time.");

					ImGui::Dummy(ImVec2(0.0f, 35.f));

					ImGui::Checkbox(u8"Disable Stuns/Knockdowns", &FGInternal::COLLISIONS::stun_collision);
					if (ImGui::IsItemHovered())
						ImGui::SetTooltip("When you are pulled by another player or fall\nfrom a height, your character will pass out\nfor a few hundredths of a second, you can minimize it.");

					ImGui::Dummy(ImVec2(0.0f, 7.5f));

					ImGui::Checkbox(u8"Disable Objects Collisions", &FGInternal::COLLISIONS::object_collision);
					if (ImGui::IsItemHovered())
						ImGui::SetTooltip("As long as the function is enabled, your\ncharacter will not be able to fall over\nafter hitting an object (e.g. a moving wall)");

					ImGui::Dummy(ImVec2(0.0f, 7.5f));

					ImGui::Checkbox(u8"Player to Player Collisions", &FGInternal::COLLISIONS::player_collision);
					if (ImGui::IsItemHovered())
						ImGui::SetTooltip("When the feature remains on, each time you collide\nwith another player while running or after falling,\nyour character will not fall to the ground.");

					ImGui::Dummy(ImVec2(0.0f, 7.5f));

					ImGui::Checkbox(u8"Always Fall on Legs", &FGInternal::COLLISIONS::falling_down);
					if (ImGui::IsItemHovered())
						ImGui::SetTooltip("Each time you fall from a height,\nyour character will stand on its\nfeet instead of falling to the ground.");

					ImGui::Dummy(ImVec2(0.0f, 7.5f));

					ImGui::Checkbox(u8"Unintentional Move Speed", &FGInternal::COLLISIONS::unintentional_move_speed);
					if (ImGui::IsItemHovered())
						ImGui::SetTooltip("The unintentional speed of a character movement occurs\nwhen he is grabbed by another player and moved towards him,\nsetting this function to 0.01 will keep the character always standing still.");
					ImGui::Dummy(ImVec2(0.0f, 2.5f));
					if (FGInternal::COLLISIONS::unintentional_move_speed) {
						draw_slider("Normal Threshold", &FGInternal::COLLISIONS::unintentional_move_speed_value, VALUES::SAFE_VALUES::COLLISIONS::unintentional_move_speed_min, VALUES::SAFE_VALUES::COLLISIONS::unintentional_move_speed_max);
						draw_slider("During Emote Threshold", &FGInternal::COLLISIONS::unintentional_move_speed_emote_value, VALUES::SAFE_VALUES::COLLISIONS::unintentional_move_speed_emote_min, VALUES::SAFE_VALUES::COLLISIONS::unintentional_move_speed_emote_max);
					} else {
						draw_sliderDisabled(u8"DISABLED » Normal Threshold", &FGInternal::COLLISIONS::unintentional_move_speed_value, VALUES::SAFE_VALUES::COLLISIONS::unintentional_move_speed_min, VALUES::SAFE_VALUES::COLLISIONS::unintentional_move_speed_max);
						draw_sliderDisabled(u8"DISABLED » During Emote Threshold", &FGInternal::COLLISIONS::unintentional_move_speed_emote_value, VALUES::SAFE_VALUES::COLLISIONS::unintentional_move_speed_emote_min, VALUES::SAFE_VALUES::COLLISIONS::unintentional_move_speed_emote_max);
					}
				} else {
					ImGui::Dummy(ImVec2(0.0f, 10.0f));

					ImGui::Checkbox(u8"F1 » Flying Mode", &FGInternal::MOVEMENT::fly_enabled);
					if (ImGui::IsItemHovered())
						ImGui::SetTooltip("Turning on the flying mode will raise your\ncharacter above the map level, you can\nfreely adjust the flight speed, and you use the\nMOUSE, W, SPACE and SHIFT to control.\n\nSPACE [PS4 - X | XBOX - A] - Fly up,\nSHIFT [PS4 - O | XBOX - B] - Fly down,\nSHIFT+C [PS4 - O+R2 | XBOX - B+RT] - Grab without flying down.");
					ImGui::Dummy(ImVec2(0.0f, 2.5f));
					if (FGInternal::MOVEMENT::fly_enabled) {
						draw_slider("Flying Speed", &FGInternal::MOVEMENT::fly_speed, FGInternal::GENERAL::min_unsafe_value, FGInternal::GENERAL::max_unsafe_value);
					} else {
						draw_sliderDisabled(u8"DISABLED » Flying Speed", &FGInternal::MOVEMENT::fly_speed, FGInternal::GENERAL::min_unsafe_value, FGInternal::GENERAL::max_unsafe_value);
					}

					ImGui::Dummy(ImVec2(0.0f, 7.5f));

					ImGui::Checkbox(u8"F2 » Speed Hack", &FGInternal::MOVEMENT::speed_enabled);
					if (ImGui::IsItemHovered())
						ImGui::SetTooltip("Speedhack will speed up or slow\ndown your character, you decide!");
					ImGui::Dummy(ImVec2(0.0f, 2.5f));
					if (FGInternal::MOVEMENT::speed_enabled) {
						draw_slider("Normal Speed", &FGInternal::MOVEMENT::ms_normal_boost, FGInternal::GENERAL::min_unsafe_value, FGInternal::GENERAL::max_unsafe_value);
						draw_slider("Carrying Speed", &FGInternal::MOVEMENT::ms_carry_boost, FGInternal::GENERAL::min_unsafe_value, FGInternal::GENERAL::max_unsafe_value);
						draw_slider("Grabbing Speed", &FGInternal::MOVEMENT::ms_grabbing_boost, FGInternal::GENERAL::min_unsafe_value, FGInternal::GENERAL::max_unsafe_value);
						draw_slider("Rolling Speed", &FGInternal::MOVEMENT::ms_rolling_boost, FGInternal::GENERAL::min_unsafe_value, FGInternal::GENERAL::max_unsafe_value);
					} else {
						draw_sliderDisabled(u8"DISABLED » Normal Speed", &FGInternal::MOVEMENT::ms_normal_boost, FGInternal::GENERAL::min_unsafe_value, FGInternal::GENERAL::max_unsafe_value);
						draw_sliderDisabled(u8"DISABLED » Carrying Speed", &FGInternal::MOVEMENT::ms_carry_boost, FGInternal::GENERAL::min_unsafe_value, FGInternal::GENERAL::max_unsafe_value);
						draw_sliderDisabled(u8"DISABLED » Grabbing Speed", &FGInternal::MOVEMENT::ms_grabbing_boost, FGInternal::GENERAL::min_unsafe_value, FGInternal::GENERAL::max_unsafe_value);
						draw_sliderDisabled(u8"DISABLED » Rolling Speed", &FGInternal::MOVEMENT::ms_rolling_boost, FGInternal::GENERAL::min_unsafe_value, FGInternal::GENERAL::max_unsafe_value);
					}

					ImGui::Dummy(ImVec2(0.0f, 7.5f));

					ImGui::Checkbox(u8"F3 » Dive", &FGInternal::MOVEMENT::dive_enabled);
					if (ImGui::IsItemHovered())
						ImGui::SetTooltip("During the dive, the speed depends on the\nposition you are in, you can adjust it as you see\nfit by decreasing or increasing the value.");
					ImGui::Dummy(ImVec2(0.0f, 2.5f));
					if (FGInternal::MOVEMENT::dive_enabled) {
						draw_slider("Normal Dive", &FGInternal::MOVEMENT::normalDive_speed, FGInternal::GENERAL::min_unsafe_value, FGInternal::GENERAL::max_unsafe_value);
						draw_slider("Air Dive", &FGInternal::MOVEMENT::airDive_speed, FGInternal::GENERAL::min_unsafe_value, FGInternal::GENERAL::max_unsafe_value);
					} else {
						draw_sliderDisabled(u8"DISABLED » Normal Dive", &FGInternal::MOVEMENT::normalDive_speed, FGInternal::GENERAL::min_unsafe_value, FGInternal::GENERAL::max_unsafe_value);
						draw_sliderDisabled(u8"DISABLED » Air Dive", &FGInternal::MOVEMENT::airDive_speed, FGInternal::GENERAL::min_unsafe_value, FGInternal::GENERAL::max_unsafe_value);
					}

					ImGui::Dummy(ImVec2(0.0f, 7.5f));

					ImGui::Checkbox(u8"F4 » Gravitation [Higher Jumps]", &FGInternal::MOVEMENT::gravity_enabled);
					if (ImGui::IsItemHovered())
						ImGui::SetTooltip("Gravity will increase or decrease the height\nof your jump, it works perfectly with\nhe speed of movement of the character.\n\n\nLOWER VALUE = LOWER GRAVITATION");
					ImGui::Dummy(ImVec2(0.0f, 2.5f));
					if (FGInternal::MOVEMENT::gravity_enabled) {
						draw_slider("Gravity Boost", &FGInternal::MOVEMENT::gravity_scale, FGInternal::GENERAL::min_unsafe_value, FGInternal::GENERAL::max_unsafe_value);
					} else {
						draw_sliderDisabled(u8"DISABLED » Gravity Boost", &FGInternal::MOVEMENT::gravity_scale, FGInternal::GENERAL::min_unsafe_value, FGInternal::GENERAL::max_unsafe_value);
					}

					ImGui::NextColumn();
					//-->

					ImGui::Dummy(ImVec2(0.0f, 10.0f));

					ImGui::Checkbox(u8"Auto Bunny Hop", &FGInternal::JUMP::auto_bunny_hop);
					if (ImGui::IsItemHovered())
						ImGui::SetTooltip("Holding the M3 (scroll) button will cause\nthe character to jump continuously.\n\nRequires Disable Jump Delay to be enabled!");

					ImGui::Dummy(ImVec2(0.0f, 7.5f));

					ImGui::Checkbox(u8"Disable Jump Delay", &FGInternal::JUMP::no_jump_delay);
					if (ImGui::IsItemHovered())
						ImGui::SetTooltip("Removing the delay on the jump will allow\nyou to jump without limit, all the time.");

					ImGui::Dummy(ImVec2(0.0f, 35.f));

					ImGui::Checkbox(u8"Disable Stuns/Knockdowns", &FGInternal::COLLISIONS::stun_collision);
					if (ImGui::IsItemHovered())
						ImGui::SetTooltip("When you are pulled by another player or fall\nfrom a height, your character will pass out\nfor a few hundredths of a second, you can minimize it.");

					ImGui::Dummy(ImVec2(0.0f, 7.5f));

					ImGui::Checkbox(u8"Disable Objects Collisions", &FGInternal::COLLISIONS::object_collision);
					if (ImGui::IsItemHovered())
						ImGui::SetTooltip("As long as the function is enabled, your\ncharacter will not be able to fall over\nafter hitting an object (e.g. a moving wall)");

					ImGui::Dummy(ImVec2(0.0f, 7.5f));

					ImGui::Checkbox(u8"Player to Player Collisions", &FGInternal::COLLISIONS::player_collision);
					if (ImGui::IsItemHovered())
						ImGui::SetTooltip("When the feature remains on, each time you collide\nwith another player while running or after falling,\nyour character will not fall to the ground.");

					ImGui::Dummy(ImVec2(0.0f, 7.5f));

					ImGui::Checkbox(u8"Always Fall on Legs", &FGInternal::COLLISIONS::falling_down);
					if (ImGui::IsItemHovered())
						ImGui::SetTooltip("Each time you fall from a height,\nyour character will stand on its\nfeet instead of falling to the ground.");

					ImGui::Dummy(ImVec2(0.0f, 7.5f));

					ImGui::Checkbox(u8"Unintentional Move Speed", &FGInternal::COLLISIONS::unintentional_move_speed);
					if (ImGui::IsItemHovered())
						ImGui::SetTooltip("The unintentional speed of a character movement occurs\nwhen he is grabbed by another player and moved towards him,\nsetting this function to 0.01 will keep the character always standing still.");
					ImGui::Dummy(ImVec2(0.0f, 2.5f));
					if (FGInternal::COLLISIONS::unintentional_move_speed) {
						draw_slider("Normal Threshold", &FGInternal::COLLISIONS::unintentional_move_speed_value, VALUES::SAFE_VALUES::COLLISIONS::unintentional_move_speed_min, VALUES::SAFE_VALUES::COLLISIONS::unintentional_move_speed_max);
						draw_slider("During Emote Threshold", &FGInternal::COLLISIONS::unintentional_move_speed_emote_value, VALUES::SAFE_VALUES::COLLISIONS::unintentional_move_speed_emote_min, VALUES::SAFE_VALUES::COLLISIONS::unintentional_move_speed_emote_max);
					} else {
						draw_sliderDisabled(u8"DISABLED » Normal Threshold", &FGInternal::COLLISIONS::unintentional_move_speed_value, VALUES::SAFE_VALUES::COLLISIONS::unintentional_move_speed_min, VALUES::SAFE_VALUES::COLLISIONS::unintentional_move_speed_max);
						draw_sliderDisabled(u8"DISABLED » During Emote Threshold", &FGInternal::COLLISIONS::unintentional_move_speed_emote_value, VALUES::SAFE_VALUES::COLLISIONS::unintentional_move_speed_emote_min, VALUES::SAFE_VALUES::COLLISIONS::unintentional_move_speed_emote_max);
					}
				}

				ImGui::Dummy(ImVec2(0.0f, 92.f));
				ImGui::EndChild();
			}
			
			// grab
			if (FGInternal::TAB::tab == 2) {
				ImGui::BeginChild("3_Tab", ImVec2(752, 402), true);
				ImGui::Columns(2);

				if (!FGInternal::GENERAL::disable_safe_values) {
					ImGui::Dummy(ImVec2(0.0f, 10.0f));

					ImGui::Checkbox(u8"F11 » Super Grab Feature", &FGInternal::GRAB::supergrabfeature_enabled);
					if (ImGui::IsItemHovered())
						ImGui::SetTooltip("Super grab uses all further functions, so it is\nrecommended to disable everything except Grabber Velocity.\n\nThe function itself increases the power, range and way\nof catching and holding players/items to the limit.");

					ImGui::Dummy(ImVec2(0.0f, 15.f));
					//
					if (FGInternal::GRAB::supergrabfeature_enabled) {
						ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
					}
					//
					ImGui::Checkbox(u8"Override Grab Detect Radius", &FGInternal::GRAB::grabDetectRadius);
					if (ImGui::IsItemHovered())
						ImGui::SetTooltip("Increase or decrease the detection\nrange of the player/item grab.");
					ImGui::Dummy(ImVec2(0.0f, 2.5f));
					if (FGInternal::GRAB::grabDetectRadius) {
						draw_slider("Detect Radius", &FGInternal::GRAB::grabDetectRadiusBoost, VALUES::SAFE_VALUES::GRAB::grabDetectRadius_min, VALUES::SAFE_VALUES::GRAB::grabDetectRadius_max);
					} else {
						draw_sliderDisabled(u8"DISABLED » Detect Radius", &FGInternal::GRAB::grabDetectRadiusBoost, VALUES::SAFE_VALUES::GRAB::grabDetectRadius_min, VALUES::SAFE_VALUES::GRAB::grabDetectRadius_max);
					}

					ImGui::Dummy(ImVec2(0.0f, 7.5f));

					ImGui::Checkbox(u8"Override Grab Check Distance", &FGInternal::GRAB::grabCheckDistance);
					if (ImGui::IsItemHovered())
						ImGui::SetTooltip("Increase or decrease the distance\ncheck between players, useful for\npushing people off the map.");
					ImGui::Dummy(ImVec2(0.0f, 2.5f));
					if (FGInternal::GRAB::grabCheckDistance) {
						draw_slider("Check Distance", &FGInternal::GRAB::grabCheckDistanceBoost, VALUES::SAFE_VALUES::GRAB::grabCheckDistance_min, VALUES::SAFE_VALUES::GRAB::grabCheckDistance_max);
					} else {
						draw_sliderDisabled(u8"DISABLED » Check Distance", &FGInternal::GRAB::grabCheckDistanceBoost, VALUES::SAFE_VALUES::GRAB::grabCheckDistance_min, VALUES::SAFE_VALUES::GRAB::grabCheckDistance_max);
					}

					ImGui::Dummy(ImVec2(0.0f, 7.5f));

					ImGui::Checkbox(u8"Override Grab Max Force", &FGInternal::GRAB::grabMaxForce);
					if (ImGui::IsItemHovered())
						ImGui::SetTooltip("Increase or decrease the grabbing force.");
					ImGui::Dummy(ImVec2(0.0f, 2.5f));
					if (FGInternal::GRAB::grabMaxForce) {
						draw_slider("Grab Force", &FGInternal::GRAB::grabMaxForceBoost, VALUES::SAFE_VALUES::GRAB::grabMaxForce_min, VALUES::SAFE_VALUES::GRAB::grabMaxForce_max);
					} else {
						draw_sliderDisabled(u8"DISABLED » Grab Force", &FGInternal::GRAB::grabMaxForceBoost, VALUES::SAFE_VALUES::GRAB::grabMaxForce_min, VALUES::SAFE_VALUES::GRAB::grabMaxForce_max);
					}

					ImGui::Dummy(ImVec2(0.0f, 7.5f));

					ImGui::Checkbox(u8"Override Grab Break Time", &FGInternal::GRAB::grabBreakTime);
					if (ImGui::IsItemHovered())
						ImGui::SetTooltip("Increase or decrease the time between\nbreaking the grab after catching\nanother player (server limitation).");
					ImGui::Dummy(ImVec2(0.0f, 2.5f));
					if (FGInternal::GRAB::grabBreakTime) {
						draw_slider("Normal Break", &FGInternal::GRAB::grabBreakTimeBoost, VALUES::SAFE_VALUES::GRAB::grabBreakTime_min, VALUES::SAFE_VALUES::GRAB::grabBreakTime_max);
						draw_slider("Jump Break", &FGInternal::GRAB::grabBreakTimeJumpBoost, VALUES::SAFE_VALUES::GRAB::grabBreakTimeJump_min, VALUES::SAFE_VALUES::GRAB::grabBreakTimeJump_max);
					} else {
						draw_sliderDisabled(u8"DISABLED » Normal Break", &FGInternal::GRAB::grabBreakTimeBoost, VALUES::SAFE_VALUES::GRAB::grabBreakTime_min, VALUES::SAFE_VALUES::GRAB::grabBreakTime_max);
						draw_sliderDisabled(u8"DISABLED » Jump Break", &FGInternal::GRAB::grabBreakTimeJumpBoost, VALUES::SAFE_VALUES::GRAB::grabBreakTimeJump_min, VALUES::SAFE_VALUES::GRAB::grabBreakTimeJump_max);
					}

					ImGui::Dummy(ImVec2(0.0f, 7.5f));

					ImGui::Checkbox(u8"Override Grab Check Prediction", &FGInternal::GRAB::grabCheckPredictionBase);
					if (ImGui::IsItemHovered())
						ImGui::SetTooltip("Increase or decrease the grab prediction.");
					ImGui::Dummy(ImVec2(0.0f, 2.5f));
					if (FGInternal::GRAB::grabCheckPredictionBase) {
						draw_slider("Prediction Boost", &FGInternal::GRAB::grabCheckPredictionBaseBoost, VALUES::SAFE_VALUES::GRAB::grabCheckPredictionBase_min, VALUES::SAFE_VALUES::GRAB::grabCheckPredictionBase_max);
					} else {
						draw_sliderDisabled(u8"DISABLED » Prediction Boost", &FGInternal::GRAB::grabCheckPredictionBaseBoost, VALUES::SAFE_VALUES::GRAB::grabCheckPredictionBase_min, VALUES::SAFE_VALUES::GRAB::grabCheckPredictionBase_max);
					}

					ImGui::Dummy(ImVec2(0.0f, 7.5f));

					ImGui::Checkbox(u8"Override Grab Velocity Reduction", &FGInternal::GRAB::grabImmediateVelocityReduction);
					if (ImGui::IsItemHovered())
						ImGui::SetTooltip("Increase or decrease the movement speed\nreductions for the captured player.");
					ImGui::Dummy(ImVec2(0.0f, 2.5f));
					if (FGInternal::GRAB::grabImmediateVelocityReduction) {
						draw_slider("Velocity Reduction", &FGInternal::GRAB::grabImmediateVelocityReductionBoost, VALUES::SAFE_VALUES::GRAB::grabImmediateVelocityReduction_min, VALUES::SAFE_VALUES::GRAB::grabImmediateVelocityReduction_max);
					} else {
						draw_sliderDisabled(u8"DISABLED » Velocity Reduction", &FGInternal::GRAB::grabImmediateVelocityReductionBoost, VALUES::SAFE_VALUES::GRAB::grabImmediateVelocityReduction_min, VALUES::SAFE_VALUES::GRAB::grabImmediateVelocityReduction_max);
					}

					ImGui::NextColumn();

					ImGui::Dummy(ImVec2(0.0f, 10.0f));

					ImGui::Checkbox(u8"Override Grab Direction Contribution", &FGInternal::GRAB::grabDragDirectionContribution);
					if (ImGui::IsItemHovered())
						ImGui::SetTooltip("Increase or decrease the influence on\nthe movement direction of the captured player.");
					ImGui::Dummy(ImVec2(0.0f, 2.5f));
					if (FGInternal::GRAB::grabDragDirectionContribution) {
						draw_slider("Drag Boost", &FGInternal::GRAB::grabDragDirectionContributionBoost, VALUES::SAFE_VALUES::GRAB::grabDragDirectionContribution_min, VALUES::SAFE_VALUES::GRAB::grabDragDirectionContribution_max);
					} else {
						draw_sliderDisabled(u8"DISABLED » Drag Boost", &FGInternal::GRAB::grabDragDirectionContributionBoost, VALUES::SAFE_VALUES::GRAB::grabDragDirectionContribution_min, VALUES::SAFE_VALUES::GRAB::grabDragDirectionContribution_max);
					}

					ImGui::Dummy(ImVec2(0.0f, 7.5f));

					ImGui::Checkbox(u8"Override Grab Cooldown", &FGInternal::GRAB::grabCooldown);
					if (ImGui::IsItemHovered())
						ImGui::SetTooltip("Increase or decrease the wait for grab reuse.");
					ImGui::Dummy(ImVec2(0.0f, 2.5f));
					if (FGInternal::GRAB::grabCooldown) {
						draw_slider("Grab Cooldown", &FGInternal::GRAB::grabCooldownBoost, VALUES::SAFE_VALUES::GRAB::grabCooldown_min, VALUES::SAFE_VALUES::GRAB::grabCooldown_max);
						draw_slider("Release Grab Cooldown", &FGInternal::GRAB::grabReleaseRegrabCooldownBoost, VALUES::SAFE_VALUES::GRAB::grabReleaseRegrabCooldown_min, VALUES::SAFE_VALUES::GRAB::grabReleaseRegrabCooldown_max);
					} else {
						draw_sliderDisabled(u8"DISABLED » Grab Cooldown", &FGInternal::GRAB::grabCooldownBoost, VALUES::SAFE_VALUES::GRAB::grabCooldown_min, VALUES::SAFE_VALUES::GRAB::grabCooldown_max);
						draw_sliderDisabled("DISABLED » Release Grab Cooldown", &FGInternal::GRAB::grabReleaseRegrabCooldownBoost, VALUES::SAFE_VALUES::GRAB::grabReleaseRegrabCooldown_min, VALUES::SAFE_VALUES::GRAB::grabReleaseRegrabCooldown_max);
					}

					ImGui::Dummy(ImVec2(0.0f, 7.5f));

					ImGui::Checkbox(u8"Override Regrab Delay", &FGInternal::GRAB::grabRegrabDelay);
					if (ImGui::IsItemHovered())
						ImGui::SetTooltip("Increase or decrease the rate of\ngrab reuse on the same unit.");
					ImGui::Dummy(ImVec2(0.0f, 2.5f));
					if (FGInternal::GRAB::grabRegrabDelay) {
						draw_slider("Regrab Delay", &FGInternal::GRAB::grabRegrabDelayBoost, VALUES::SAFE_VALUES::GRAB::grabRegrabDelay_min, VALUES::SAFE_VALUES::GRAB::grabRegrabDelay_max);
					} else {
						draw_sliderDisabled(u8"DISABLED » Regrab Delay", &FGInternal::GRAB::grabRegrabDelayBoost, VALUES::SAFE_VALUES::GRAB::grabRegrabDelay_min, VALUES::SAFE_VALUES::GRAB::grabRegrabDelay_max);
					}

					ImGui::Dummy(ImVec2(0.0f, 7.5f));

					ImGui::Checkbox(u8"Override Break Grab Angle", &FGInternal::GRAB::grabBreakAngle);
					if (ImGui::IsItemHovered())
						ImGui::SetTooltip("Increase or decrease the angle at which\nthe connection between you and the\ncaptured player will be broken.");
					ImGui::Dummy(ImVec2(0.0f, 2.5f));
					if (FGInternal::GRAB::grabBreakAngle) {
						draw_slider("Break Grab Angle", &FGInternal::GRAB::grabBreakAngleBoost, VALUES::SAFE_VALUES::GRAB::grabBreakAngle_min, VALUES::SAFE_VALUES::GRAB::grabBreakAngle_max);
					} else {
						draw_sliderDisabled(u8"DISABLED » Break Grab Angle", &FGInternal::GRAB::grabBreakAngleBoost, VALUES::SAFE_VALUES::GRAB::grabBreakAngle_min, VALUES::SAFE_VALUES::GRAB::grabBreakAngle_max);
					}

					ImGui::Dummy(ImVec2(0.0f, 7.5f));

					ImGui::Checkbox(u8"Override Grab Separation Force", &FGInternal::GRAB::grabSeparationForce);
					if (ImGui::IsItemHovered())
						ImGui::SetTooltip("Increase or decrease the power of\nknock and recoil after releasing the player.");
					ImGui::Dummy(ImVec2(0.0f, 2.5f));
					if (FGInternal::GRAB::grabSeparationForce) {
						draw_slider("Separation Force", &FGInternal::GRAB::grabSeparationForceBoost, VALUES::SAFE_VALUES::GRAB::grabSeparationForce_min, VALUES::SAFE_VALUES::GRAB::grabSeparationForce_max);
					} else {
						draw_sliderDisabled(u8"DISABLED » Separation Force", &FGInternal::GRAB::grabSeparationForceBoost, VALUES::SAFE_VALUES::GRAB::grabSeparationForce_min, VALUES::SAFE_VALUES::GRAB::grabSeparationForce_max);
					}

					ImGui::Dummy(ImVec2(0.0f, 7.5f));
					//
					if (FGInternal::GRAB::supergrabfeature_enabled) {
						ImGui::PushItemFlag(ImGuiItemFlags_Disabled, false);
					}
					//
					ImGui::Checkbox(u8"Override Grabber Velocity", &FGInternal::GRAB::grabberVelocity);
					if (ImGui::IsItemHovered())
						ImGui::SetTooltip("Increase or decrease the speed of the\nrake and the effect of its speed on\nthe speed of the captured person.\n(Perfect for throwing people off the\nmap with the Super Grab feature)");
					ImGui::Dummy(ImVec2(0.0f, 2.5f));
					if (FGInternal::GRAB::grabberVelocity) {
						draw_slider("Grabber Velocity", &FGInternal::GRAB::grabberVelocityBoost, VALUES::SAFE_VALUES::GRAB::grabberVelocity_min, VALUES::SAFE_VALUES::GRAB::grabberVelocity_max);
					} else {
						draw_sliderDisabled(u8"DISABLED » Grabber Velocity", &FGInternal::GRAB::grabberVelocityBoost, VALUES::SAFE_VALUES::GRAB::grabberVelocity_min, VALUES::SAFE_VALUES::GRAB::grabberVelocity_max);
					}
				} else { //////////////////////////////////////////////////////////////////////////////
					ImGui::Dummy(ImVec2(0.0f, 10.0f));

					ImGui::Checkbox(u8"F11 » Super Grab Feature", &FGInternal::GRAB::supergrabfeature_enabled);
					if (ImGui::IsItemHovered())
						ImGui::SetTooltip("Super grab uses all further functions, so it is\nrecommended to disable everything except Grabber Velocity.\n\nThe function itself increases the power, range and way\nof catching and holding players/items to the limit.");

					ImGui::Dummy(ImVec2(0.0f, 15.f));
					//
					if (FGInternal::GRAB::supergrabfeature_enabled) {
						ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
					}
					//
					ImGui::Checkbox(u8"Override Grab Detect Radius", &FGInternal::GRAB::grabDetectRadius);
					if (ImGui::IsItemHovered())
						ImGui::SetTooltip("Increase or decrease the detection\nrange of the player/item grab.");
					ImGui::Dummy(ImVec2(0.0f, 2.5f));
					if (FGInternal::GRAB::grabDetectRadius) {
						draw_slider("Detect Radius", &FGInternal::GRAB::grabDetectRadiusBoost, FGInternal::GENERAL::min_unsafe_value, FGInternal::GENERAL::max_unsafe_value);
					} else {
						draw_sliderDisabled(u8"DISABLED » Detect Radius", &FGInternal::GRAB::grabDetectRadiusBoost, FGInternal::GENERAL::min_unsafe_value, FGInternal::GENERAL::max_unsafe_value);
					}

					ImGui::Dummy(ImVec2(0.0f, 7.5f));

					ImGui::Checkbox(u8"Override Grab Check Distance", &FGInternal::GRAB::grabCheckDistance);
					if (ImGui::IsItemHovered())
						ImGui::SetTooltip("Increase or decrease the distance\ncheck between players, useful for\npushing people off the map.");
					ImGui::Dummy(ImVec2(0.0f, 2.5f));
					if (FGInternal::GRAB::grabCheckDistance) {
						draw_slider("Check Distance", &FGInternal::GRAB::grabCheckDistanceBoost, FGInternal::GENERAL::min_unsafe_value, FGInternal::GENERAL::max_unsafe_value);
					} else {
						draw_sliderDisabled(u8"DISABLED » Check Distance", &FGInternal::GRAB::grabCheckDistanceBoost, FGInternal::GENERAL::min_unsafe_value, FGInternal::GENERAL::max_unsafe_value);
					}

					ImGui::Dummy(ImVec2(0.0f, 7.5f));

					ImGui::Checkbox(u8"Override Grab Max Force", &FGInternal::GRAB::grabMaxForce);
					if (ImGui::IsItemHovered())
						ImGui::SetTooltip("Increase or decrease the grabbing force.");
					ImGui::Dummy(ImVec2(0.0f, 2.5f));
					if (FGInternal::GRAB::grabMaxForce) {
						draw_slider("Grab Force", &FGInternal::GRAB::grabMaxForceBoost, FGInternal::GENERAL::min_unsafe_value, FGInternal::GENERAL::max_unsafe_value);
					} else {
						draw_sliderDisabled(u8"DISABLED » Grab Force", &FGInternal::GRAB::grabMaxForceBoost, FGInternal::GENERAL::min_unsafe_value, FGInternal::GENERAL::max_unsafe_value);
					}

					ImGui::Dummy(ImVec2(0.0f, 7.5f));

					ImGui::Checkbox(u8"Override Grab Break Time", &FGInternal::GRAB::grabBreakTime);
					if (ImGui::IsItemHovered())
						ImGui::SetTooltip("Increase or decrease the time between\nbreaking the grab after catching\nanother player (server limitation).");
					ImGui::Dummy(ImVec2(0.0f, 2.5f));
					if (FGInternal::GRAB::grabBreakTime) {
						draw_slider("Normal Break", &FGInternal::GRAB::grabBreakTimeBoost, FGInternal::GENERAL::min_unsafe_value, FGInternal::GENERAL::max_unsafe_value);
						draw_slider("Jump Break", &FGInternal::GRAB::grabBreakTimeJumpBoost, FGInternal::GENERAL::min_unsafe_value, FGInternal::GENERAL::max_unsafe_value);
					} else {
						draw_sliderDisabled(u8"DISABLED » Normal Break", &FGInternal::GRAB::grabBreakTimeBoost, FGInternal::GENERAL::min_unsafe_value, FGInternal::GENERAL::max_unsafe_value);
						draw_sliderDisabled(u8"DISABLED » Jump Break", &FGInternal::GRAB::grabBreakTimeJumpBoost, FGInternal::GENERAL::min_unsafe_value, FGInternal::GENERAL::max_unsafe_value);
					}

					ImGui::Dummy(ImVec2(0.0f, 7.5f));

					ImGui::Checkbox(u8"Override Grab Check Prediction", &FGInternal::GRAB::grabCheckPredictionBase);
					if (ImGui::IsItemHovered())
						ImGui::SetTooltip("Increase or decrease the grab prediction.");
					ImGui::Dummy(ImVec2(0.0f, 2.5f));
					if (FGInternal::GRAB::grabCheckPredictionBase) {
						draw_slider("Prediction Boost", &FGInternal::GRAB::grabCheckPredictionBaseBoost, FGInternal::GENERAL::min_unsafe_value, FGInternal::GENERAL::max_unsafe_value);
					} else {
						draw_sliderDisabled(u8"DISABLED » Prediction Boost", &FGInternal::GRAB::grabCheckPredictionBaseBoost, FGInternal::GENERAL::min_unsafe_value, FGInternal::GENERAL::max_unsafe_value);
					}

					ImGui::Dummy(ImVec2(0.0f, 7.5f));

					ImGui::Checkbox(u8"Override Grab Velocity Reduction", &FGInternal::GRAB::grabImmediateVelocityReduction);
					if (ImGui::IsItemHovered())
						ImGui::SetTooltip("Increase or decrease the movement speed\nreductions for the captured player.");
					ImGui::Dummy(ImVec2(0.0f, 2.5f));
					if (FGInternal::GRAB::grabImmediateVelocityReduction) {
						draw_slider("Velocity Reduction", &FGInternal::GRAB::grabImmediateVelocityReductionBoost, FGInternal::GENERAL::min_unsafe_value, FGInternal::GENERAL::max_unsafe_value);
					} else {
						draw_sliderDisabled(u8"DISABLED » Velocity Reduction", &FGInternal::GRAB::grabImmediateVelocityReductionBoost, FGInternal::GENERAL::min_unsafe_value, FGInternal::GENERAL::max_unsafe_value);
					}

					ImGui::NextColumn();

					ImGui::Dummy(ImVec2(0.0f, 10.0f));

					ImGui::Checkbox(u8"Override Grab Direction Contribution", &FGInternal::GRAB::grabDragDirectionContribution);
					if (ImGui::IsItemHovered())
						ImGui::SetTooltip("Increase or decrease the influence on\nthe movement direction of the captured player.");
					ImGui::Dummy(ImVec2(0.0f, 2.5f));
					if (FGInternal::GRAB::grabDragDirectionContribution) {
						draw_slider("Drag Boost", &FGInternal::GRAB::grabDragDirectionContributionBoost, FGInternal::GENERAL::min_unsafe_value, FGInternal::GENERAL::max_unsafe_value);
					} else {
						draw_sliderDisabled(u8"DISABLED » Drag Boost", &FGInternal::GRAB::grabDragDirectionContributionBoost, FGInternal::GENERAL::min_unsafe_value, FGInternal::GENERAL::max_unsafe_value);
					}

					ImGui::Dummy(ImVec2(0.0f, 7.5f));

					ImGui::Checkbox(u8"Override Grab Cooldown", &FGInternal::GRAB::grabCooldown);
					if (ImGui::IsItemHovered())
						ImGui::SetTooltip("Increase or decrease the wait for grab reuse.");
					ImGui::Dummy(ImVec2(0.0f, 2.5f));
					if (FGInternal::GRAB::grabCooldown) {
						draw_slider("Grab Cooldown", &FGInternal::GRAB::grabCooldownBoost, FGInternal::GENERAL::min_unsafe_value, FGInternal::GENERAL::max_unsafe_value);
						draw_slider("Release Grab Cooldown", &FGInternal::GRAB::grabReleaseRegrabCooldownBoost, FGInternal::GENERAL::min_unsafe_value, FGInternal::GENERAL::max_unsafe_value);
					} else {
						draw_sliderDisabled(u8"DISABLED » Grab Cooldown", &FGInternal::GRAB::grabCooldownBoost, FGInternal::GENERAL::min_unsafe_value, FGInternal::GENERAL::max_unsafe_value);
						draw_sliderDisabled("DISABLED » Release Grab Cooldown", &FGInternal::GRAB::grabReleaseRegrabCooldownBoost, FGInternal::GENERAL::min_unsafe_value, FGInternal::GENERAL::max_unsafe_value);
					}

					ImGui::Dummy(ImVec2(0.0f, 7.5f));

					ImGui::Checkbox(u8"Override Regrab Delay", &FGInternal::GRAB::grabRegrabDelay);
					if (ImGui::IsItemHovered())
						ImGui::SetTooltip("Increase or decrease the rate of\ngrab reuse on the same unit.");
					ImGui::Dummy(ImVec2(0.0f, 2.5f));
					if (FGInternal::GRAB::grabRegrabDelay) {
						draw_slider("Regrab Delay", &FGInternal::GRAB::grabRegrabDelayBoost, FGInternal::GENERAL::min_unsafe_value, FGInternal::GENERAL::max_unsafe_value);
					} else {
						draw_sliderDisabled(u8"DISABLED » Regrab Delay", &FGInternal::GRAB::grabRegrabDelayBoost, FGInternal::GENERAL::min_unsafe_value, FGInternal::GENERAL::max_unsafe_value);
					}

					ImGui::Dummy(ImVec2(0.0f, 7.5f));

					ImGui::Checkbox(u8"Override Break Grab Angle", &FGInternal::GRAB::grabBreakAngle);
					if (ImGui::IsItemHovered())
						ImGui::SetTooltip("Increase or decrease the angle at which\nthe connection between you and the\ncaptured player will be broken.");
					ImGui::Dummy(ImVec2(0.0f, 2.5f));
					if (FGInternal::GRAB::grabBreakAngle) {
						draw_slider("Break Grab Angle", &FGInternal::GRAB::grabBreakAngleBoost, 0, 360);
					} else {
						draw_sliderDisabled(u8"DISABLED » Break Grab Angle", &FGInternal::GRAB::grabBreakAngleBoost, FGInternal::GENERAL::min_unsafe_value, FGInternal::GENERAL::max_unsafe_value);
					}

					ImGui::Dummy(ImVec2(0.0f, 7.5f));

					ImGui::Checkbox(u8"Override Grab Separation Force", &FGInternal::GRAB::grabSeparationForce);
					if (ImGui::IsItemHovered())
						ImGui::SetTooltip("Increase or decrease the power of\nknock and recoil after releasing the player.");
					ImGui::Dummy(ImVec2(0.0f, 2.5f));
					if (FGInternal::GRAB::grabSeparationForce) {
						draw_slider("Separation Force", &FGInternal::GRAB::grabSeparationForceBoost, FGInternal::GENERAL::min_unsafe_value, FGInternal::GENERAL::max_unsafe_value);
					} else {
						draw_sliderDisabled(u8"DISABLED » Separation Force", &FGInternal::GRAB::grabSeparationForceBoost, FGInternal::GENERAL::min_unsafe_value, FGInternal::GENERAL::max_unsafe_value);
					}

					ImGui::Dummy(ImVec2(0.0f, 7.5f));
					//
					if (FGInternal::GRAB::supergrabfeature_enabled) {
						ImGui::PushItemFlag(ImGuiItemFlags_Disabled, false);
					}
					//
					ImGui::Checkbox(u8"Override Grabber Velocity", &FGInternal::GRAB::grabberVelocity);
					if (ImGui::IsItemHovered())
						ImGui::SetTooltip("Increase or decrease the speed of the\nrake and the effect of its speed on\nthe speed of the captured person.\n(Perfect for throwing people off the\nmap with the Super Grab feature)");
					ImGui::Dummy(ImVec2(0.0f, 2.5f));
					if (FGInternal::GRAB::grabberVelocity) {
						draw_slider("Grabber Velocity", &FGInternal::GRAB::grabberVelocityBoost, FGInternal::GENERAL::min_unsafe_value, FGInternal::GENERAL::max_unsafe_value);
					} else {
						draw_sliderDisabled(u8"DISABLED » Grabber Velocity", &FGInternal::GRAB::grabberVelocityBoost, FGInternal::GENERAL::min_unsafe_value, FGInternal::GENERAL::max_unsafe_value);
					}
				}

				ImGui::Dummy(ImVec2(0.0f, 42.f));

				ImGui::EndChild();
			}

			// misc
			if (FGInternal::TAB::tab == 3) {
				ImGui::BeginChild("4_Tab", ImVec2(752, 402), true);
				ImGui::Columns(2);
				//-->
				std::string MISCELLANEOUS = "MISCELLANEOUS";
				ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (ImGui::GetColumnWidth() / 2) - (ImGui::CalcTextSize(MISCELLANEOUS.c_str()).x / 2)
					- ImGui::GetScrollX() - 2 * (ImGui::GetStyle().ItemSpacing.x / 2));
				ImGui::Text("%s", MISCELLANEOUS);
				ImGui::NextColumn();
				std::string OTHERS = "OTHERS";
				ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (ImGui::GetColumnWidth() / 2) - (ImGui::CalcTextSize(OTHERS.c_str()).x / 2)
					- ImGui::GetScrollX() - 2 * (ImGui::GetStyle().ItemSpacing.x / 2));
				ImGui::Text("%s", OTHERS);
				ImGui::NextColumn();
				ImGui::Separator();
				//-->


				ImGui::Dummy(ImVec2(0.0f, 10.0f));

				ImGui::Checkbox(u8"DEL » Enable Unhook Key", &FGInternal::GENERAL::enable_unhook_key);
				if (ImGui::IsItemHovered())
					ImGui::SetTooltip("Checking this option enables a button that\nturns off the cheat, leaving the game on.");

				ImGui::Dummy(ImVec2(0.0f, 7.5f));

				ImGui::Checkbox(u8"END » Enable Panic Key", &FGInternal::GENERAL::enable_panic_key);
				if (ImGui::IsItemHovered())
					ImGui::SetTooltip("Selecting this option enables a button that\nimmediately exits the game along with the cheat.");

				ImGui::NextColumn();
				//-->

				ImGui::Dummy(ImVec2(0.0f, 10.0f));

				ImGui::Checkbox(u8"G » Instant Qualify", &FGInternal::GENERAL::instant_win);
				if (ImGui::IsItemHovered())
					ImGui::SetTooltip("After pressing the assigned button, the character will\nautomatically go to the finish line on maps that:\n- are not team games,\n- are not elimination maps,\n- are not hunt maps.");

				ImGui::Dummy(ImVec2(0.0f, 7.5f));

				ImGui::Checkbox(u8"Equal Tussle Chances", &FGInternal::CARRY::carryTussleChances);
				if (ImGui::IsItemHovered())
					ImGui::SetTooltip("When the feature is on and another player catches you,\nyour chance of winning is equal to the chance of losing a scuffle.\nWithout this feature, you will always lose when caught.");

				ImGui::Dummy(ImVec2(0.0f, 316.f));

				ImGui::EndChild();
			}

			// config
			if (FGInternal::TAB::tab == 4) {
				ImGui::BeginChild("5_Tab", ImVec2(752, 402), true);
				ImGui::Columns(2);
				ImGui::Text("Next update, I swear");

				ImGui::EndChild();
			}
			
			// changelog
			if (FGInternal::TAB::tab == 5) {
				ImGui::BeginChild("5_Tab", ImVec2(752, 402), true);
				ImGui::Columns(1);
				//-->
				std::string CHANGELOG = "CHANGELOG";
				ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (ImGui::GetColumnWidth() / 2) - (ImGui::CalcTextSize(CHANGELOG.c_str()).x / 2)
					- ImGui::GetScrollX() - 2 * (ImGui::GetStyle().ItemSpacing.x / 2));
				ImGui::Text("%s", CHANGELOG);
				ImGui::Separator();
				//-->


				ImGui::Dummy(ImVec2(0.0f, 10.0f));

				ImGui::Text("07 SEPT 2020");
				ImGui::Separator();
				ImGui::Text(u8"» Improved menu display after the first launch;");
				ImGui::Text(u8"» Added PAGE UP button resetting menu item to 0x 0y;");
				ImGui::Text(u8"» Auto Bunny Hop performance has been improved;");

				ImGui::Dummy(ImVec2(0.0f, 10.0f));

				ImGui::Text("06 SEPT 2020");
				ImGui::Separator();
				ImGui::Text(u8"» A reset value button has been added;");
				ImGui::Text(u8"» A function has been added to display activated features;");
				ImGui::Text(u8"» Added automatic grab functions disabling when Super Grab is enabled to avoid bugs;");
				ImGui::Text(u8"» Fixed a bug with disabling and enabling the function under the END and DEL buttons;");
				ImGui::Text(u8"» The SHIFT + C keys combo while flying has been added to make it easier\n  to catch a target without changing the character's position;");
				ImGui::Text(u8"» The Auto Bunny Hop function has been added and can be used with the M3 (scroll) button (hold);");
				//ImGui::Text(u8"» CONFIG!!!;");

				ImGui::EndChild();
			}

		}
		ImGui::End();
	}

	bool OldKeysDown[512];
	float OldNavInputs[ImGuiNavInput_COUNT];
	void update_keys() {

		auto io = ImGui::GetIO();

		if ((io.KeysDown[VK_INSERT] && !OldKeysDown[VK_INSERT]) ||
			(io.NavInputs[ImGuiNavInput_RightThumb] > 0.f && OldNavInputs[ImGuiNavInput_RightThumb] == 0.f)) {
			render::menu_is_open = !render::menu_is_open;
		}

		if (render::menu_is_open) {
			auto context = ImGui::GetCurrentContext();
			if (context->NavWindow) {
				if (context->Windows.Size > 1) {
					int32_t move_window_direction = -1;
					if (io.NavInputs[ImGuiNavInput_FocusPrev] > 0.f && OldNavInputs[ImGuiNavInput_FocusPrev] == 0.f)
						move_window_direction = 0;
					else if (io.NavInputs[ImGuiNavInput_FocusNext] > 0.f && OldNavInputs[ImGuiNavInput_FocusNext] == 0.f)
						move_window_direction = 1;

					if (move_window_direction != -1) {
						ImGuiWindow* new_window = nullptr;
						for (auto window : context->Windows) {
							if (window == context->NavWindow || window->Hidden || window->IsFallbackWindow)
								continue;

							if (!strcmp(window->Name, "BackBuffer"))
								continue;

							if (move_window_direction == 0) {
								if (window->Pos.x <= context->NavWindow->Pos.x)
									if (!new_window || window->Pos.x > new_window->Pos.x)
										new_window = window;
							}
							else {
								if (window->Pos.x >= context->NavWindow->Pos.x)
									if (!new_window || window->Pos.x < new_window->Pos.x)
										new_window = window;
							}
						}
						if (new_window) {
							ImGui::FocusWindow(new_window);
						}
					}
				}
			}
		}

		if ((io.KeysDown[VK_F1] && !OldKeysDown[VK_F1]) || (io.NavInputs[ImGuiNavInput_Input] && !OldNavInputs[ImGuiNavInput_Input])) {
			FGInternal::MOVEMENT::fly_enabled = !FGInternal::MOVEMENT::fly_enabled;
		}

		if (io.KeysDown[VK_F2] && !OldKeysDown[VK_F2]) {
			FGInternal::MOVEMENT::speed_enabled = !FGInternal::MOVEMENT::speed_enabled;
		}

		if (io.KeysDown[VK_F3] && !OldKeysDown[VK_F3]) {
			FGInternal::MOVEMENT::dive_enabled = !FGInternal::MOVEMENT::dive_enabled;
		}

		if (io.KeysDown[VK_F4] && !OldKeysDown[VK_F4]) {
			FGInternal::MOVEMENT::gravity_enabled = !FGInternal::MOVEMENT::gravity_enabled;
		}
		if (io.KeysDown[VK_F5] && !OldKeysDown[VK_F5]) {
			FGInternal::ESP::enable_players_esp = !FGInternal::ESP::enable_players_esp;
		}

		if (io.KeysDown[VK_F6] && !OldKeysDown[VK_F6]) {
			FGInternal::ESP::correct_doors_enabled = !FGInternal::ESP::correct_doors_enabled;
		}

		if (io.KeysDown[VK_F7] && !OldKeysDown[VK_F7]) {
			FGInternal::ESP::correct_path_enabled = !FGInternal::ESP::correct_path_enabled;
		}

		if (io.KeysDown[VK_F8] && !OldKeysDown[VK_F8]) {
			FGInternal::ESP::non_jinxed_players_enabled = !FGInternal::ESP::non_jinxed_players_enabled;
		}

		if (io.KeysDown[VK_F9] && !OldKeysDown[VK_F9]) {
			FGInternal::ESP::show_all_platforms_enabled = !FGInternal::ESP::show_all_platforms_enabled;
		}

		if (io.KeysDown[VK_F10] && !OldKeysDown[VK_F10]) {
			FGInternal::ESP::show_player_with_tail = !FGInternal::ESP::show_player_with_tail;
		}

		if (io.KeysDown[VK_F11] && !OldKeysDown[VK_F11])
			FGInternal::GRAB::supergrabfeature_enabled = !FGInternal::GRAB::supergrabfeature_enabled;

		if (io.KeysDown[VK_HOME] && !OldKeysDown[VK_HOME]) {
			FGInternal::GENERAL::hide_corner_text = !FGInternal::GENERAL::hide_corner_text;
		}

		if (io.NavInputs[ImGuiNavInput_FocusPrev] > 0.f) {
			FGInternal::MOVEMENT::speed_enabled = true;
		} else if (io.NavInputs[ImGuiNavInput_FocusPrev] == 0.f && OldNavInputs[ImGuiNavInput_FocusPrev] > 0.f) {
			FGInternal::MOVEMENT::speed_enabled = false;
		}

		memcpy(OldKeysDown, io.KeysDown, 512 * sizeof(bool));
		memcpy(OldNavInputs, io.NavInputs, ImGuiNavInput_COUNT * sizeof(float));
	}

	void update_indicators() {
		auto text_size = draw_manager::calc_text_size(12, "TAKE");
		auto y = float(15);

		if (!FGInternal::GENERAL::hide_corner_text) {
			draw_manager::add_text_on_screen({ 5, y }, 0xFF232323, 12, "FG PREMIUM");
			y += text_size.y + 3.f;
			draw_manager::add_text_on_screen({ 5, y }, 0xFF363636, 12, u8"» INSERT » Open Cheat Menu");
			y += text_size.y + 3.f;
			draw_manager::add_text_on_screen({ 5, y }, 0xFF363636, 12, u8"» HOME » Hide Corner Text");
			y += text_size.y + 3.f;
			draw_manager::add_text_on_screen({ 5, y }, 0xFF363636, 12, u8"» PAGE UP » Reset Menu Possition");
		}

		if (FGInternal::GENERAL::show_enabled_features) {
			if (FGInternal::MOVEMENT::fly_enabled || FGInternal::MOVEMENT::speed_enabled
				|| FGInternal::MOVEMENT::dive_enabled || FGInternal::MOVEMENT::gravity_enabled) {
				y += text_size.y + 70.f;
				draw_manager::add_text_on_screen({ 5, y }, 0xFF232323, 12, u8"MOVEMENT MODS");
				if (FGInternal::MOVEMENT::fly_enabled) {
					y += text_size.y + 3.f;
					draw_manager::add_text_on_screen({ 5, y }, 0xFF232323, 12, u8"» Flying Mode » Enabled");
				}
				if (FGInternal::MOVEMENT::speed_enabled) {
					y += text_size.y + 3.f;
					draw_manager::add_text_on_screen({ 5, y }, 0xFF232323, 12, u8"» Speed Hack » Enabled");
				}
				if (FGInternal::MOVEMENT::dive_enabled) {
					y += text_size.y + 3.f;
					draw_manager::add_text_on_screen({ 5, y }, 0xFF232323, 12, u8"» Dive Boost » Enabled");
				}
				if (FGInternal::MOVEMENT::gravity_enabled) {
					y += text_size.y + 3.f;
					draw_manager::add_text_on_screen({ 5, y }, 0xFF232323, 12, u8"» Gravity Boost » Enabled");
				}
			}
			if (FGInternal::JUMP::no_jump_delay) {
				y += text_size.y + 10.f;
				draw_manager::add_text_on_screen({ 5, y }, 0xFF232323, 12, u8"JUMP MODS");
				y += text_size.y + 3.f;
				draw_manager::add_text_on_screen({ 5, y }, 0xFF232323, 12, u8"» No Jump Delay");
			}
			if (FGInternal::GRAB::supergrabfeature_enabled) {
				y += text_size.y + 10.f;
				draw_manager::add_text_on_screen({ 5, y }, 0xFF232323, 12, u8"GRAB MODS");
				y += text_size.y + 3.f;
				draw_manager::add_text_on_screen({ 5, y }, 0xFF232323, 12, u8"» Super Grab » Enabled");
			}
			if (FGInternal::COLLISIONS::object_collision || FGInternal::COLLISIONS::stun_collision
				|| FGInternal::COLLISIONS::player_collision || FGInternal::COLLISIONS::falling_down) {
				y += text_size.y + 10.f;
				draw_manager::add_text_on_screen({ 5, y }, 0xFF232323, 12, u8"COLLISION MODS");
				if (FGInternal::COLLISIONS::object_collision) {
					y += text_size.y + 3.f;
					draw_manager::add_text_on_screen({ 5, y }, 0xFF232323, 12, u8"» Object Collision » Disabled");
				}
				if (FGInternal::COLLISIONS::stun_collision) {
					y += text_size.y + 3.f;
					draw_manager::add_text_on_screen({ 5, y }, 0xFF232323, 12, u8"» Stuns/Knockbacks » Disabled");
				}
				if (FGInternal::COLLISIONS::player_collision) {
					y += text_size.y + 3.f;
					draw_manager::add_text_on_screen({ 5, y }, 0xFF232323, 12, u8"» Player Collision » Disabled");
				}
				if (FGInternal::COLLISIONS::falling_down) {
					y += text_size.y + 3.f;
					draw_manager::add_text_on_screen({ 5, y }, 0xFF232323, 12, u8"» Falling Down » Disabled");
				}
				if (FGInternal::COLLISIONS::unintentional_move_speed) {
					y += text_size.y + 3.f;
					draw_manager::add_text_on_screen({ 5, y }, 0xFF232323, 12, u8"» Unintentional Moves » Disabled");
				}
			}
			if (FGInternal::GENERAL::instant_win || FGInternal::CARRY::carryTussleChances) {
				y += text_size.y + 10.f;
				draw_manager::add_text_on_screen({ 5, y }, 0xFF232323, 12, u8"OTHER MODS");
				if (FGInternal::GENERAL::instant_win) {
					y += text_size.y + 3.f;
					draw_manager::add_text_on_screen({ 5, y }, 0xFF232323, 12, u8"» Instant Qualify » Enabled");
				}
				if (FGInternal::CARRY::carryTussleChances) {
					y += text_size.y + 3.f;
					draw_manager::add_text_on_screen({ 5, y }, 0xFF232323, 12, u8"» Equal Tussle Chances » Enabled");
				}
			}
		}
	}
};
