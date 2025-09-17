// daHuang


#include "UI/WidgetController/AttributeMenuWidgetController.h"
#include "AbilitySystem/AureAttributeSet.h"
#include "AuraGameplayTags.h"

void UAttributeMenuWidgetController::BindCallbacksToDependencies()
{
}

void UAttributeMenuWidgetController::BroadcastInitialValues()
{
	UAureAttributeSet* AS = CastChecked<UAureAttributeSet>(AttributeSet);
	check(AttributeInfo);
	FAuraAttributeInfo Info = AttributeInfo->FindAttributeInfoForTag(FAuraGameplayTags::Get().Attributes_Primary_Strength);
	Info.AttributeValue = AS->GetStrength();
	AttributeInfoDelegate.Broadcast(Info);
}
