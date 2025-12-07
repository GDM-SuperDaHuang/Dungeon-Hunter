// daHuang
#include "UI/WidgetController/OverlayWidgetController.h"
#include "AbilitySystem/AureAbilitySystemComponent.h"
#include "AbilitySystem/AureAttributeSet.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "AbilitySystem/Data/LevelUpInfo.h"
#include "Palyer/AurePlayerState.h"

void UOverlayWidgetController::BroadcastInitialValues()
{
	//广播
	OnHealthChanged.Broadcast(GetAuraAS()->GetHealth());
	OnMaxHealthChanged.Broadcast(GetAuraAS()->GetMaxHealth());
	OnManaChanged.Broadcast(GetAuraAS()->GetMana());
	OnMaxManaChanged.Broadcast(GetAuraAS()->GetMaxMana());
}

/**
 * 绑定属性变化回调
 * 当属性值发生变化时，会调用对应的回调函数
 * 回调函数会广播属性值的变化，使 UI 能够及时更新
 */
void UOverlayWidgetController::BindCallbacksToDependencies()
{
	// Super::BindCallbacksToDependencies();
	// AAurePlayerState* AurePlayerState = CastChecked<AAurePlayerState>(PlayerState);
	GetAuraPS()->OnXpChangedDelegate.AddUObject(this, &UOverlayWidgetController::OnXPChanged);
	GetAuraPS()->OnLevelChangedDelegate.AddLambda([this](int32 NewLevel)
	{
		OnPlayerLevelChangedChangedDelegate.Broadcast(NewLevel);
	});

	// const UAureAttributeSet* AuraAttributeSet = CastChecked<UAureAttributeSet>(AttributeSet);

	//绑定属性变化回调 FGameplayAttribute& XxxAttribute
	//FGameplayAttribute 是 GAS 中用于标识某个具体属性的 “句柄”，它本质上是对 UAttributeSet 中属性的封装（如 UAureAttributeSet 中的 Health 属性）。
	/***
	 * 只有当属性通过UAbilitySystemComponent提供的官方接口修改时，才会发生回调，其他方式不会回调，
	 * 如：
	 * 1，AbilitySystemComponent->SetNuwmericAttributeBase(UAureAttributeSet::GetHealthAttribute(), NewHealthValue);
	 * 2，
	 */
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GetAuraAS()->GetHealthAttribute())
	                      .AddLambda([this](const FOnAttributeChangeData& Data)
	                      {
		                      OnHealthChanged.Broadcast(Data.NewValue);
	                      });

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GetAuraAS()->GetMaxHealthAttribute())
	                      .AddLambda([this](const FOnAttributeChangeData& Data)
	                      {
		                      OnMaxHealthChanged.Broadcast(Data.NewValue);
	                      });

	// AddUObject(this, &UOverlayWidgetController::MaxHealthChanged);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GetAuraAS()->GetManaAttribute())
	                      .AddLambda([this](const FOnAttributeChangeData& Data)
	                      {
		                      OnManaChanged.Broadcast(Data.NewValue);
	                      });
	//this, &UOverlayWidgetController::ManaChanged);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GetAuraAS()->GetMaxManaAttribute())
	                      .AddLambda([this](const FOnAttributeChangeData& Data)
	                      {
		                      OnMaxManaChanged.Broadcast(Data.NewValue);
	                      });


	if (GetAuraASC())
	{
		if (GetAuraASC()->bStartUpAbilitiesGiven)
		{
			BroadcastAbilityInfo();
			// OnInitializeStartupProperties(GetAuraASC());
		}
		else
		{
			GetAuraASC()->AbilitiesGivenDelegate.AddUObject(this, &UOverlayWidgetController::BroadcastAbilityInfo);
		}

		GetAuraASC()->EffectAssetTags.AddLambda(
			[this](const FGameplayTagContainer& AssetTags)
			{
				for (const FGameplayTag& Tag : AssetTags)
				{
					FGameplayTag MessageTag = FGameplayTag::RequestGameplayTag(FName("Message"));
					if (Tag.MatchesTag(MessageTag))
					{
						FUIWidgetRow* Row = GetDataTableRowByTag<FUIWidgetRow>(MessageWidgetDataTable, Tag);
						MessageWidgetRowDelegate.Broadcast(*Row);
					}
					// const FString Msg = FString::Printf(TEXT("tag:%s"), *Tag.ToString());
					// GEngine->AddOnScreenDebugMessage(-1, 8.f, FColor::Blue, Msg);
				}
			});
	}


	// .AddUObject(this, &UOverlayWidgetController::MaxManaChanged);
}

// void UOverlayWidgetController::OnInitializeStartupProperties(UAureAbilitySystemComponent* InAureAbilitySystemComponent)
// {
// 	if (!GetAuraASC()->bStartUpAbilitiesGiven) return;
//
// 	FForEachAbility BroadcastDelegate;
// 	BroadcastDelegate.BindLambda([this](const FGameplayAbilitySpec& AbilitySpec)
// 	{
// 		FAuraAbilityInfo Info = AbilityInfo->FindFAuraAbilityInfoForTag(
// 			AureAbilitySystemComponent->GetAbilityTagFromSpec(AbilitySpec));
// 		Info.InputTag = AureAbilitySystemComponent->GetInputTagFromSpec(AbilitySpec);
// 		AbilityInfoDelegate.Broadcast(Info);
// 	});
// 	GetAuraASC()->FForEachAbility(BroadcastDelegate);
// }

void UOverlayWidgetController::OnXPChanged(int32 NewXP) 
{
	// const AAurePlayerState* AurePlayerState = CastChecked<AAurePlayerState>(PlayerState);
	const ULevelUpInfo* LevelUpInfo = GetAuraPS()->LevelUpInfo;

	checkf(LevelUpInfo, TEXT("LevelUpInfo is null"));

	int32 Level = LevelUpInfo->FindLevelForXP(NewXP);
	int32 MaxLevel = LevelUpInfo->LevelUpInformation.Num();

	if (Level <= MaxLevel && Level > 0)
	{
		const int32 LeveUpRequirement = LevelUpInfo->LevelUpInformation[Level].LeveUpRequirement;
		const int32 PreviousLeveUpRequirement = LevelUpInfo->LevelUpInformation[Level - 1].LeveUpRequirement;

		const int32 DeltaLeveUpRequirement = LeveUpRequirement - PreviousLeveUpRequirement;
		const int32 XPForThisLevel = NewXP - PreviousLeveUpRequirement;

		const float XPBarPercent = static_cast<float>(XPForThisLevel) / static_cast<float>(DeltaLeveUpRequirement);
		OnXPPercentManaChanged.Broadcast(XPBarPercent);
	}
}


// void UOverlayWidgetController::MaxHealthChanged(const FOnAttributeChangeData& Data) const
// {
// 	OnMaxHealthChanged.Broadcast(Data.NewValue);
// }
//
// void UOverlayWidgetController::ManaChanged(const FOnAttributeChangeData& Data) const
// {
// 	OnManaChanged.Broadcast(Data.NewValue);
// }

// void UOverlayWidgetController::MaxManaChanged(const FOnAttributeChangeData& Data) const
// {
// 	OnMaxManaChanged.Broadcast(Data.NewValue);
// }
