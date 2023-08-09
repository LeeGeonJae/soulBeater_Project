#pragma once

#include "../D2DFramework/GameProcessor.h"
#include "D2d1.h"
#include "gui.h"

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

		virtual void Init() override;
		virtual void Update() override;
		virtual void Release() override;

	private:
		void MouseClickCheck();
		void CheckedRender();
		void ObjectCreate();
		void ObjectDelete();

		void Render();

		void ImGuiImageLoading();

	public:
		// create 할 시 격자판과 격자판을 다룰 2d 배열 생성
		unsigned int mWidth;
		unsigned int mHeight;
		unsigned int mGridDistance;

		std::vector<std::vector<bool>> mbIsChecked; // 이거 루프 돌면서 클릭 처리
		std::vector<std::vector<bool>> mbIsObject; // 여기는 해당 격자칸에 오브젝트가 존재하는지
		std::map<std::pair<int, int>, int> mObjectIdMap;

		// 클릭한 곳 표시 ( 임시 ) 
		D2D1_RECT_F mRect;

		unsigned int tempId = 10000;

		MapToolGui* mImGui = new MapToolGui;
	};
}
