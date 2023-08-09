#include "IRenderable.h"
#include "GameProcessor.h"
#include "RenderManger.h"

namespace d2dFramework
{
	RenderManager* IRenderable::mRenderManager = nullptr;

	void IRenderable::SetRenderManager(RenderManager* renderManager)
	{
		mRenderManager = renderManager;
	}

	void IRenderable::Init()
	{
		GetRenderManager()->RegisterRenderable(this);
	}

	void IRenderable::Release()
	{
		GetRenderManager()->UnregisterRenderable(this);
	}
}