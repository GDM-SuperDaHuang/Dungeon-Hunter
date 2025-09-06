// daHuang
#include "UI/WidgetController/OverlayWidgetController.h"
#include "AbilitySystem/AureAbilitySystemComponent.h"
#include "AbilitySystem/AureAttributeSet.h"

void UOverlayWidgetController::BroadcastIntialValues()
{
	UAureAttributeSet* AuraAttributeSet = CastChecked<UAureAttributeSet>(AttributeSet);

	//广播
	OnHealthChanged.Broadcast(AuraAttributeSet->GetHealth());
	OnMaxHealthChanged.Broadcast(AuraAttributeSet->GetMaxHealth());
	OnManaChanged.Broadcast(AuraAttributeSet->GetMana());
	OnMaxManaChanged.Broadcast(AuraAttributeSet->GetMaxMana());
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
	/***
	 * 只有当属性通过UAbilitySystemComponent提供的官方接口修改时，才会发生回调，其他方式不会回调，
	 * 如：
	 * 1，AbilitySystemComponent->SetNumericAttributeBase(UAureAttributeSet::GetHealthAttribute(), NewHealthValue);
	 * 2，
	 */
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetHealthAttribute()).AddUObject(
		this, &UOverlayWidgetController::HealthChanged);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetMaxHealthAttribute()).
	                        AddUObject(this, &UOverlayWidgetController::MaxHealthChanged);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetManaAttribute()).AddUObject(
		this, &UOverlayWidgetController::ManaChanged);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetMaxManaAttribute()).AddUObject(
		this, &UOverlayWidgetController::MaxManaChanged);

	Cast<UAureAbilitySystemComponent>(AbilitySystemComponent)->EffectAssetTags.AddLambda(
		[this](const FGameplayTagContainer& AssetTags)
		{
			for (const FGameplayTag& Tag : AssetTags)
			{
				const FString Msg = FString::Printf(TEXT("tag:%s"), *Tag.ToString());
				GEngine->AddOnScreenDebugMessage(-1, 8.f, FColor::Blue, Msg);
				FUIWidgetRow* Row = GetDataTableRowByTag<FUIWidgetRow>(MessageWidgetDataTable,Tag);
				
			}
		});
}

void UOverlayWidgetController::HealthChanged(const FOnAttributeChangeData& Data) const
{
	OnHealthChanged.Broadcast(Data.NewValue);
}

void UOverlayWidgetController::MaxHealthChanged(const FOnAttributeChangeData& Data) const
{
	OnMaxHealthChanged.Broadcast(Data.NewValue);
}

void UOverlayWidgetController::ManaChanged(const FOnAttributeChangeData& Data) const
{
	OnManaChanged.Broadcast(Data.NewValue);
}

void UOverlayWidgetController::MaxManaChanged(const FOnAttributeChangeData& Data) const
{
	OnMaxManaChanged.Broadcast(Data.NewValue);
}
