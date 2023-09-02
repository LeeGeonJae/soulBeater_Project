#pragma once

#include "Vector2.h"

#include <string>
#include <Windows.h>

namespace soulBeater
{
	const extern unsigned int BOSS_STAGE_TIME;
	const extern unsigned int STAGE_TIME;
	const extern unsigned int BOSS_TIME;
	const extern unsigned int GRID_DISTANCE;
	const extern unsigned int MAX_X;
	const extern unsigned int MAX_Y;

	const extern char* KNIGHT_EFFECT_EVENT;

	const extern WCHAR* STAGE_SPRITE_KEY;
	const extern WCHAR* PLAYER_NODE_SPRITE_KEY;
	const extern WCHAR* CUTSCENE_BEALZEBUB_SPRITE_KEY;
	const extern WCHAR* CUTSCENE_EVE_ESS_SPRITE_KEY;
	const extern WCHAR* CUTSCENE_LILITH_SPRITE_KEY;
	const extern WCHAR* GAMEOVER_SPRITE_KEY;
	const extern WCHAR* ENDING_SPRITE_KEY;
	const extern WCHAR* HIT_EFFECT_SPRITE_KEY;

	const extern WCHAR* STAGE_SPRITE_PATH;
	const extern WCHAR* PLAYER_NODE_SPRITE_PATH;
	const extern WCHAR* CUTSCENE_BEALZEBUB_PATH;
	const extern WCHAR* CUTSCENE_EVE_ESS_PATH;
	const extern WCHAR* CUTSCENE_LILITH_PATH;
	const extern WCHAR* GAMEOVER_SPRITE_PATH;
	const extern WCHAR* ENDING_SPRITE_PATH;
	const extern WCHAR* HIT_EFFECT_SPRITE_PATH;

	const extern WCHAR* PLAYER_ANI_KEY;
	const extern WCHAR* BEALZEBUB_ANI_KEY;
	const extern WCHAR* EVE_ANI_KEY;
	const extern WCHAR* YS_ANI_KEY;
	const extern WCHAR* LILITH_ANI_KEY;
	const extern WCHAR* FLY_ANI_KEY;
	const extern WCHAR* GHOST_ANI_KEY;
	const extern WCHAR* SUCCUBUS_ANI_KEY;
	const extern WCHAR* KNIGHT_ANI_KEY;
	const extern WCHAR* WARNING_ANI_KEY;

	const extern WCHAR* PLAYER_ANI_PATH;
	const extern WCHAR* BEALZEBUB_ANI_PATH;
	const extern WCHAR* EVE_ANI_PATH;
	const extern WCHAR* YS_ANI_PATH;
	const extern WCHAR* LILITH_ANI_PATH;
	const extern WCHAR* FLY_ANI_PATH;
	const extern WCHAR* GHOST_ANI_PATH;
	const extern WCHAR* SUCCUBUS_ANI_PATH;
	const extern WCHAR* KNIGHT_ANI_PATH;

	const extern WCHAR* WARNING_ANI_KEY;
	const extern WCHAR* BOOM_ANI_KEY;

	// const extern WCHAR* WARNING_ANI_PATH (추가 : 이건재)
	const extern WCHAR* WARNING_ANI_PATH;
	const extern WCHAR* BOOM_ANI_PATH;

	const extern d2dFramework::Vector2 STAGE_NAME_OBJECT_TRANSLATE;
	const extern D2D1_RECT_F STAGE_NAME[9];

	const extern int BGM_TITLE_KEY;
	const extern int BGM_STAGE1_1_KEY;
	const extern int BGM_STAGE1_2_KEY;
	const extern int BGM_STAGE1_3_KEY;
	const extern int BGM_GAMEOVER_KEY;

	const extern int BGM_STAGE2_1_KEY;
	const extern int BGM_STAGE2_2_KEY;
	const extern int BGM_STAGE2_3_KEY;
	const extern int BGM_STAGE3_1_KEY;
	const extern int BGM_STAGE3_2_KEY;
	const extern int BGM_STAGE3_3_KEY;
	const extern int BGM_STAGE1_BOSS_KEY;
	const extern int BGM_STAGE2_BOSS_KEY;
	const extern int BGM_STAGE3_BOSS_KEY;

	const extern int EFFECT_PLAYERHIT_KEY;
	const extern int EFFECT_PLAYERMOVE_KEY;

	const extern int EFFECT_BEELZEBUBHIT_KEY;
	const extern int EFFECT_PLAYERATTACK_KEY;
	const extern int EFFECT_EVEHIT_KEY;
	const extern int EFFECT_YSHIT_KEY;
	const extern int EFFECT_KNIGHTHIT_KEY;
	const extern int EFFECT_GHOSTHIT_KEY;
	const extern int EFFECT_SUCCUBUSHIT_KEY;
	const extern int EFFECT_LILITHHIT_KEY;
	const extern int EFFECT_SOUND11_KEY;
	const extern int EFFECT_SOUND12_KEY;
	const extern int EFFECT_SOUND13_KEY;

	const extern char* BGM_TITLE_PATH;
	const extern char* BGM_STAGE1_1_PATH;
	const extern char* BGM_STAGE1_2_PATH;
	const extern char* BGM_STAGE1_3_PATH;
	const extern char* BGM_GAMEOVER_PATH;

	const extern char* BGM_STAGE2_1_PATH;
	const extern char* BGM_STAGE2_2_PATH;
	const extern char* BGM_STAGE2_3_PATH;
	const extern char* BGM_STAGE3_1_PATH;
	const extern char* BGM_STAGE3_2_PATH;
	const extern char* BGM_STAGE3_3_PATH;
	const extern char* BGM_STAGE1_BOSS_PATH;
	const extern char* BGM_STAGE2_BOSS_PATH;
	const extern char* BGM_STAGE3_BOSS_PATH;

	const extern char* EFFECT_PLAYERHIT_PATH;
	const extern char* EFFECT_PLAYERMOVE_PATH;
	const extern char* EFFECT_BEELZEBUBHIT_PATH;
	const extern char* EFFECT_PLAYERATTACK_PATH;
	const extern char* EFFECT_EVEHIT_PATH;
	const extern char* EFFECT_YSHIT_PATH;
	const extern char* EFFECT_KNIGHTHIT_PATH;
	const extern char* EFFECT_GHOSTHIT_PATH;
	const extern char* EFFECT_SUCCUBUSHIT_PATH;
	const extern char* EFFECT_LILITHHIT_PATH;

	const extern char* EFFECT_SOUND11_PATH;
	const extern char* EFFECT_SOUND12_PATH;
	const extern char* EFFECT_SOUND13_PATH;
}