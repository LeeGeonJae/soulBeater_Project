#include "AnimationAsset.h"

namespace d2dFramework
{
	AnimationAsset::AnimationAsset(ID2D1Bitmap* bitmap, const std::wstring& bitmapKey, const std::vector<std::vector<FrameInformation>>& FrameInformations)
		: mBitmap(bitmap)
		, mBitmapKey(bitmapKey)
		, mFrameInformations(FrameInformations)
	{
		int count = mBitmap->AddRef();
	}

	AnimationAsset::~AnimationAsset()
	{
		int count = mBitmap->Release();
		mBitmap = nullptr;
	}
}