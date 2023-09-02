#pragma once

#include "../D2DFramework/GameProcessor.h"
#include "D2d1.h"
#include "gui.h"
#include "RenderManger.h"
#include "Scene.h"

#include <vector>
#include <map>

class Example;

namespace d2dFramework
{
	class GameObject;
}

namespace mapTool
{
	class SoulBeaterProcessor : public d2dFramework::GameProcessor
	{
	public:
		SoulBeaterProcessor(UINT width, UINT height, std::wstring name);
		virtual ~SoulBeaterProcessor() = default;

		virtual void Init(HWND hwnd) override;
		virtual void Update() override;
		virtual void Release() override;

	public:
		ID2D1Bitmap* GetBitmap(std::wstring key)
		{
			return d2dFramework::RenderManager::GetInstance()->GetBitmapOrNull(key.c_str());
		}
		HRESULT CreateBitmap(std::wstring key, std::wstring path)
		{
			return d2dFramework::RenderManager::GetInstance()->CreateD2DBitmapFromFile(key.c_str(), path.c_str());
		}

	private:
		void MouseClickCheck();
		void CheckedRender();
		void ColliderRender();

		void TileObjectCreate();
		void TileColliderCreate();
		void TileObjectDelete();
		void ObjectCreate();
		void ObjectDelete();
		void ColliderSetting();
		void SpriteSetting();

		void Render();

		void ImGuiImageLoading();

		void CreateScene();
		void SaveAndLoad();

	public:
		// 클릭한 곳 표시 ( 임시 ) 
		D2D1_RECT_F mRect;

		MapToolGui* mImGui;

		d2dFramework::Scene* MapToolScene;
	};
}
