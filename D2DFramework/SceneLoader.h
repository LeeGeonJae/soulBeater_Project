#pragma once

#include <string>

namespace d2dFramework
{
	class RenderManager;
	class Scene;

	// static class
	class SceneLoader
	{
	public:
		static void LoadScene(Scene* outScene);

		static void SaveScene(Scene* outScene);
		static void LoadAllAnimationAssets();
		static void LoadAllBitmaps();

			//{
			//	///AnimationJsonList에서 AnimationKey에 맞는
			//	///string tempstr = imgpath; string tempstr2 = framesFilePath
			//	///framesFilePath =CSV로 관리


			//}


	};
}