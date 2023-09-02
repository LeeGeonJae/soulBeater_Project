#pragma once

namespace soulBeater
{
	enum eUISpriteType
	{
		StartUI,
		HelpUI,
		GameOverUI,
		InGameUI,		/// 한장만 넣어서 인게임~Setting까지 On /Off
		InBossUI,		/// 한장만 넣어서 인게임~Setting까지 On /Off
		PlayerHealthUI, /// 스프라이드르 4종류 넣어서 관리하는 방법?
		SoundMusicUI,
		InGamePauseUI,	/// 한장만 넣어서 InGamePause 에만
		SettingUI,		/// 한장만 넣어서 EndingScene 에만
		TimerUI,
		eUISpriteTypeEnd
	};
}