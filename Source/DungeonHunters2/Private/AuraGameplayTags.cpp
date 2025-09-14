// daHuang


#include "AuraGameplayTags.h"
#include "GameplayTagsManager.h"
FAuraGameplayTags FAuraGameplayTags::GameplayTags;

void FAuraGameplayTags::InitializeNativeGameplayTags()
{
	/**
	 * Primary
	 */
	GameplayTags.Attributes_Primary_Strength = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName(TEXT("Attributes.Primary.Strength")),
		FString("Strength"));
	GameplayTags.Attributes_Primary_Intelligence = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName(TEXT("Attributes.Primary.Intelligence")),
		FString("Intelligence"));
	GameplayTags.Attributes_Primary_Resilience = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName(TEXT("Attributes.Primary.Resilience")),
		FString("Resilience"));
	GameplayTags.Attributes_Primary_Vigor = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName(TEXT("Attributes.Primary.Vigor")),
		FString("Vigor"));

	/**
	 * Secondary
	 */
	GameplayTags.Attributes_Secondary_Armor = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName(TEXT("Attributes.Secondary.Armor")),
		FString("Armor"));
	GameplayTags.Attributes_Secondary_ArmorPenetration = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName(TEXT("Attributes.Secondary.ArmorPenetration")),
		FString("ArmorPenetration"));
	GameplayTags.Attributes_Secondary_BlockChance = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName(TEXT("Attributes.Secondary.BlockChance")),
		FString("BlockChance"));
	GameplayTags.Attributes_Secondary_CriticalHitChance = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName(TEXT("Attributes.Secondary.CriticalHitChance")),
		FString("CriticalHitChance"));
	GameplayTags.Attributes_Secondary_CriticalHitDamage = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName(TEXT("Attributes.Secondary.CriticalHitDamage")),
		FString("CriticalHitDamage"));
	GameplayTags.Attributes_Secondary_CriticalHitResistance = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName(TEXT("Attributes.Secondary.CriticalHitResistance")),
		FString("CriticalHitResistance"));
	GameplayTags.Attributes_Secondary_HealthRegeneration = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName(TEXT("Attributes.Secondary.HealthRegeneration")),
		FString("HealthRegeneration"));
	GameplayTags.Attributes_Secondary_ManaRegeneration = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName(TEXT("Attributes.Secondary.ManaRegeneration")),
		FString("ManaRegeneration"));
	GameplayTags.Attributes_Secondary_MaxHealth = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName(TEXT("Attributes.Secondary.MaxHealth")),
		FString("MaxHealth"));
	GameplayTags.Attributes_Secondary_MaxMana = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName(TEXT("Attributes.Secondary.MaxMana")),
		FString("MaxMana"));
}
