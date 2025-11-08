// daHuang


#include "UI/WidgetController/AttributeMenuWidgetController.h"

#include <utility>

#include "AbilitySystem/AureAttributeSet.h"
#include "AuraGameplayTags.h"

void UAttributeMenuWidgetController::BindCallbacksToDependencies()
{
	UAureAttributeSet* AS = Cast<UAureAttributeSet>(AttributeSet);
	for (TPair<FGameplayTag, FGameplayAttribute(*)()> Pair : AS->TagsToAttributes)
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
}

void UAttributeMenuWidgetController::BroadcastInitialValues()
{
	UAureAttributeSet* AS = CastChecked<UAureAttributeSet>(AttributeSet);
	check(AttributeInfo);

	/**
	*	FAuraAttributeInfo Info = AttributeInfo->FindAttributeInfoForTag(FAuraGameplayTags::Get().Attributes_Primary_Strength);
	*	Info.AttributeValue = AS->GetStrength();
	*	AttributeInfoDelegate.Broadcast(Info);
	*/
	for (TPair<FGameplayTag, FGameplayAttribute(*)()> Pair : AS->TagsToAttributes)
	{
		BroadcastAttributeInfo(Pair.Key, Pair.Value());
		// FAuraAttributeInfo Info = AttributeInfo->FindAttributeInfoForTag(Pair.Key);
		// // FGameplayAttribute Attribute = Pair.Value.Execute();
		// Info.AttributeValue =  Pair.Value().GetNumericValue(AS);
		// AttributeInfoDelegate.Broadcast(Info);
	}

}

void UAttributeMenuWidgetController::BroadcastAttributeInfo(const FGameplayTag& AttributeTag,
	const FGameplayAttribute& Attribute) const
{
	FAuraAttributeInfo Info = AttributeInfo->FindAttributeInfoForTag(AttributeTag);
	// FGameplayAttribute Attribute = Pair.Value.Execute();
	Info.AttributeValue = Attribute.GetNumericValue(AttributeSet);
	AttributeInfoDelegate.Broadcast(Info);
}
