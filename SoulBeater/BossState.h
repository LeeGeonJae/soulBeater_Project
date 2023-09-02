#pragma once

namespace soulBeater
{
	enum class eBossState
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

	enum class ePhase
	{
		None,
		One,
		Two,
		Three,
		Four,
		Ending
	};

	enum class eMonsterType
	{
		Hold,
		Attack,
		Escape
	};
}