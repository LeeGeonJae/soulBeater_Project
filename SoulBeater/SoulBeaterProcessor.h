#pragma once

#include "GameProcessor.h"

namespace d2dFramework
{
	class GameObject;
}

namespace soulBeater
{
	class SoulBeaterProcessor : public d2dFramework::GameProcessor
	{
	public:
		SoulBeaterProcessor(UINT width, UINT height, std::wstring name);
		virtual ~SoulBeaterProcessor() = default;

		virtual void Init() override;
		virtual void Release() override;

	private:
	};
}