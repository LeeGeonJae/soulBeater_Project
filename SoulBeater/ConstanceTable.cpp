#include "ConstantTable.h"


namespace soulBeater
{
	const unsigned int BOSS_STAGE_TIME = 60;
	const unsigned int STAGE_TIME = 30;
	const unsigned int BOSS_TIME = 60;
	const unsigned int GRID_DISTANCE = 150;
	const unsigned int MAX_X = 20;
	const unsigned int MAX_Y = 20;

	const char* KNIGHT_EFFECT_EVENT = "knightDeathEffect";
	 
	const WCHAR* STAGE_SPRITE_KEY = L"./image/Bealzebub/fly_move.png";
	const WCHAR* PLAYER_NODE_SPRITE_KEY = L"nodeSpriteKey";
	const WCHAR* CUTSCENE_BEALZEBUB_SPRITE_KEY = L"cusSceneBealzeBubKey";
	const WCHAR* CUTSCENE_EVE_ESS_SPRITE_KEY = L"cutSceneEveEssKey";
	const WCHAR* CUTSCENE_LILITH_SPRITE_KEY = L"cutSceneLilithKey";
	const WCHAR* GAMEOVER_SPRITE_KEY = L"gameOverSpriteKey";
	const WCHAR* ENDING_SPRITE_KEY = L"endingSpriteKey";
	const WCHAR* HIT_EFFECT_SPRITE_KEY = L"hitEffectSpriteKey";

	const WCHAR* STAGE_SPRITE_PATH = L"./image/UI/StageName.png";
	const WCHAR* PLAYER_NODE_SPRITE_PATH = L"./image/Player/PlayerNode.png";
	const WCHAR* CUTSCENE_BEALZEBUB_PATH = L"./image/CutScene/beelzebub.png";
	const WCHAR* CUTSCENE_EVE_ESS_PATH = L"./image/CutScene/eve&ess.png";
	const WCHAR* CUTSCENE_LILITH_PATH = L"./image/CutScene/lilith.png";
	const WCHAR* GAMEOVER_SPRITE_PATH = L"./image/CutScene/game_over_base.png";
	const WCHAR* ENDING_SPRITE_PATH = L"./image/CutScene/game_clear_credits.png";
	const WCHAR* HIT_EFFECT_SPRITE_PATH = L"./image/hitEffect.png";

	const WCHAR* PLAYER_ANI_KEY = L"plyaerAnimaitionKey";
	const WCHAR* BEALZEBUB_ANI_KEY = L"bealzebubAnmationKey";
	const WCHAR* EVE_ANI_KEY = L"eveAnmationKey";
	const WCHAR* YS_ANI_KEY = L"ysAnmationKey";
	const WCHAR* LILITH_ANI_KEY = L"lilithAnmationKey";
	const WCHAR* FLY_ANI_KEY = L"flyAnimationKey";
	const WCHAR* GHOST_ANI_KEY = L"ghostAnimationKey";
	const WCHAR* SUCCUBUS_ANI_KEY = L"succubusAnimationKey";
	const WCHAR* KNIGHT_ANI_KEY = L"knightAnimationKey";

	const WCHAR* PLAYER_ANI_PATH = L"./image/Player/Player.png";
	const WCHAR* BEALZEBUB_ANI_PATH = L"./image/Bealzebub/bealzebub.png";
	const WCHAR* EVE_ANI_PATH = L"./image/Eve/Eve.png";
	const WCHAR* YS_ANI_PATH = L"./image/Ys/Ys.png";
	const WCHAR* LILITH_ANI_PATH = L"./image/Lilith/Lilith.png";
	const  WCHAR* FLY_ANI_PATH = L"./image/Monster/monster_sprite.png";
	const  WCHAR* GHOST_ANI_PATH = L"./image/Monster/monster_sprite.png";
	const  WCHAR* SUCCUBUS_ANI_PATH = L"./image/Monster/monster_sprite.png";
	const  WCHAR* KNIGHT_ANI_PATH = L"./image/Monster/monster_sprite.png";

	const WCHAR* WARNING_ANI_KEY = L"Warning";
	const WCHAR* BOOM_ANI_KEY = L"Boom";

	const WCHAR* WARNING_ANI_PATH = L"./image/Object/Warning.png";
	const WCHAR* BOOM_ANI_PATH = L"./image/Object/Boom.png";

	const d2dFramework::Vector2 STAGE_NAME_OBJECT_TRANSLATE = { 1727, 887 };
	const D2D1_RECT_F STAGE_NAME[9] =
	{
		{0, 0, 189, 104},
		{189, 0, 378, 104},
		{378, 0, 567, 104},
		{567, 0, 756, 104},
		{756, 0, 945, 104},
		{945, 0, 1134, 104},
		{1134, 0, 1323, 104},
		{1323, 0, 1512, 104},
		{1512, 0, 1701, 104},
	};

	const int BGM_TITLE_KEY = 80001;
	const int BGM_STAGE1_1_KEY = 80002;
	const int BGM_STAGE1_2_KEY = 80003;
	const int BGM_STAGE1_3_KEY = 80004;
	const int BGM_GAMEOVER_KEY = 80005;

	const int BGM_STAGE2_1_KEY = 80006;
	const int BGM_STAGE2_2_KEY = 80007;
	const int BGM_STAGE2_3_KEY = 80008;
	const int BGM_STAGE3_1_KEY = 80009;
	const int BGM_STAGE3_2_KEY = 80010;
	const int BGM_STAGE3_3_KEY = 80011;
	const int BGM_STAGE1_BOSS_KEY = 80012;
	const int BGM_STAGE2_BOSS_KEY = 80013;
	const int BGM_STAGE3_BOSS_KEY = 80027;

	const int EFFECT_PLAYERHIT_KEY = 80014;
	const int EFFECT_PLAYERMOVE_KEY = 80015;
	const int EFFECT_BEELZEBUBHIT_KEY = 80016;
	const int EFFECT_PLAYERATTACK_KEY = 80017;
	const int EFFECT_EVEHIT_KEY = 80018;
	const int EFFECT_YSHIT_KEY = 80019;
	const int EFFECT_KNIGHTHIT_KEY = 80020;
	const int EFFECT_GHOSTHIT_KEY = 80021;
	const int EFFECT_SUCCUBUSHIT_KEY = 80022;
	const int EFFECT_LILITHHIT_KEY = 80023;
	const int EFFECT_SOUND11_KEY = 80024;
	const int EFFECT_SOUND12_KEY = 80025;
	const int EFFECT_SOUND13_KEY = 80026;

	const char* BGM_TITLE_PATH = "./sound/BGM/Title.mp3";
	const char* BGM_STAGE1_1_PATH = "./sound/BGM/Stage1_1.mp3";
	const char* BGM_STAGE1_2_PATH = "./sound/BGM/Stage1_2.mp3";
	const char* BGM_STAGE1_3_PATH = "./sound/BGM/Stage1_3.mp3";
	const char* BGM_GAMEOVER_PATH = "./sound/BGM/Gameover.mp3";

	const char* BGM_STAGE2_1_PATH = "./sound/BGM/Stage2_1.mp3";
	const char* BGM_STAGE2_2_PATH = "./sound/BGM/Stage2_2.mp3";
	const char* BGM_STAGE2_3_PATH = "./sound/BGM/Stage2_3.mp3";
	const char* BGM_STAGE3_1_PATH = "./sound/BGM/Stage3_1.mp3";
	const char* BGM_STAGE3_2_PATH = "./sound/BGM/Stage3_2.mp3";
	const char* BGM_STAGE3_3_PATH = "./sound/BGM/Stage3_3.mp3";
	const char* BGM_STAGE1_BOSS_PATH = "./sound/BGM/StageBoss1.mp3";
	const char* BGM_STAGE2_BOSS_PATH = "./sound/BGM/Stage2_Boss.mp3";
	const char* BGM_STAGE3_BOSS_PATH = "./sound/BGM/Stage3_Boss.mp3";

	const char* EFFECT_PLAYERHIT_PATH = "./sound/effect/PlayerHit.mp3";
	const char* EFFECT_PLAYERMOVE_PATH = "./sound/effect/PlayerMove.wav";
	const char* EFFECT_BEELZEBUBHIT_PATH = "./sound/effect/BeelzebubHit.mp3";
	const char* EFFECT_PLAYERATTACK_PATH = "./sound/effect/PlayerAttack.wav";
	const char* EFFECT_EVEHIT_PATH = "./sound/effect/EveHit.mp3";
	const char* EFFECT_YSHIT_PATH = "./sound/effect/YsHit.mp3";
	const char* EFFECT_KNIGHTHIT_PATH = "./sound/effect/KnightHit.wav";
	const char* EFFECT_GHOSTHIT_PATH = "./sound/effect/GhostHit.wav";
	const char* EFFECT_SUCCUBUSHIT_PATH = "./sound/effect/SuccubusHit.mp3";
	const char* EFFECT_LILITHHIT_PATH = "./sound/effect/LilithHit.mp3";

	const char* EFFECT_SOUND11_PATH = "./sound/effect/FECT11.mp3";
	const char* EFFECT_SOUND12_PATH = "./sound/effect/FECT12.mp3";
	const char* EFFECT_SOUND13_PATH = "./sound/effect/FECT13.mp3";
}
