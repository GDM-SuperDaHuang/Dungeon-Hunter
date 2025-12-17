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

	GameplayTags.InputTag_Passive_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName(TEXT("InputTag.Passive.1")),
		FString("InputTag Passive 1"));

	GameplayTags.InputTag_Passive_2 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName(TEXT("InputTag.Passive.2")),
		FString("InputTag Passive 2"));
		
	/***
	 * 蒙太奇事件通知标签
	 */		
	GameplayTags.Event_Montage_FireBolt = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName(TEXT("Event.Montage.FireBolt")),
		FString("FireBolt"));

	GameplayTags.Event_Montage_Electrocute = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName(TEXT("Event.Montage.Electrocute")),
		FString("Electrocute"));
	
	// GameplayTags.Event_Montage_Attack_Melee = UGameplayTagsManager::Get().AddNativeGameplayTag(
	// 	FName(TEXT("Event.Montage.Attack.Melee")),
	// 	FString("Attack Melee"));


	/**
	 *
	 */
	GameplayTags.CombatSocket_Weapon = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName(TEXT("Event.Montage.Attack.Weapon")),
		FString("Attack Weapon"));

	GameplayTags.CombatSocket_RightHand = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName(TEXT("Event.Montage.Attack.RightHand")),
		FString("Attack RightHand"));

	GameplayTags.CombatSocket_LeftHand = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName(TEXT("Event.Montage.Attack.LeftHand")),
		FString("Attack LeftHand"));

	GameplayTags.CombatSocket_Tail = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName(TEXT("Event.Montage.Attack.Tail")),
		FString("Attack Tail"));


	//montage tag
	GameplayTags.Montage_Attack_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName(TEXT("Montage.Attack.1")),
		FString("Montage Attack 1"));

	GameplayTags.Montage_Attack_2 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName(TEXT("Montage.Attack.2")),
		FString("Montage Attack 2"));

	GameplayTags.Montage_Attack_3 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName(TEXT("Montage.Attack.3")),
		FString("Montage Attack 3"));

	GameplayTags.Montage_Attack_4 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName(TEXT("Montage.Attack.4")),
		FString("Montage Attack 4"));


	// 输入
	GameplayTags.Play_Block_InputPressed = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName(TEXT("Play.Block.InputPressed")),
		FString("Play Block InputPressed"));
	
	GameplayTags.Play_Block_InputHeld = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName(TEXT("Play.Block.InputHeld")),
		FString("Play Block InputHeld"));

	GameplayTags.Play_Block_InputReleased = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName(TEXT("Play.Block.InputReleased")),
		FString("Play Block InputReleased"));

	GameplayTags.Play_Block_CursorTrace = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName(TEXT("Play.Block.CursorTrace")),
		FString("Play Block CursorTrace"));



	// 伤害类型
	GameplayTags.Damage = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName(TEXT("Damage")),
		FString("Damage"));

	GameplayTags.Damage_Fire = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName(TEXT("Damage.Fire")),
		FString("Damage Fire Type"));

	GameplayTags.Damage_Lightning = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName(TEXT("Damage.Lightning")),
		FString("Damage Lightning Type"));

	GameplayTags.Damage_Arcane = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName(TEXT("Damage.Arcane")),
		FString("Damage Arcane Type"));

	GameplayTags.Damage_Physical = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName(TEXT("Damage.Physical")),
		FString("Damage Physical Type"));


	// 伤害抗性
	GameplayTags.Attributes_Resilience_Fire = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName(TEXT("Damage.Resilience.Fire")),
		FString("Damage ResiliencePhysical Fire Type"));

	GameplayTags.Attributes_Resilience_Lightning = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName(TEXT("Damage.Resilience.Lightning")),
		FString("Damage ResiliencePhysical Lightning Type"));

	GameplayTags.Attributes_Resilience_Arcane = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName(TEXT("Damage.Resilience.Arcane")),
		FString("Damage ResiliencePhysical Arcane Type"));

	GameplayTags.Attributes_Resilience_Physical = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName(TEXT("Damage.Resilience.Physical")),
		FString("Damage ResiliencePhysical Physical Type"));

	/** debfffus */
	GameplayTags.Debuff_Burn = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName(TEXT("Debuff.Burn")),
		FString("Damage Debuff Fire Type"));

	GameplayTags.Debuff_Stun = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName(TEXT("Debuff.Stun")),
		FString("Damage Debuff Stun"));

	GameplayTags.Debuff_Arcane = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName(TEXT("Debuff.Arcane")),
		FString("Damage Debuff Arcane"));

	GameplayTags.Debuff_Physical = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName(TEXT("Debuff.Physical")),
		FString("Damage Debuff Physical Type"));

	
	GameplayTags.Debuff_Chance = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName(TEXT("Debuff.Chance")),
	FString("Damage Debuff Chance Type"));

	GameplayTags.Debuff_Damage = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName(TEXT("Debuff.Damage")),
	FString("Damage Debuff Damage Type"));

	GameplayTags.Debuff_Duration = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName(TEXT("Debuff.Duration")),
	FString("Damage Debuff Duration Type"));

	GameplayTags.Debuff_Frequency = UGameplayTagsManager::Get().AddNativeGameplayTag(
	FName(TEXT("Debuff.Frequency")),
	FString("Damage Debuff Frequency Type"));


	/** mata */
	GameplayTags.Attributes_Meta_IncomingXp = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName(TEXT("Attributes.Meta.IncomingXp")),
		FString("Attributes Meta IncomingXp Tag"));


	/**
	 * 抗性伤害map
	 */
	GameplayTags.DamageTypesToResilience.Add(GameplayTags.Damage_Fire, GameplayTags.Attributes_Resilience_Fire);
	GameplayTags.DamageTypesToResilience.Add(GameplayTags.Damage_Lightning,GameplayTags.Attributes_Resilience_Lightning);
	GameplayTags.DamageTypesToResilience.Add(GameplayTags.Damage_Arcane, GameplayTags.Attributes_Resilience_Arcane);
	GameplayTags.DamageTypesToResilience.Add(GameplayTags.Damage_Physical, GameplayTags.Attributes_Resilience_Physical);

	GameplayTags.DamageTypesToDebuffs.Add(GameplayTags.Damage_Fire, GameplayTags.Debuff_Burn);
	GameplayTags.DamageTypesToDebuffs.Add(GameplayTags.Damage_Lightning, GameplayTags.Debuff_Stun);
	GameplayTags.DamageTypesToDebuffs.Add(GameplayTags.Damage_Arcane, GameplayTags.Debuff_Arcane);
	GameplayTags.DamageTypesToDebuffs.Add(GameplayTags.Damage_Physical, GameplayTags.Debuff_Physical);

	

	/*技能 */
	GameplayTags.Abilities_None = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName(TEXT("Abilities.None")),
		FString("Abilities None Tag"));

	GameplayTags.Abilities_Attack = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName(TEXT("Abilities.Attack")),
		FString("Abilities Attack Tag"));


	GameplayTags.Abilities_Summon = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName(TEXT("Abilities.Summon")),
		FString("Abilities Summon Tag"));

	GameplayTags.Abilities_Fire_FireBolt = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName(TEXT("Abilities.Fire.FireBolt")),
		FString("Abilities Fire FireBolt Tag"));

	GameplayTags.Abilities_Lightning_Electrocute = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName(TEXT("Abilities.Lightning.Electrocute")),
		FString("Abilities Lightning Electrocute"));

	GameplayTags.Abilities_HitReact = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName(TEXT("Abilities.HitReact")),
		FString("Abilities HitReact"));


	/** 技能菜单状态 */
	GameplayTags.Abilities_Status_Locked = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName(TEXT("Abilities.Status.Locked")),
		FString("Abilities Status Locked"));

	GameplayTags.Abilities_Status_Eligible = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName(TEXT("Abilities.Status.Eligible")),
		FString("Abilities Status Eligible"));

	GameplayTags.Abilities_Status_Unlocked = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName(TEXT("Abilities.Status.Unlocked")),
		FString("Abilities Status Unlocked"));

	GameplayTags.Abilities_Status_Equipped = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName(TEXT("Abilities.Status.Equipped")),
		FString("Abilities Status Equipped"));


	GameplayTags.Abilities_Type_Offensive = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName(TEXT("Abilities.Status.Offensive")),
		FString("Abilities Status Offensive"));

	GameplayTags.Abilities_Type_Passive = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName(TEXT("Abilities.Status.Passive")),
		FString("Abilities Status Passive"));

	GameplayTags.Abilities_Type_None = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName(TEXT("Abilities.Status.None")),
		FString("Abilities Status None"));


	/**
	 * Cooldown
	 */
	GameplayTags.Cooldown_Fire_FireBolt = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName(TEXT("Cooldown.Fire.FireBolt")),
		FString("Cooldown Fire FireBolt Tag"));


	GameplayTags.Effects_HitReact = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName(TEXT("Effects.HitReact")),
		FString("Effects.HitReact"));

	// GameplayCue 类型标签
	GameplayTags.GameplayCue_MeleeImpact = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName(TEXT("GameplayCue.MeleeImpact")),
		FString("GameplayCue MeleeImpact"));
}
