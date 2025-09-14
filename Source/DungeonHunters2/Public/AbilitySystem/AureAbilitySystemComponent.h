// daHuang

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AureAbilitySystemComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FEffectAssetTags,const FGameplayTagContainer&);

/**
 * 
 */
UCLASS()
class DUNGEONHUNTERS2_API UAureAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
public:
	void AbilityActorInfoSet();
	FEffectAssetTags EffectAssetTags;
protected:
	// 自定义效果应用时的回调函数
	void EffectApplied(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle);
};
