#pragma once

namespace d2dFramework
{
	enum class eObjectType
	{
		None,
		Background,
		Tile,
		Node,
		Wall,
		Item,
		Enemy,
		Boss,
		Player,
		Projectile,				// ( 추가 : 이건재 ) 발사체..-	
		UI,						//(추가: 이동휴) UI
		Button,					//(추가: 이동휴) Button
		Size
	};
}