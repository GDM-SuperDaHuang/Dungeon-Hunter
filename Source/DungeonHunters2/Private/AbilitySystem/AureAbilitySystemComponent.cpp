// daHuang


#include "AbilitySystem/AureAbilitySystemComponent.h"

#include "AuraGameplayTags.h"

void UAureAbilitySystemComponent::AbilityActorInfoSet()
{
	UE_LOG(LogTemp, Log, TEXT("UAureAbilitySystemComponent created: %p"), this);
	//当前 UAureAbilitySystemComponent 所属的 Actor 被应用 GameplayEffect（游戏性效果）时自动触发
	//如，ApplyGameplayEffectSpecToSelf，SetNumericAttributeBase，的调用 会触发
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UAureAbilitySystemComponent::EffectApplied);
}

void UAureAbilitySystemComponent::AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartUpAbility)
{
	for (TSubclassOf<UGameplayAbility> AbilityClass : StartUpAbility)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
		// GiveAbility(AbilitySpec);
		GiveAbilityAndActivateOnce(AbilitySpec);
		
	}
}

// 效果应用时的回调处理
void UAureAbilitySystemComponent::EffectApplied(UAbilitySystemComponent* AbilitySystemComponent,
                                                const FGameplayEffectSpec& EffectSpec,
                                                FActiveGameplayEffectHandle ActiveEffectHandle)
{
	// GEngine->AddOnScreenDebugMessage(1,8.f,FColor::Blue,FString("EffectApplied"));
	FGameplayTagContainer TagContainer;
	EffectSpec.GetAllAssetTags(TagContainer);
	EffectAssetTags.Broadcast(TagContainer);

	// for ( const FGameplayTag& Tag:TagContainer)
	// {
	// 	const FString Msg = FString::Printf(TEXT("tag:%s"), *Tag.ToString());
	// 	GEngine->AddOnScreenDebugMessage(-1,8.f,FColor::Blue,Msg);
	// }
}
