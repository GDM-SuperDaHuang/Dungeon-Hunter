// daHuang


#include "AbilitySystem/AureAbilitySystemComponent.h"

void UAureAbilitySystemComponent::AbilityActorInfoSet()
{
	UE_LOG(LogTemp, Log, TEXT("UAureAbilitySystemComponent created: %p"), this);
	//当前 UAureAbilitySystemComponent 所属的 Actor 被应用 GameplayEffect（游戏性效果）时自动触发
	//如，ApplyGameplayEffectSpecToSelf，SetNumericAttributeBase，的调用 会触发
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this,&UAureAbilitySystemComponent::EffectApplied);
}

void UAureAbilitySystemComponent::EffectApplied(UAbilitySystemComponent* AbilitySystemComponent,
                                                const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle)
{
	GEngine->AddOnScreenDebugMessage(1,8.f,FColor::Blue,FString("EffectApplied"));
}
