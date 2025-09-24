// daHuang


#include "AbilitySystem/AureAbilitySystemComponent.h"
#include "AbilitySystem/Ability/AuraGameplayAbility.h"
#include "AuraGameplayTags.h"

void UAureAbilitySystemComponent::AbilityActorInfoSet()
{
	UE_LOG(LogTemp, Log, TEXT("UAureAbilitySystemComponent created: %p"), this);
	//当前 UAureAbilitySystemComponent 所属的 Actor 被应用 GameplayEffect（游戏性效果）时自动触发
	//如，ApplyGameplayEffectSpecToSelf，SetNumericAttributeBase，的调用 会触发
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UAureAbilitySystemComponent::ClientEffectApplied);
}

void UAureAbilitySystemComponent::AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartUpAbility)
{
	for (const TSubclassOf<UGameplayAbility> AbilityClass : StartUpAbility)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
		if (const UAuraGameplayAbility* AuraAbility = Cast<UAuraGameplayAbility>(AbilitySpec.Ability))
		{
			AbilitySpec.DynamicAbilityTags.AddTag(AuraAbility->StartupInputTag);
			GiveAbility(AbilitySpec);
		}
		
		// GiveAbilityAndActivateOnce(AbilitySpec);
	}
}

void UAureAbilitySystemComponent::AbilityInputTagHeld(const FGameplayTag& InputTags)
{
	if (!InputTags.IsValid()) return;

	for (FGameplayAbilitySpec AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.DynamicAbilityTags.HasTag(InputTags))
		{
			AbilitySpecInputPressed(AbilitySpec);
			if (!AbilitySpec.IsActive())
			{
				TryActivateAbility(AbilitySpec.Handle);
			}
		}
	}
	
}

void UAureAbilitySystemComponent::AbilityInputTagReleased(const FGameplayTag& InputTags)
{
	if (!InputTags.IsValid()) return;

	for (FGameplayAbilitySpec AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.DynamicAbilityTags.HasTag(InputTags))
		{
			AbilitySpecInputReleased(AbilitySpec);
		}
	}
}

// 效果应用时的回调处理
void UAureAbilitySystemComponent::ClientEffectApplied_Implementation(UAbilitySystemComponent* AbilitySystemComponent,
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
