// daHuang


#include "UI/WidgetController/AttributeMenuWidgetController.h"

#include <utility>

#include "AbilitySystem/AureAttributeSet.h"
#include "AuraGameplayTags.h"
#include "RenderGraphResources.h"
#include "AbilitySystem/AureAbilitySystemComponent.h"
#include "Palyer/AurePlayerState.h"

void UAttributeMenuWidgetController::BindCallbacksToDependencies()
{
	// UAureAttributeSet* AS = Cast<UAureAttributeSet>(AttributeSet);
	check(AttributeInfo);
	for (TPair<FGameplayTag, FGameplayAttribute(*)()> Pair : GetAuraAS()->TagsToAttributes)
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Pair.Value()).AddLambda(
			[this,Pair](const FOnAttributeChangeData& Data)
			{
				BroadcastAttributeInfo(Pair.Key, Pair.Value());
				// FAuraAttributeInfo Info = AttributeInfo->FindAttributeInfoForTag(Pair.Key);
				// float NumericValue = Pair.Value().GetNumericValue(AS);
				// AttributeInfoDelegate.Broadcast(Info);
			});
	}

	// AAurePlayerState* AurePlayerState = CastChecked<AAurePlayerState>(PlayerState);
	GetAuraPS()->OnAttributePointsChangedDelegate.AddLambda(
		[this](int32 Points)
		{
			OnAttributePointsChangedDelegate.Broadcast(Points);
		});
}

void UAttributeMenuWidgetController::BroadcastInitialValues()
{
	// UAureAttributeSet* AS = CastChecked<UAureAttributeSet>(AttributeSet);
	check(AttributeInfo);

	/**
	*	FAuraAttributeInfo Info = AttributeInfo->FindAttributeInfoForTag(FAuraGameplayTags::Get().Attributes_Primary_Strength);
	*	Info.AttributeValue = AS->GetStrength();
	*	AttributeInfoDelegate.Broadcast(Info);
	*/
	for (TPair<FGameplayTag, FGameplayAttribute(*)()> Pair :  GetAuraAS()->TagsToAttributes)
	{
		BroadcastAttributeInfo(Pair.Key, Pair.Value());
		// FAuraAttributeInfo Info = AttributeInfo->FindAttributeInfoForTag(Pair.Key);
		// // FGameplayAttribute Attribute = Pair.Value.Execute();
		// Info.AttributeValue =  Pair.Value().GetNumericValue(AS);
		// AttributeInfoDelegate.Broadcast(Info);
	}
	// AAurePlayerState* AurePlayerState = CastChecked<AAurePlayerState>(PlayerState);
	OnAttributePointsChangedDelegate.Broadcast(GetAuraPS()->GetAttributePoints());

}

void UAttributeMenuWidgetController::UpgradeAttributes(const FGameplayTag& AttributeTag)
{
	UAureAbilitySystemComponent* AuraASC = CastChecked<UAureAbilitySystemComponent>(AbilitySystemComponent);
	AuraASC->UpgradeAttributes(AttributeTag);

}

void UAttributeMenuWidgetController::BroadcastAttributeInfo(const FGameplayTag& AttributeTag,
                                                            const FGameplayAttribute& Attribute) const
{
	FAuraAttributeInfo Info = AttributeInfo->FindAttributeInfoForTag(AttributeTag);
	// FGameplayAttribute Attribute = Pair.Value.Execute();
	Info.AttributeValue = Attribute.GetNumericValue(AttributeSet);
	AttributeInfoDelegate.Broadcast(Info);
}
