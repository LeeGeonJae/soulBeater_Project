#pragma once

#include <vector>
#include <string>

namespace d2dFramework
{
	class IFixedUpdateable;
	class IUpdateable;
	class IRenderable;
	class RenderManager;

	class Scene
	{
	public:
		Scene(const std::string& string);
		virtual ~Scene() = default;

		virtual void Enter() {};
		virtual void Exit() {};

	private:
		std::string mName;
	};
}