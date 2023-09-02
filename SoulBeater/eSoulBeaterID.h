#pragma once

#include "eFrameworkID.h"

namespace soulBeater
{
	enum class eSoulBeaterID
	{
		Processor = static_cast<unsigned int>(d2dFramework::eFrameworkID::Size) + 1u,
		PlayerFSM,
		MonsterFSM,
		BealzebubBossFSM,
		YsFSM,
		EveFSM,
		LilithFSM,
		PlayerObject,
		PlayerNodeObject,
		BelzebubObject,
		YsObject,
		EveObject,
		LilithObject,
		StageNameObject,
		TimerObject,
		HPObject,
		TitleParticle,
		BeatParticle1,
		BeatParticle2,
		BeatParticle3,
		BeatParticle4,
		KnightDeathParticle,
		GhostDeathParticle,
		SuccubusDeathParticle,
		CutScene1Object,
		CutScene2Object,
		CutScene3Object,
		CutSceneEndingObject,
		CutSceneGameOverObject,
		StartScene = 400000,
		EndingScene,
		Stage1_1 = 50000,
		Stage1_2,
		Stage1_3,
		Stage2_1,
		Stage2_2,
		Stage2_3,
		Stage3_1,
		Stage3_2,
		Stage3_3,
		CutScene1 = 600000,
		CutScene2,
		CutScene3,
		BossScene1 = 50100,
		BossScene2 = 50200,
		BossScene3 = 50300,
		GameOverScene = 50301
	};
}