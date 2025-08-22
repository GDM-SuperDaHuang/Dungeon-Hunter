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

/**
 * 绑定属性变化回调
 * 当属性值发生变化时，会调用对应的回调函数
 * 回调函数会广播属性值的变化，使 UI 能够及时更新
 */
void UOverlayWidgetController::BindCallbacksToDependences()
{
	// Super::BindCallbacksToDependences();
	const UAureAttributeSet* AuraAttributeSet = CastChecked<UAureAttributeSet>(AttributeSet);
	//绑定属性变化回调 FGameplayAttribute& XxxAttribute
	//FGameplayAttribute 是 GAS 中用于标识某个具体属性的 “句柄”，它本质上是对 UAttributeSet 中属性的封装（如 UAureAttributeSet 中的 Health 属性）。
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


