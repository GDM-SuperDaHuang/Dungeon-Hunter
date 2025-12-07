// daHuang


#include "UI/WidgetController/SpellMenuWidgetController.h"

#include "AuraGameplayTags.h"
#include "AbilitySystem/AureAbilitySystemComponent.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "Palyer/AurePlayerState.h"

void USpellMenuWidgetController::BroadcastInitialValues()
{
	BroadcastAbilityInfo();
	SpellPointsChanged.Broadcast(GetAuraPS()->GetSpellPoints());
}

void USpellMenuWidgetController::BindCallbacksToDependencies()
{
	GetAuraASC()->AbilityStatusChangedDelegate.AddLambda(
		[this](const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag, int32 NewLevel)
		{
			if (SelectedAbility.Ability.MatchesTag(AbilityTag))
			{
				SelectedAbility.Status = StatusTag;

				bool bEnableSpellPoints = false;
				bool bEnableEquip = false;
				ShowEnableButtons(StatusTag, CurrentSpellPoints, bEnableSpellPoints, bEnableEquip);
				FString Descriptions;
				FString NextLevelDescriptions;
				GetAuraASC()->GetDescriptionsByAbilityTag(AbilityTag, Descriptions, NextLevelDescriptions);
				SpellGlobeSelectedDelegate.Broadcast(bEnableSpellPoints,
				                                     bEnableEquip,
				                                     Descriptions,
				                                     NextLevelDescriptions);
			}

			if (AbilityInfo)
			{
				FAuraAbilityInfo Info = AbilityInfo->FindFAuraAbilityInfoForTag(AbilityTag);
				Info.StatusTag = StatusTag;
				AbilityInfoDelegate.Broadcast(Info);
			}
		});

	GetAuraPS()->OnSpellPointsChangedDelegate.AddLambda([this](int32 SpellPoints)
	{
		SpellPointsChanged.Broadcast(SpellPoints);
		CurrentSpellPoints = SpellPoints;

		bool bEnableSpellPoints = false;
		bool bEnableEquip = false;
		ShowEnableButtons(SelectedAbility.Status, CurrentSpellPoints, bEnableSpellPoints, bEnableEquip);

		FString Descriptions;
		FString NextLevelDescriptions;
		GetAuraASC()->GetDescriptionsByAbilityTag(SelectedAbility.Ability, Descriptions, NextLevelDescriptions);

		SpellGlobeSelectedDelegate.Broadcast(bEnableSpellPoints, bEnableEquip, Descriptions, NextLevelDescriptions);
	});
}

void USpellMenuWidgetController::SpellGlobeSelected(const FGameplayTag& AbilityTag)
{
	FAuraGameplayTags GameplayTags = FAuraGameplayTags::Get();
	const int32 SpellPoints = GetAuraPS()->GetSpellPoints();
	FGameplayTag AbilityStatus;

	bool bTagValid = AbilityTag.IsValid();
	bool bTagNone = AbilityTag.MatchesTag(FAuraGameplayTags::Get().Abilities_None);

	FGameplayAbilitySpec* AbilitySpec = GetAuraASC()->GetSpecFromAbilityTag(AbilityTag);
	bool bSpecValid = AbilitySpec != nullptr;

	if (!bTagValid || !bTagNone || !bSpecValid)
	{
		AbilityStatus = GameplayTags.Abilities_Status_Locked;
	}
	else
	{
		AbilityStatus = GetAuraASC()->GetStatusFromSpec(*AbilitySpec);
	}

	SelectedAbility.Ability = AbilityTag;
	SelectedAbility.Status = AbilityStatus;
	bool bEnableSpellPoints = false;
	bool bEnableEquip = false;
	ShowEnableButtons(AbilityStatus, SpellPoints, bEnableSpellPoints, bEnableEquip);
	FString Descriptions;
	FString NextLevelDescriptions;
	GetAuraASC()->GetDescriptionsByAbilityTag(AbilityTag, Descriptions, NextLevelDescriptions);
	SpellGlobeSelectedDelegate.Broadcast(bEnableSpellPoints, bEnableEquip, Descriptions, NextLevelDescriptions);
}

void USpellMenuWidgetController::SendPointButtonPressed()
{
	if (GetAuraASC())
	{
		GetAuraASC()->ServerSpendSpellPoint(SelectedAbility.Ability);
	}
}

void USpellMenuWidgetController::GlobeDeselect()
{
	SelectedAbility.Ability = FAuraGameplayTags::Get().Abilities_None;
	SelectedAbility.Status = FAuraGameplayTags::Get().Abilities_Status_Locked;
	SpellGlobeSelectedDelegate.Broadcast(false, false, FString(), FString());
}

void USpellMenuWidgetController::ShowEnableButtons(const FGameplayTag& AbilityStatus, int32 SpellPoints,
                                                   bool& bShouldEnableSpellPointsButton, bool& bShouldEnableEquipButton)
{
	const FAuraGameplayTags GameplayTags = FAuraGameplayTags::Get();
	bShouldEnableSpellPointsButton = false;
	bShouldEnableEquipButton = false;
	if (AbilityStatus.MatchesTag(FAuraGameplayTags::Get().Abilities_Status_Equipped))
	{
		bShouldEnableEquipButton = true;
		if (SpellPoints > 0) bShouldEnableSpellPointsButton = true;
	}
	else if (AbilityStatus.MatchesTag(FAuraGameplayTags::Get().Abilities_Status_Eligible))
	{
		if (SpellPoints > 0) bShouldEnableSpellPointsButton = true;
	}
	else if (AbilityStatus.MatchesTag(FAuraGameplayTags::Get().Abilities_Status_Unlocked))
	{
		bShouldEnableEquipButton = true;
		if (SpellPoints > 0) bShouldEnableSpellPointsButton = true;
	}
}
