#pragma once

#include "BaseEntity.h"
#include "ISerialize.h"

#include <vector>
#include <string>
#include <set>

namespace d2dFramework
{
	class SceneManager;
	class IFixedUpdateable;
	class IUpdateable;
	class IRenderable;
	class RenderManager;
	class GameObject;

	class Scene : public BaseEntity, public ISerialize
	{
	public:
		Scene(unsigned int id);
		virtual ~Scene() = default;

		virtual void Enter();
		virtual void Exit();

		void SerializeIn(nlohmann::ordered_json& object) override;
		void SerializeOut(nlohmann::ordered_json& object) override;

		inline const std::string& GetName();

	private:
		std::string mName;
	};

	const std::string& Scene::GetName()
	{
		return mName;
	}
}
