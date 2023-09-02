#pragma once
enum eButtonType
{
	StartButton,
	HelpButton,
	ExitButton,
	ResumeButton,
	SettingButton,
	MenuButton,
	SoundButton,
	MusicButton,
	eButtonTypeEnd
};
enum eButtonStatus
{
	FocusOut,
	FocusOn,
	Pushed,
	End
};
enum class eUIManagerType
{
	StartScene,
	InGame,
	InGamePause,
	SettingOption,
	EndingScene,
	None
};

enum eUISpriteType
{
	StartUI,
	InGameUI,		/// 한장만 넣어서 인게임~Setting까지 On /Off
	PlayerHealthUI, /// 스프라이드르 4종류 넣어서 관리하는 방법?
	SoundMusicUI,
	InGamePauseUI,	/// 한장만 넣어서 InGamePause 에만
	SettingUI,		/// 한장만 넣어서 EndingScene 에만
	TimerUI,
	eUISpriteTypeEnd
};
