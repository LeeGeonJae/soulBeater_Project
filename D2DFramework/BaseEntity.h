#pragma once

#include <unordered_set>

namespace d2dFramework
{
	class BaseEntity abstract
	{
	public:
		BaseEntity(unsigned int id);
		virtual ~BaseEntity() = 0;

		inline unsigned int GetId() const;

		// 디버깅용 명시적 호출 절대 금지
		static void ClearIdMap()
		{
			mIDSet.clear();
		}

	private:
		static std::unordered_set<unsigned int> mIDSet;

		unsigned int mId;
		// 로깅 남길 거면 개체 생성 삭제 시간 정도 추가될 수 있을 거 같다.
	};

	unsigned int BaseEntity::GetId() const
	{
		return mId;
	}
}