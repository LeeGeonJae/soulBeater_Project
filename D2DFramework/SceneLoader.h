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
		static void LoadAllAnimationAssets(RenderManager* renderManager);
		static void LoadAllBitmaps(RenderManager* renderManager);

			//{
			//	///AnimationJsonList���� AnimationKey�� �´�
			//	///string tempstr = imgpath; string tempstr2 = framesFilePath
			//	///framesFilePath =CSV�� ����


			//}


	};
}