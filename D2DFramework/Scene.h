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

		virtual void Enter( );
		virtual void Exit();

		GameObject* CreateObject(unsigned int id);

		inline const std::set<unsigned int>& GetObjectIDs();
		inline const std::string& GetName();
		void SerializeIn(nlohmann::ordered_json& object) override;
		void SerializeOut(nlohmann::ordered_json& object) override;
		void SetGrid(int Width, int Height, int Distance) {mGridWidth = Width; mGridHeight = Height; mGridDistance = Distance;}

	private:
		std::string mName;
		std::set<unsigned int> mObjectIDs;
		int mGridWidth;
		int mGridHeight;
		int mGridDistance;

	};

	const std::set<unsigned int>& Scene::GetObjectIDs()
	{
		return mObjectIDs;
	}
	const std::string& Scene::GetName()
	{
		return mName;
	}
}