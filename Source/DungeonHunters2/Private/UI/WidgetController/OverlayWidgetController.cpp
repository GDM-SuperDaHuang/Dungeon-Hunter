// daHuang


#include "UI/WidgetController/OverlayWidgetController.h"
#include "AbilitySystem/AureAttributeSet.h"
void UOverlayWidgetController::BroadcastIntialValues()
{
	UAureAttributeSet* AuraAttributeSet = CastChecked<UAureAttributeSet>(AttributeSet);

	//广播
	OnHealthChanged.Broadcast(AuraAttributeSet->GetHealth());
	OnMaxHealthChanged.Broadcast(AuraAttributeSet->GetMaxHealth());

	
}

void UOverlayWidgetController::BindCallbacksToDependences()
{
	// Super::BindCallbacksToDependences();
	const UAureAttributeSet* AuraAttributeSet = CastChecked<UAureAttributeSet>(AttributeSet);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetHealthAttribute()).AddUObject(this,&UOverlayWidgetController::HealthChanged);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetMaxHealthAttribute()).AddUObject(this,&UOverlayWidgetController::MaxHealthChanged);

}

void UOverlayWidgetController::HealthChanged(const FOnAttributeChangeData& Data) const
{
	OnHealthChanged.Broadcast(Data.NewValue);
}

void UOverlayWidgetController::MaxHealthChanged(const FOnAttributeChangeData& Data) const
{
	OnMaxHealthChanged.Broadcast(Data.NewValue);
}


