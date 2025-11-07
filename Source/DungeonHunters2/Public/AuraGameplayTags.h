// daHuang

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "AbilitySystem/AureAttributeSet.h"
/**
 * 
 */
struct FAuraGameplayTags
{
public:
	static const FAuraGameplayTags& Get() { return GameplayTags; }
	static void InitializeNativeGameplayTags();


	FGameplayTag Attributes_Primary_Strength;
	FGameplayTag Attributes_Primary_Intelligence;
	FGameplayTag Attributes_Primary_Resilience;
	FGameplayTag Attributes_Primary_Vigor;


	FGameplayTag Attributes_Secondary_Armor;
	FGameplayTag Attributes_Secondary_ArmorPenetration;
	FGameplayTag Attributes_Secondary_BlockChance;
	FGameplayTag Attributes_Secondary_CriticalHitChance;
	FGameplayTag Attributes_Secondary_CriticalHitDamage;
	FGameplayTag Attributes_Secondary_CriticalHitResistance;
	FGameplayTag Attributes_Secondary_HealthRegeneration;
	FGameplayTag Attributes_Secondary_ManaRegeneration;
	
	FGameplayTag Attributes_Secondary_MaxHealth;
	FGameplayTag Attributes_Secondary_MaxMana;


	//伤害抗性
	FGameplayTag Attributes_Resilience_Fire;
	FGameplayTag Attributes_Resilience_Lightning;
	FGameplayTag Attributes_Resilience_Arcane;
	FGameplayTag Attributes_Resilience_Physical;

	
	FGameplayTag InputTag_LMB;
	FGameplayTag InputTag_RMB;
	FGameplayTag InputTag_1;
	FGameplayTag InputTag_2;
	FGameplayTag InputTag_3;
	FGameplayTag InputTag_4;


	FGameplayTag Event_Montage_FireBolt;

	//伤害类型
	FGameplayTag Damage;
	FGameplayTag Damage_Fire;
	FGameplayTag Damage_Lightning;
	FGameplayTag Damage_Arcane;
	FGameplayTag Damage_Physical;

	TMap<FGameplayTag,FGameplayTag> DamageTypesToResilience;
	// TArray<FGameplayTag> DamageTypes;

	FGameplayTag Effects_HitReact;



protected:


private:
	static FAuraGameplayTags GameplayTags;
};


// /**
//  * 
//  */
// class DUNGEONHUNTERS2_API AuraGameplayTags
// {
// public:
// 	AuraGameplayTags();
// 	~AuraGameplayTags();
// };
