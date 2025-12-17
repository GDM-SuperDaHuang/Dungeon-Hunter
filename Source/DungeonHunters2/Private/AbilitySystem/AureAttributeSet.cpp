// daHuang


#include "AbilitySystem/AureAttributeSet.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AuraAbilityTypes.h"
#include "GameplayEffectExtension.h"
#include "GameFramework/Character.h"
#include "Net/UnrealNetwork.h"
#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "DungeonHunters2/AuraLogChannels.h"
#include "Interaction/CombatInterface.h"
#include "Interaction/PlayerInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Palyer/AuraPlayerController.h"

UAureAttributeSet::UAureAttributeSet()
{
	//属性宏产生
	// InitHealth(10.f);
	// InitMaxHealth(150.f);
	// InitMana(10.f);
	// InitMaxMana(150.f);

	const FAuraGameplayTags GameplayTags = FAuraGameplayTags::Get();

	// FAttributeSignature StrengthDelegate;
	// StrengthDelegate.BindStatic(GetStrengthAttribute);
	// TagsToAttributes.Add(GameplayTags.Attributes_Primary_Strength, StrengthDelegate);
	//
	// FAttributeSignature IntelligenceDelegate;
	// IntelligenceDelegate.BindStatic(GetIntelligenceAttribute);
	// TagsToAttributes.Add(GameplayTags.Attributes_Primary_Intelligence, IntelligenceDelegate);

	/* primary */
	TagsToAttributes.Add(GameplayTags.Attributes_Primary_Strength, GetStrengthAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Primary_Intelligence, GetIntelligenceAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Primary_Resilience, GetResilienceAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Primary_Vigor, GetVigorAttribute);

	/* secondary */
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_Armor, GetArmorAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_ArmorPenetration, GetArmorPenetrationAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_BlockChance, GetBlockChanceAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_CriticalHitChance, GetCriticalHitChanceAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_CriticalHitDamage, GetCriticalHitDamageAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_CriticalHitResistance, GetCriticalHitResistanceAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_HealthRegeneration, GetHealthRegenerationAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_ManaRegeneration, GetManaRegenerationAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_MaxHealth, GetMaxHealthAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Secondary_MaxMana, GetMaxManaAttribute);

	/*Resistance Attributes */

	TagsToAttributes.Add(GameplayTags.Attributes_Resilience_Fire, GetFireResistanceAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Resilience_Lightning, GetLightningResistanceAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Resilience_Arcane, GetArcaneResistanceAttribute);
	TagsToAttributes.Add(GameplayTags.Attributes_Resilience_Physical, GetPhysicalResistanceAttribute);
}

void UAureAttributeSet::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	/**
	 * DOREPLIFETIME_CONDITION_NOTIFY 参数,GAS专用宏
	 * 1.当前类名
	 * 2.属性名
	 * 3.复制条件（COND_None/COND_OwnerOnly/COND_SkipOwner 等）
	 * 
	 * 4.通知策略（REPNOTIFY_Always/REPNOTIFY_OnChanged）
	 * 		– REPNOTIFY_OnChanged（默认值改变才触发）
	 * 		– REPNOTIFY_Always（只要收到包就触发，用于预测回滚）
	 */
	//primary Attributes
	DOREPLIFETIME_CONDITION_NOTIFY(UAureAttributeSet, Strength, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAureAttributeSet, Intelligence, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAureAttributeSet, Resilience, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAureAttributeSet, Vigor, COND_None, REPNOTIFY_Always);

	//secondary Attributes
	DOREPLIFETIME_CONDITION_NOTIFY(UAureAttributeSet, Armor, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAureAttributeSet, ArmorPenetration, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAureAttributeSet, BlockChance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAureAttributeSet, CriticalHitChance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAureAttributeSet, CriticalHitDamage, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAureAttributeSet, CriticalHitResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAureAttributeSet, HealthRegeneration, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAureAttributeSet, ManaRegeneration, COND_None, REPNOTIFY_Always);


	/*Resistance Attributes */
	DOREPLIFETIME_CONDITION_NOTIFY(UAureAttributeSet, FireResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAureAttributeSet, LightningResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAureAttributeSet, ArcaneResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAureAttributeSet, PhysicalResistance, COND_None, REPNOTIFY_Always);


	//vital Attributes
	DOREPLIFETIME_CONDITION_NOTIFY(UAureAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAureAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAureAttributeSet, Mana, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAureAttributeSet, MaxMana, COND_None, REPNOTIFY_Always);
}

/* ---------- 属性即将被修改 ---------- */
//执行时机是属性值即将被修改但尚未实际更新到 FGameplayAttributeData 之前
void UAureAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);
	if (Attribute == GetHealthAttribute())
	{
		// 限制Health在0到MaxHealth之间
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxHealth());
		// UE_LOG(LogTemp, Warning, TEXT("Health=%f"), NewValue);
	}
	if (Attribute == GetMaxHealthAttribute())
	{
		// UE_LOG(LogTemp, Warning, TEXT("MaxHealth=%f"), NewValue);
	}
	if (Attribute == GetManaAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxMana());
		// UE_LOG(LogTemp, Warning, TEXT("Mana=%f"), NewValue);
	}
	if (Attribute == GetMaxManaAttribute())
	{
		// UE_LOG(LogTemp, Warning, TEXT("MaxMana=%f"), NewValue);
	}
}


/* ---------- 工具：提取上下文 ---------- */
void UAureAttributeSet::SetEffectProperties(const struct FGameplayEffectModCallbackData& Data,
                                            FEffectProperties& props) const
{
	// 1. 获取效果上下文
	props.EffectContextHandle = Data.EffectSpec.GetContext();
	// 2. 获取源能力系统组件（施加效果的一方）
	props.SourceASC = props.EffectContextHandle.GetOriginalInstigatorAbilitySystemComponent();

	// 3. 获取源相关信息
	if (IsValid(props.SourceASC) && props.SourceASC->AbilityActorInfo.IsValid() && props.SourceASC->AbilityActorInfo->
		AvatarActor.IsValid())
	{
		// 源Avatar角色（通常是角色实例）
		props.SourceAvatarActor = props.SourceASC->AbilityActorInfo->AvatarActor.Get();
		// 源控制器
		props.SourceController = props.SourceASC->AbilityActorInfo->PlayerController.Get();
		// 如果直接获取控制器失败，尝试通过Pawn获取
		if (props.SourceController == nullptr && props.SourceAvatarActor != nullptr)
		{
			if (const APawn* Pawn = Cast<APawn>(props.SourceAvatarActor))
			{
				props.SourceController = Pawn->GetController();
			}
		}
		// 源角色
		if (props.SourceController != nullptr)
		{
			props.SourceCharacter = Cast<ACharacter>(props.SourceController->GetPawn());
		}
	}

	// 4. 获取目标相关信息（接收效果的一方）
	if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
	{
		// 目标Avatar角色
		props.TargetAvatarActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
		// 目标控制器
		props.TargetController = Data.Target.AbilityActorInfo->PlayerController.Get();
		// 目标角色
		props.TargetCharacter = Cast<ACharacter>(props.TargetAvatarActor);
		// 目标能力系统组件
		props.TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(props.TargetAvatarActor);
	}
}

void UAureAttributeSet::ShowFloatingText(const FEffectProperties& Props, float Damage, bool bBlockedHit,
                                         bool bCriticalHit) const
{
	if (Props.SourceCharacter != Props.TargetCharacter)
	{
		AAuraPlayerController* PC = Cast<AAuraPlayerController>(Props.SourceCharacter->Controller);
		if (PC)
		{
			PC->ShowDamageNumber(Damage, Props.TargetCharacter, bBlockedHit, bCriticalHit);
			return;
		}

		if (AAuraPlayerController* TargetPC = Cast<AAuraPlayerController>(Props.TargetCharacter->Controller))
		{
			TargetPC->ShowDamageNumber(Damage, Props.TargetCharacter, bBlockedHit, bCriticalHit);
		}
	}
}

void UAureAttributeSet::SendXPEvent(const FEffectProperties& Props)
{
	if (Props.TargetCharacter->Implements<UCombatInterface>())
	{
		int32 TargetLevel = ICombatInterface::Execute_GetPlayerLevel(Props.TargetCharacter);
		ECharacterClass TargetClass = ICombatInterface::Execute_GetCharacterClass(Props.TargetCharacter);
		int32 XPReward = UAuraAbilitySystemLibrary::GetXPRewardForClassAndLevel(
			Props.SourceCharacter, TargetClass, TargetLevel);

		FAuraGameplayTags GameplayTags = FAuraGameplayTags::Get();
		FGameplayEventData Payload;
		Payload.EventTag = GameplayTags.Attributes_Meta_IncomingXp;
		Payload.EventMagnitude = XPReward;


		// 这里执行会触发 OnGameplayEvent(FGameplayTag EventTag, FGameplayEventData Payload) 方法的回调
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Props.SourceCharacter,
		                                                         GameplayTags.Attributes_Meta_IncomingXp, Payload);
	}
}

/* ---------- GE 执行完毕，本次 GE 对属性的数值修改已经“落袋”的回调 ---------- */
void UAureAttributeSet::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
	// 创建效果属性结构并填充数据
	FEffectProperties Props;
	SetEffectProperties(Data, Props); // 解析效果的源和目标信息

	if (Props.TargetCharacter->Implements<UCombatInterface>() &&
		ICombatInterface::Execute_IsDead(Props.TargetCharacter))
	{
		return;
	}

	// 生命值处理：确保在效果应用后仍然在合法范围内
	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		// 限制Health在0到MaxHealth之间
		SetHealth(FMath::Clamp(GetHealth(), 0.0f, GetMaxHealth()));
		UE_LOG(LogTemp, Warning, TEXT("Health Changed on %s ,Health: %f"), *Props.TargetAvatarActor->GetName(),
		       GetHealth());
	}

	if (Data.EvaluatedData.Attribute == GetManaAttribute())
	{
		SetMana(FMath::Clamp(GetMana(), 0.0f, GetMaxMana()));
	}

	if (Data.EvaluatedData.Attribute == GetIncomingDamageAttribute())
	{
		HandleIncomingDamage(Props);
	}

	if (Data.EvaluatedData.Attribute == GetIncomingXPAttribute())
	{
		HandleIncomingXP(Props);
	}

	// if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	// {
	// 	UE_LOG(LogTemp, Warning, TEXT("MaxHealth=%f"), GetMaxHealth());
	// 	UE_LOG(LogTemp, Warning, TEXT("Health=%f"), GetHealth());
	// 	UE_LOG(LogTemp, Warning, TEXT("Magnitude=%f"), Data.EvaluatedData.Magnitude);
	// }
	// if (Data.EvaluatedData.Attribute == GetManaAttribute())
	// {
	// 	UE_LOG(LogTemp, Warning, TEXT("MaxMana=%f"), GetMaxMana());
	// 	UE_LOG(LogTemp, Warning, TEXT("Mana=%f"), GetMana());
	// 	UE_LOG(LogTemp, Warning, TEXT("Magnitude=%f"), Data.EvaluatedData.Magnitude);
	// }
}

void UAureAttributeSet::HandleIncomingDamage(const FEffectProperties& Props)
{
	const float LocalIncomingDamage = GetIncomingDamage();
	SetIncomingDamage(0);
	if (LocalIncomingDamage > 0)
	{
		const float NewHealth = GetHealth() - LocalIncomingDamage;
		SetHealth(FMath::Clamp(NewHealth, 0.0f, GetMaxHealth()));
		const float bFatal = NewHealth <= 0.f;
		if (bFatal)
		{
			
			ICombatInterface* CombatInterface = Cast<ICombatInterface>(Props.TargetAvatarActor);
			if (CombatInterface)
			{
				CombatInterface->Die(UAuraAbilitySystemLibrary::GetDeathImpulse(Props.EffectContextHandle));
			}
			SendXPEvent(Props);
		}
		else
		{
			FGameplayTagContainer TagContainer;
			//受击反应
			TagContainer.AddTag(FAuraGameplayTags::Get().Effects_HitReact);
			Props.TargetASC->TryActivateAbilitiesByTag(TagContainer);

			const FVector& KnockbackForce = UAuraAbilitySystemLibrary::GetKnockbackForce(Props.EffectContextHandle);
			if (!KnockbackForce.IsNearlyZero(1.f))
			{
				Props.TargetCharacter->LaunchCharacter(KnockbackForce,true,true);
			}
		}

		bool bBlocked = UAuraAbilitySystemLibrary::IsBlockedHit(Props.EffectContextHandle);
		bool bCritical = UAuraAbilitySystemLibrary::IsCriticalHit(Props.EffectContextHandle);

		ShowFloatingText(Props, LocalIncomingDamage, bBlocked, bCritical);
		if (UAuraAbilitySystemLibrary::IsSuccessfulDebuff(Props.EffectContextHandle))
		{
			//处理debuff
			Debuff(Props);
		}
	}
}

void UAureAttributeSet::Debuff(const FEffectProperties& Props)
{
	FAuraGameplayTags GameplayTags = FAuraGameplayTags::Get();
	FGameplayEffectContextHandle EffectContext = Props.SourceASC->MakeEffectContext();
	EffectContext.AddSourceObject(Props.SourceAvatarActor);

	const FGameplayTag DamageType = UAuraAbilitySystemLibrary::GetDamageType(Props.EffectContextHandle);

	// DebuffDamage = -1?
	const float DebuffDamage = UAuraAbilitySystemLibrary::GetDebuffDamage(Props.EffectContextHandle);
	const float DebuffDuration = UAuraAbilitySystemLibrary::GetDebuffDuration(Props.EffectContextHandle);
	const float DebuffFrequency = UAuraAbilitySystemLibrary::GetDebuffFrequency(Props.EffectContextHandle);


	FString DebuffName = FString::Printf(TEXT("DynamicDebuff_%s"), *DamageType.ToString());
	UGameplayEffect* Effect = NewObject<UGameplayEffect>(GetTransientPackage(), FName(DebuffName));

	Effect->DurationPolicy = EGameplayEffectDurationType::HasDuration;
	Effect->Period = DebuffFrequency;
	Effect->DurationMagnitude = FScalableFloat(DebuffDuration);

	// 触发回调ASC的RegisterGameplayTagEvent回调DebuffTagChanged。
	Effect->InheritableOwnedTagsContainer.AddTag(GameplayTags.DamageTypesToDebuffs[DamageType]);

	Effect->StackingType = EGameplayEffectStackingType::AggregateBySource;
	Effect->StackLimitCount = 1;

	int32 Index = Effect->Modifiers.Num();
	Effect->Modifiers.Add(FGameplayModifierInfo());
	FGameplayModifierInfo& ModifierInfo = Effect->Modifiers[Index];

	ModifierInfo.ModifierMagnitude = FScalableFloat(DebuffDamage);
	ModifierInfo.ModifierOp = EGameplayModOp::Additive;
	ModifierInfo.Attribute = UAureAttributeSet::GetIncomingDamageAttribute();
	if (FGameplayEffectSpec* MutableSpec = new FGameplayEffectSpec(Effect, EffectContext, 1.f))
	{
		FAuraGameplayEffectContext* AuraContext = static_cast<FAuraGameplayEffectContext*>(MutableSpec->GetContext().
			Get());
		TSharedPtr<FGameplayTag> DebuffDamageType = MakeShareable(new FGameplayTag(DamageType));
		AuraContext->SetDamageType(DebuffDamageType);

		Props.TargetASC->ApplyGameplayEffectSpecToSelf(*MutableSpec);
	}
}

void UAureAttributeSet::HandleIncomingXP(const FEffectProperties& Props)
{
	const float LocalIncomingXP = GetIncomingXP();
	SetIncomingXP(0);
	UE_LOG(LogAura, Log, TEXT("XP=%f"), GetIncomingXP());
	if (Props.SourceCharacter->Implements<UPlayerInterface>() &&
		Props.SourceCharacter->Implements<UCombatInterface>())
	{
		const int32 CurLevel = ICombatInterface::Execute_GetPlayerLevel(Props.SourceCharacter);
		const int32 CurXP = IPlayerInterface::Execute_GetXP(Props.SourceCharacter);
		const int32 NewLevel = IPlayerInterface::Execute_FindLevelForXP(Props.SourceCharacter, CurXP + LocalIncomingXP);
		const int32 NumLevelUps = NewLevel - CurLevel;
		if (NumLevelUps > 0)
		{
			//add level, add spell point ,full health and Mana

			const int32 AttributePointsReward = IPlayerInterface::Execute_GetAttributePointsReward(
				Props.SourceCharacter, CurLevel);
			const int32 SpellPointsReward = IPlayerInterface::Execute_GetSpellPointsReward(
				Props.SourceCharacter, CurLevel);
			IPlayerInterface::Execute_AddToPlayerLevel(Props.SourceCharacter, NumLevelUps);
			IPlayerInterface::Execute_AddToAttributePoints(Props.SourceCharacter, AttributePointsReward);
			IPlayerInterface::Execute_AddToSpellPoints(Props.SourceCharacter, SpellPointsReward);
			bToOffHealth = true;
			bToOffMana = true;
			IPlayerInterface::Execute_LevelUp(Props.SourceCharacter);
		}

		IPlayerInterface::Execute_AddToXP(Props.SourceCharacter, LocalIncomingXP);
	}
}


void UAureAttributeSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);
	if (Attribute == GetHealthAttribute() && bToOffHealth)
	{
		SetHealth(GetMaxHealth());
		bToOffHealth = false;
	}
	if (Attribute == GetManaAttribute() && bToOffMana)
	{
		SetMana(GetMaxMana());
		bToOffMana = false;
	}
}

/******************************************
 * 属性复制回调函数
 * 当属性通过网络复制时调用
 * 主要用于处理客户端接收到新属性值后的逻辑
 * GAMEPLAYATTRIBUTE_REPNOTIFY 为专供 FGameplayAttributeData 的 OnRep 实现宏；
 * GAS 专用” 的 OnRep 实现，负责 值同步 + 预测回滚
 ******************************************/

void UAureAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth) const
{
	/**
	 * 把 OldHealth 登记到 ASC 的 PredictionRollback 表
	 * 把 当前值 更新为 网络新值（ASC 内部缓存）
	 */
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAureAttributeSet, Health, OldHealth);
}

void UAureAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAureAttributeSet, MaxHealth, OldMaxHealth);
}

void UAureAttributeSet::OnRep_Mana(const FGameplayAttributeData& OldMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAureAttributeSet, Mana, OldMana);
}

void UAureAttributeSet::OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAureAttributeSet, MaxMana, OldMaxMana);
}

void UAureAttributeSet::OnRep_Strength(const FGameplayAttributeData& OldStrength) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAureAttributeSet, Strength, OldStrength);
}

void UAureAttributeSet::OnRep_Intelligence(const FGameplayAttributeData& OldIntelligence) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAureAttributeSet, Intelligence, OldIntelligence);
}

void UAureAttributeSet::OnRep_Resilience(const FGameplayAttributeData& OldResilience) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAureAttributeSet, Resilience, OldResilience);
}

void UAureAttributeSet::OnRep_Vigor(const FGameplayAttributeData& OldVigor) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAureAttributeSet, Vigor, OldVigor);
}

void UAureAttributeSet::OnRep_Armor(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAureAttributeSet, Armor, OldValue);
}

void UAureAttributeSet::OnRep_ArmorPenetration(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAureAttributeSet, ArmorPenetration, OldValue);
}

void UAureAttributeSet::OnRep_BlockChance(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAureAttributeSet, BlockChance, OldValue);
}

void UAureAttributeSet::OnRep_CriticalHitChance(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAureAttributeSet, CriticalHitChance, OldValue);
}

void UAureAttributeSet::OnRep_CriticalHitDamage(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAureAttributeSet, CriticalHitDamage, OldValue);
}

void UAureAttributeSet::OnRep_CriticalHitResistance(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAureAttributeSet, CriticalHitResistance, OldValue);
}

void UAureAttributeSet::OnRep_HealthRegeneration(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAureAttributeSet, HealthRegeneration, OldValue);
}

void UAureAttributeSet::OnRep_ManaRegeneration(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAureAttributeSet, ManaRegeneration, OldValue);
}

void UAureAttributeSet::OnRep_FireResistance(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAureAttributeSet, FireResistance, OldValue);
}

void UAureAttributeSet::OnRep_LightningResistance(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAureAttributeSet, LightningResistance, OldValue);
}

void UAureAttributeSet::OnRep_ArcaneResistance(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAureAttributeSet, ArcaneResistance, OldValue);
}

void UAureAttributeSet::OnRep_PhysicalResistance(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAureAttributeSet, PhysicalResistance, OldValue);
}
