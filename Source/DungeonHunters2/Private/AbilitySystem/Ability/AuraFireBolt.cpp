// daHuang


#include "AbilitySystem/Ability/AuraFireBolt.h"

FString UAuraFireBolt::GetDescription(int32 Level)
{
	const float ManaCost = GetManaCost(Level);
	float Cooldown = GetCooldown(Level);
	if (Level == 1)
	{
		return FString::Printf(TEXT("%d,%f,%f"), NumProjectiles, ManaCost, Cooldown);
	}
	else
	{
		return FString::Printf(TEXT(""));
	}
}

FString UAuraFireBolt::GetNextLevelDescription(int32 Level)
{
	if (Level == 1)
	{
		return FString::Printf(TEXT(""));
	}
	else
	{
		return FString::Printf(TEXT(""));
	}
}
