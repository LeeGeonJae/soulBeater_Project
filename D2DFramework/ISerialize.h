#pragma once
#include <string>
#include "json.hpp"

namespace d2dFramework
{
	class ISerialize
	{
	public:
		virtual ~ISerialize() = default;
		virtual void SerializeOut(nlohmann::ordered_json& object) = 0;
		virtual void SerializeIn(nlohmann::ordered_json& object) = 0;
	};
}
