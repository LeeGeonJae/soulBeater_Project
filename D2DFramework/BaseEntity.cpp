#include "BaseEntity.h"

#include <cassert>

namespace d2dFramework
{
	std::unordered_set<unsigned int> BaseEntity::mIDSet;

	BaseEntity::BaseEntity(unsigned int id)
		: mId(id)
	{
		assert(mIDSet.find(id) == mIDSet.end());
		mIDSet.insert(id);
	}

	BaseEntity::~BaseEntity()
	{
		assert(mIDSet.find(mId) != mIDSet.end());
		mIDSet.erase(mId);
	}
}