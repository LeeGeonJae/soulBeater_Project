#pragma once

namespace TestGeon
{
	enum class eYsAndEveBossState
	{
		Idle,
		Trace,
		AttackStandby,
		Attack,
		BeAttacked,
		Death
	};

	enum class eBossTurn
	{
		StayTurn,
		ActionStandbyTurn,
		ActionTurn
	};
}