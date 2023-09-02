#pragma once

#include "FrameInformation.h"

#include <d2d1.h>
#include <vector>
#include <cassert>

#include "ISerialize.h"

namespace d2dFramework
{

	class AnimationAsset 
	{
	public:
		AnimationAsset(ID2D1Bitmap* bitmap, const std::wstring& bitmapKey, const std::vector<std::vector<FrameInformation>>& frameInfo);
		~AnimationAsset();
		AnimationAsset(const AnimationAsset& other) = delete;
		AnimationAsset& operator=(const AnimationAsset& other) = delete;

		inline ID2D1Bitmap* GetBitmap() const;

		inline const std::vector<std::vector<FrameInformation>>& GetFrameInformations() const;
		inline const FrameInformation& GetFrameInformation(unsigned int animationIndex, unsigned int frameIndex) const;
		inline unsigned int GetAnimationSize() const;
		inline unsigned int GetFrameSize(unsigned int animationIndex) const;

	private:
		ID2D1Bitmap* mBitmap;
		std::wstring mBitmapKey;
		std::vector<std::vector<FrameInformation>> mFrameInformations;
	};

	ID2D1Bitmap* AnimationAsset::GetBitmap() const
	{
		return mBitmap;
	}
	const std::vector<std::vector<FrameInformation>>& AnimationAsset::GetFrameInformations() const
	{
		return mFrameInformations;
	}
	const FrameInformation& AnimationAsset::GetFrameInformation(unsigned int animationIndex, unsigned int frameIndex) const
	{
		assert(animationIndex < GetAnimationSize());
		assert(frameIndex < GetFrameSize(animationIndex));
		return mFrameInformations[animationIndex][frameIndex];
	}
	unsigned int AnimationAsset::GetAnimationSize() const
	{
		return static_cast<unsigned int>(mFrameInformations.size());
	}
	unsigned int AnimationAsset::GetFrameSize(unsigned int animationIndex) const
	{
		assert(animationIndex < GetAnimationSize());
		return static_cast<unsigned int>(mFrameInformations[animationIndex].size());
	}
}
