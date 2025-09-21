// daHuang


#include "AuraGameplayTags.h"
#include "GameplayTagsManager.h"
FAuraGameplayTags FAuraGameplayTags::GameplayTags;

void FAuraGameplayTags::InitializeNativeGameplayTags()
{
	/**
	 * 方式一：使用cpp定义属性游戏标签
	 */
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

	/**
	 * input
	 */
	GameplayTags.InputTag_LMB = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName(TEXT("InputTag.LMB")),
	FString("input tag for left mouse button"));
	
	GameplayTags.InputTag_RMB = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName(TEXT("InputTag.RMB")),
	FString("input tag for right mouse button"));
	
	GameplayTags.InputTag_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName(TEXT("InputTag.1")),
	FString("input tag for 1"));
	
	GameplayTags.InputTag_2 = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName(TEXT("InputTag.2")),
	FString("input tag for 2"));
	
	GameplayTags.InputTag_3 = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName(TEXT("InputTag.3")),
	FString("input tag for 2"));
	
	GameplayTags.InputTag_4 = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName(TEXT("InputTag.4")),
	FString("input tag for 4"));
}
