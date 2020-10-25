#pragma once
#include <Windows.h>

namespace FGInternal {
	namespace TAB {
		extern int tab;
		extern bool main_tab_0;
		extern bool main_tab_1;
		extern bool main_tab_2;
		extern bool main_tab_3;
		extern bool main_tab_4;
		extern bool main_tab_5;
	};
	//
	namespace GENERAL {
		extern bool hide_corner_text;
		extern bool show_enabled_features;
		extern bool disable_safe_values;
		extern bool instant_win;
		extern bool enable_unhook_key;
		extern bool enable_panic_key;
		extern bool reset_button;
	};

	namespace ESP {
		extern bool enable_players_esp;
		extern bool correct_doors_enabled;
		extern bool correct_path_enabled;
		extern bool non_jinxed_players_enabled;
		extern bool non_jinxed_enemies_enabled;
		extern bool non_jinxed_allies_enabled;
		extern bool show_all_platforms_enabled;
		extern bool show_player_with_tail;

		// COLORS

		extern unsigned int players_esp_color;
		extern unsigned int correct_doors_color;
		extern unsigned int correct_path_color;
		extern unsigned int jinxed_enemies_color;
		extern unsigned int jinxed_allies_color;
		extern unsigned int player_with_tail_color;
	};

	namespace MOVEMENT {
		extern bool fly_enabled;
		extern float fly_speed;

		extern bool speed_enabled;
		extern float ms_normal_boost;
		extern float ms_carry_boost;
		extern float ms_grabbing_boost;
		extern float ms_rolling_boost;

		extern bool dive_enabled;
		extern float normalDive_speed;
		extern float airDive_speed;

		extern bool gravity_enabled;
		extern float gravity_scale;

	};

	namespace JUMP {
		extern bool no_jump_delay;
		extern bool auto_bunny_hop;
	}

	namespace COLLISIONS {
		extern bool stun_collision;
		extern bool object_collision;
		extern bool player_collision;
		extern bool falling_down;

		extern bool unintentional_move_speed;
		extern float unintentional_move_speed_value;
		extern float unintentional_move_speed_emote_value;
	};

	namespace CARRY {
		extern bool carryDropForce;
		extern float carryNormalDropBoost;
		extern float carryDiveDropBoost;
		extern bool carryTussleChances;
	};

	namespace GRAB {
		extern bool supergrabfeature_enabled;

		extern bool grabDetectRadius;
		extern float grabDetectRadiusBoost;

		extern bool grabCheckDistance;
		extern float grabCheckDistanceBoost;

		extern bool grabMaxForce;
		extern float grabMaxForceBoost;

		extern bool grabBreakTime;
		extern float grabBreakTimeBoost;
		extern float grabBreakTimeJumpBoost;

		extern bool armLength;
		extern float armLengthBoost;

		extern bool grabCheckPredictionBase;
		extern float grabCheckPredictionBaseBoost;

		extern bool grabImmediateVelocityReduction;
		extern float grabImmediateVelocityReductionBoost;

		extern bool grabDragDirectionContribution;
		extern float grabDragDirectionContributionBoost;

		extern bool grabCooldown;
		extern float grabCooldownBoost;

		extern bool grabRegrabDelay;
		extern float grabRegrabDelayBoost;

		extern bool grabReleaseRegrabCooldown;
		extern float grabReleaseRegrabCooldownBoost;

		extern bool grabBreakAngle;
		extern float grabBreakAngleBoost;

		extern bool grabberVelocity;
		extern float grabberVelocityBoost;

		extern bool grabSeparationForce;
		extern float grabSeparationForceBoost;
	};
};

namespace menu { 
	void draw();
	void update_keys();
	void update_indicators();
};