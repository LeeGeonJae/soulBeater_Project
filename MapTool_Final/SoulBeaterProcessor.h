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
		// create �� �� �����ǰ� �������� �ٷ� 2d �迭 ����
		unsigned int mWidth;
		unsigned int mHeight;
		unsigned int mGridDistance;

		std::vector<std::vector<bool>> mbIsChecked; // �̰� ���� ���鼭 Ŭ�� ó��
		std::vector<std::vector<bool>> mbIsObject; // ����� �ش� ����ĭ�� ������Ʈ�� �����ϴ���
		std::map<std::pair<int, int>, int> mObjectIdMap;

		// Ŭ���� �� ǥ�� ( �ӽ� ) 
		D2D1_RECT_F mRect;

		unsigned int tempId = 10000;

		MapToolGui* mImGui = new MapToolGui;
	};
}
