#include "NodeComponent.h"

#include "IncludeComponent.h"

#include "GameObject.h"
#include "RenderManger.h"

namespace TestGeon
{

	NodeComponent::NodeComponent(unsigned int id, d2dFramework::GameObject* owner)
		: SpriteRenderer::SpriteRenderer(id, owner)
		, mNodeState(eNodeState::UnableMove)
	{
	}

	void NodeComponent::Init()
	{
		SpriteRenderer::Init();
	}

	void NodeComponent::Release()
	{
		SpriteRenderer::Release();
	}

	void NodeComponent::Render(const D2D1::Matrix3x2F& cameraTransform)
	{
		using namespace d2dFramework;

		if (!GetIsActive())
		{
			return;
		}

		Transform* transform = GetGameObject()->GetComponent<Transform>();
		D2D1::Matrix3x2F matrix = D2D1::Matrix3x2F::Scale(GetIsLeft() ? -1 : 1, -1) * transform->GetTransform();

		switch (GetTransformLayer())
		{
		case eTransformLayer::None:
			GetRenderManager()->SetTransform(matrix * cameraTransform);
			break;
		case eTransformLayer::Grid:
			GetRenderManager()->SetTransform(matrix * GetRenderManager()->GetGridObjectTransform());
			break;
		case eTransformLayer::UI:
			GetRenderManager()->SetTransform(matrix * GetRenderManager()->GetUITransform());
			break;
		default:
			break;
		}

		nodeRender();
	}
	void NodeComponent::nodeRender()
	{
		using namespace d2dFramework;

		D2D1_COLOR_F prevColor = GetRenderManager()->SetColor(GetBorderColor());

		GridComponent* grid = GetGameObject()->GetComponent<GridComponent>();

		switch (mNodeState)
		{
		case TestGeon::eNodeState::UnableMove:
			assert(mBitmap != nullptr);
			GetRenderManager()->DrawBitMap( { GetOffset().GetX() + (float)grid->GetCellDistance(), GetOffset().GetY() }, mSize, mUVRectangle, mBitmap);
			GetRenderManager()->DrawBitMap( { GetOffset().GetX() - (float)grid->GetCellDistance(), GetOffset().GetY() }, mSize, mUVRectangle, mBitmap);
			GetRenderManager()->DrawBitMap( { GetOffset().GetX(), GetOffset().GetY() + (float)grid->GetCellDistance() }, mSize, mUVRectangle, mBitmap);
			GetRenderManager()->DrawBitMap( { GetOffset().GetX(), GetOffset().GetY() - (float)grid->GetCellDistance() }, mSize, mUVRectangle, mBitmap);
			break;
		case TestGeon::eNodeState::Moveable:
			assert(mBitmap != nullptr);
			GetRenderManager()->DrawBitMap({ GetOffset().GetX() + (float)grid->GetCellDistance(), GetOffset().GetY() }, mSize, mUVRectangle, mBitmap);
			GetRenderManager()->DrawBitMap({ GetOffset().GetX() - (float)grid->GetCellDistance(), GetOffset().GetY() }, mSize, mUVRectangle, mBitmap);
			GetRenderManager()->DrawBitMap({ GetOffset().GetX(), GetOffset().GetY() + (float)grid->GetCellDistance() }, mSize, mUVRectangle, mBitmap);
			GetRenderManager()->DrawBitMap({ GetOffset().GetX(), GetOffset().GetY() - (float)grid->GetCellDistance() }, mSize, mUVRectangle, mBitmap);
			break;
		case TestGeon::eNodeState::MoveSuccess:
			//GetRenderManager()->SetColor(GetBaseColor());

			//GetRenderManager()->FillRectangle(GetOffset(), { (float)grid->GetCellDistance(), (float)grid->GetCellDistance() });
			break;
		case TestGeon::eNodeState::MoveFailed:
			//GetRenderManager()->SetColor(GetBaseColor());

			//GetRenderManager()->FillRectangle({ GetOffset().GetX() - (float)grid->GetCellDistance(), GetOffset().GetY() }
			//, { (float)grid->GetCellDistance(), (float)grid->GetCellDistance() });
			//GetRenderManager()->FillRectangle({ GetOffset().GetX() + (float)grid->GetCellDistance(), GetOffset().GetY() }
			//, { (float)grid->GetCellDistance(), (float)grid->GetCellDistance() });
			//GetRenderManager()->FillRectangle({ GetOffset().GetX(), GetOffset().GetY() + (float)grid->GetCellDistance() }
			//, { (float)grid->GetCellDistance(), (float)grid->GetCellDistance() });
			//GetRenderManager()->FillRectangle({ GetOffset().GetX(), GetOffset().GetY() - (float)grid->GetCellDistance() }
			//, { (float)grid->GetCellDistance(), (float)grid->GetCellDistance() });
			break;
		default:
			assert(false);
			break;
		}

		GetRenderManager()->SetTransform(D2D1::Matrix3x2F::Identity());
		GetRenderManager()->SetColor(prevColor);
	}
}
