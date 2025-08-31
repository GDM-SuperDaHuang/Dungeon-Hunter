// daHuang

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AureAbilitySystemComponent.generated.h"

/**
 * 
 */
UCLASS()
class DUNGEONHUNTERS2_API UAureAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
	protected:

	void  EffectApplied(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle);
};
