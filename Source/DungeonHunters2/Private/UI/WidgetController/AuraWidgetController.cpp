// daHuang


#include "UI/WidgetController/AuraWidgetController.h"

#include "AbilitySystem/AureAbilitySystemComponent.h"
#include "AbilitySystem/AureAttributeSet.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "Palyer/AuraPlayerController.h"
#include "Palyer/AurePlayerState.h"

void UAuraWidgetController::SetWidgetControllerParams(const FWidgetControllerParams& WCParams)
{
	PlayerController = WCParams.PlayerController;
	PlayerState = WCParams.PlayerState;
	AbilitySystemComponent = WCParams.AbilitySystemComponent;
	AttributeSet = WCParams.AttributeSet;
}

void UAuraWidgetController::BroadcastInitialValues()
{
}

void UAuraWidgetController::BindCallbacksToDependencies()
{
}

void UAuraWidgetController::BroadcastAbilityInfo()
{
	if (!GetAuraASC()->bStartUpAbilitiesGiven) return;

	FForEachAbility BroadcastDelegate;
	BroadcastDelegate.BindLambda([this](const FGameplayAbilitySpec& AbilitySpec)
	{
		FAuraAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(
			AureAbilitySystemComponent->GetAbilityTagFromSpec(AbilitySpec));
		Info.InputTag = AureAbilitySystemComponent->GetInputTagFromSpec(AbilitySpec);
		Info.StatusTag = AureAbilitySystemComponent->GetStatusFromSpec(AbilitySpec);
		AbilityInfoDelegate.Broadcast(Info);
	});
	GetAuraASC()->FForEachAbility(BroadcastDelegate);
}

AAuraPlayerController* UAuraWidgetController::GetAuraPC()
{
	if (AureAbilitySystemComponent == nullptr)
	{
		AuraPlayerController = Cast<AAuraPlayerController>(PlayerController);
	}
	return AuraPlayerController;
}

AAurePlayerState* UAuraWidgetController::GetAuraPS()
{
	if (AurePlayerState == nullptr)
	{
		AurePlayerState = Cast<AAurePlayerState>(PlayerState);
	}
	return AurePlayerState;
}

UAureAbilitySystemComponent* UAuraWidgetController::GetAuraASC()
{
	if (AureAbilitySystemComponent == nullptr)
	{
		AureAbilitySystemComponent = Cast<UAureAbilitySystemComponent>(AbilitySystemComponent);
	}
	return AureAbilitySystemComponent;
}

UAureAttributeSet* UAuraWidgetController::GetAuraAS()
{
	if (AureAttributeSet == nullptr)
	{
		AureAttributeSet = Cast<UAureAttributeSet>(AttributeSet);
	}
	return AureAttributeSet;
}
