#include "Scene.h"

#include "GameObject.h"
#include "ICollideable.h"
#include "IFixedUpdateable.h"
#include "IUpdateable.h"
#include "IRenderable.h"
#include "RenderManger.h"

namespace d2dFramework
{
	Scene::Scene(const std::string& name)
		: mName(name)
	{
	}
}