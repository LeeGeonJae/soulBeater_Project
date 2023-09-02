#pragma once

namespace d2dFramework
{
	class BeatManager;
}

namespace soulBeater
{
	class Loader
	{
	public:
		static void LoadD2DBitmap();
		static void LoadAnimationAsset(d2dFramework::BeatManager* beatmanager);
		static void LoadSoundAsset();
	};
}