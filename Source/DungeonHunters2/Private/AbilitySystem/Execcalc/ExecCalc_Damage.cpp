// daHuang


#include "AbilitySystem/ExecCalc/ExecCalc_Damage.h"

#include "AbilitySystemComponent.h"
#include "AuraAbilityTypes.h"
#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "AbilitySystem/AureAttributeSet.h"
#include "Interaction/CombatInterface.h"
#include "Kismet/GameplayStatics.h"

struct AuraDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Armor);
	DECLARE_ATTRIBUTE_CAPTUREDEF(ArmorPenetration);
	DECLARE_ATTRIBUTE_CAPTUREDEF(BlockChance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitChance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitDamage);

	DECLARE_ATTRIBUTE_CAPTUREDEF(FireResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(LightningResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(ArcaneResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(PhysicalResistance);


	// TMap<FGameplayTag, FGameplayEffectAttributeCaptureDefinition> TagsToCaptureDefs;

	AuraDamageStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAureAttributeSet, Armor, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAureAttributeSet, BlockChance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAureAttributeSet, ArmorPenetration, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAureAttributeSet, CriticalHitChance, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAureAttributeSet, CriticalHitResistance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAureAttributeSet, CriticalHitDamage, Source, false);

		DEFINE_ATTRIBUTE_CAPTUREDEF(UAureAttributeSet, FireResistance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAureAttributeSet, LightningResistance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAureAttributeSet, ArcaneResistance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAureAttributeSet, PhysicalResistance, Target, false);

		// FAuraGameplayTags Tags = FAuraGameplayTags::Get();
		//
		// TagsToCaptureDefs.Add(Tags.Attributes_Secondary_Armor, ArmorDef);
		// TagsToCaptureDefs.Add(Tags.Attributes_Secondary_ArmorPenetration, ArmorPenetrationDef);
		// TagsToCaptureDefs.Add(Tags.Attributes_Secondary_BlockChance, BlockChanceDef);
		// TagsToCaptureDefs.Add(Tags.Attributes_Secondary_CriticalHitChance, CriticalHitChanceDef);
		// TagsToCaptureDefs.Add(Tags.Attributes_Secondary_CriticalHitDamage, CriticalHitDamageDef);
		// TagsToCaptureDefs.Add(Tags.Attributes_Secondary_CriticalHitResistance, CriticalHitResistanceDef);
		//
		// TagsToCaptureDefs.Add(Tags.Attributes_Resilience_Fire, FireResistanceDef);
		// TagsToCaptureDefs.Add(Tags.Attributes_Resilience_Lightning, LightningResistanceDef);
		// TagsToCaptureDefs.Add(Tags.Attributes_Resilience_Arcane, ArcaneResistanceDef);
		// TagsToCaptureDefs.Add(Tags.Attributes_Resilience_Physical, PhysicalResistanceDef);
	}
};

static const AuraDamageStatics& DamageStatics()
{
	static AuraDamageStatics DStatices;
	return DStatices;
}

UExecCalc_Damage::UExecCalc_Damage()
{
	RelevantAttributesToCapture.Add(DamageStatics().ArmorDef);
	RelevantAttributesToCapture.Add(DamageStatics().ArmorPenetrationDef);
	RelevantAttributesToCapture.Add(DamageStatics().BlockChanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().CriticalHitChanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().CriticalHitResistanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().CriticalHitDamageDef);

	RelevantAttributesToCapture.Add(DamageStatics().FireResistanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().LightningResistanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().ArcaneResistanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().PhysicalResistanceDef);
}

void UExecCalc_Damage::DetermineDebuff(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
                                       FGameplayEffectSpec Spec, FAggregatorEvaluateParameters EvaluateParams,
                                       const TMap<FGameplayTag, FGameplayEffectAttributeCaptureDefinition> InTagsToDefs)
const
{
	FAuraGameplayTags GameplayTags = FAuraGameplayTags::Get();

	for (TPair<FGameplayTag, FGameplayTag> Pair : GameplayTags.DamageTypesToDebuffs)
	{
		const FGameplayTag DamageType = Pair.Key;// 如 Damage.Fire
		const FGameplayTag DebuffType = Pair.Value;// 如 Debuff.Burn
		const float TypeDamage = Spec.GetSetByCallerMagnitude(DamageType, false, -1.f);
		if (TypeDamage > -.5f)// 该伤害类型有效
		{
			// 有问题？
			const float SourceDebuffChance = Spec.GetSetByCallerMagnitude(GameplayTags.Debuff_Chance, false, -1.f);
			float TargetDebuffResistance = 0.f;
			// 从目标（被伤害者）身上捕获对应的抗性属性值
			FGameplayTag ResistanceTag = GameplayTags.DamageTypesToResilience[DamageType];
			// InTagsToDefs 在 ExecutionCalculation 的 CaptureAttributes 中定义
			ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(InTagsToDefs[ResistanceTag],EvaluateParams, TargetDebuffResistance);
			TargetDebuffResistance = FMath::Max<float>(TargetDebuffResistance, 0.f);
			const float EffectiveDebuffChance = SourceDebuffChance * (100 - TargetDebuffResistance) / 100.f;
			const bool bDebuff = FMath::RandRange(1, 100) < EffectiveDebuffChance;
			if (bDebuff)
			{
				 // 6. 【核心】只在Context中标记，不创建伤害！
				FGameplayEffectContextHandle ContextHandle = Spec.GetContext();
				UAuraAbilitySystemLibrary::SetIsSuccessfulDebuff(ContextHandle, true);
				UAuraAbilitySystemLibrary::SetDamageType(ContextHandle, DamageType);
				// 7. 将Debuff参数存入Context，供后续使用
				const float DebuffDamage = Spec.GetSetByCallerMagnitude(GameplayTags.Debuff_Damage, false, -1.f);
				const float DebuffDuration = Spec.GetSetByCallerMagnitude(GameplayTags.Debuff_Duration, false, -1.f);
				const float DebuffFrequency = Spec.GetSetByCallerMagnitude(GameplayTags.Debuff_Frequency, false, -1.f);
				UAuraAbilitySystemLibrary::SetDebuffDamage(ContextHandle, DebuffDamage);
				UAuraAbilitySystemLibrary::SetDebuffDuration(ContextHandle, DebuffDuration);
				UAuraAbilitySystemLibrary::SetDebuffFrequency(ContextHandle, DebuffFrequency);
			}
		}
	}
}

void UExecCalc_Damage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
                                              FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	TMap<FGameplayTag, FGameplayEffectAttributeCaptureDefinition> TagsToCaptureDefs;
	const FAuraGameplayTags Tags = FAuraGameplayTags::Get();
	// 创建一个Tag到属性捕获定义的映射表，用于后续快速查找抗性、护甲、暴击等属性
	TagsToCaptureDefs.Add(Tags.Attributes_Secondary_Armor, DamageStatics().ArmorDef);
	TagsToCaptureDefs.Add(Tags.Attributes_Secondary_ArmorPenetration, DamageStatics().ArmorPenetrationDef);
	TagsToCaptureDefs.Add(Tags.Attributes_Secondary_BlockChance, DamageStatics().BlockChanceDef);
	TagsToCaptureDefs.Add(Tags.Attributes_Secondary_CriticalHitChance, DamageStatics().CriticalHitChanceDef);
	TagsToCaptureDefs.Add(Tags.Attributes_Secondary_CriticalHitDamage, DamageStatics().CriticalHitDamageDef);
	TagsToCaptureDefs.Add(Tags.Attributes_Secondary_CriticalHitResistance, DamageStatics().CriticalHitResistanceDef);

	TagsToCaptureDefs.Add(Tags.Attributes_Resilience_Fire, DamageStatics().FireResistanceDef);
	TagsToCaptureDefs.Add(Tags.Attributes_Resilience_Lightning, DamageStatics().LightningResistanceDef);
	TagsToCaptureDefs.Add(Tags.Attributes_Resilience_Arcane, DamageStatics().ArcaneResistanceDef);
	TagsToCaptureDefs.Add(Tags.Attributes_Resilience_Physical, DamageStatics().PhysicalResistanceDef);


	// 获取来源和目标的ASC（AbilitySystemComponent）
	UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
	UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();
	AActor* SourceAvatar = SourceASC ? SourceASC->GetAvatarActor() : nullptr;
	AActor* TargetAvatar = TargetASC ? TargetASC->GetAvatarActor() : nullptr;


	// 获取双方角色等级（用于曲线表查询）
	int32 SourcePlayerLevel = 1;
	if (SourceAvatar->Implements<UCombatInterface>())
	{
		SourcePlayerLevel = ICombatInterface::Execute_GetPlayerLevel(SourceAvatar);
	}

	int32 TargetPlayerLevel = 1;
	if (TargetAvatar->Implements<UCombatInterface>())
	{
		TargetPlayerLevel = ICombatInterface::Execute_GetPlayerLevel(TargetAvatar);
	}


	// ICombatInterface* SourceCombatInterface = Cast<ICombatInterface>(SourceAvatar);
	// ICombatInterface* TargetCombatInterface = Cast<ICombatInterface>(TargetAvatar);

	// int32 PlayerLevel = SourceCombatInterface->GetPlayerLevel();


	FGameplayEffectSpec Spec = ExecutionParams.GetOwningSpec();
	FGameplayEffectContextHandle EffectContextHandle = Spec.GetContext();
	

	// 获取捕获的标签容器（来源和目标）
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();
	FAggregatorEvaluateParameters EvaluateParams;
	EvaluateParams.SourceTags = SourceTags;
	EvaluateParams.TargetTags = TargetTags;

	//debuff,处理燃烧、眩晕等异常状态的触发判定
	DetermineDebuff(ExecutionParams, Spec, EvaluateParams, TagsToCaptureDefs);


	//伤害
	float Damage = 0.f;
	// 遍历所有伤害类型与抗性的映射（如Damage.Fire → Attributes_Resilience_Fire）
	for (TPair<FGameplayTag, FGameplayTag> Pair : FAuraGameplayTags::Get().DamageTypesToResilience)
	{
		const FGameplayTag DamageTypeTag = Pair.Key;
		const FGameplayTag ResistanceTag = Pair.Value;
		// 安全检查：确保捕获定义映射中包含该抗性
		checkf(TagsToCaptureDefs.Contains(ResistanceTag),TEXT("not contains tag: [%s] in TagsToCaptureDefs"), *ResistanceTag.ToString());
		const FGameplayEffectAttributeCaptureDefinition CaptureDef = TagsToCaptureDefs[ResistanceTag];

		//【核心】从SetByCaller获取该类型伤害的原始值（在ApplyDamageEffect中设置）
		float DamageTypeValue = Spec.GetSetByCallerMagnitude(Pair.Key, false);
		if (DamageTypeValue <= 0.f)
		{
			continue;
		}
		
		// 捕获目标的抗性值（0-100）
		float Resistance = 0.f;
		ExecutionParams.AttemptCalculateCapturedAttributeBonusMagnitude(CaptureDef, EvaluateParams, Resistance);
		Resistance = FMath::Clamp(Resistance, 0.f, 100.f);
		// 计算抗性减免后的伤害
    	// 公式: 实际伤害 = 原始伤害 * (100 - 抗性) / 100
		DamageTypeValue *= (100.f * Resistance) / 100.f;
		
		// 【范围伤害特殊处理】如果是AoE伤害，使用UE内置的径向伤害系统
		if (UAuraAbilitySystemLibrary::IsRadialDamage(EffectContextHandle))
		{
			// 1. override TakeDamage in AuraCharacterBase. *
			// 2. create delegate OnDamageDelegate, broadcast damage received in TakeDamage *
			// 3. Bind lambda to OnDamageDelegate on the Victim here. *
			// 4. Call UGameplayStatics::ApplyRadialDamageWithFalloff to cause damage (this will result in TakeDamage being called
			//		on the Victim, which will then broadcast OnDamageDelegate)
			// 5. In Lambda, set DamageTypeValue to the damage received from the broadcast *
			// 绑定Lambda到目标的OnDamageDelegate，捕获实际伤害值
			if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(TargetAvatar))
			{
				CombatInterface->GetOnDamageSignature().AddLambda([&](float DamageAmount)
				{
					DamageTypeValue = DamageAmount;
				});
			}
			// 调用UE的径向伤害函数，内部会触发TakeDamage
			UGameplayStatics::ApplyRadialDamageWithFalloff(
				TargetAvatar,
				DamageTypeValue,// 基础伤害
				0.f,			// 最小伤害（中心）
				UAuraAbilitySystemLibrary::GetRadialDamageOrigin(EffectContextHandle),// 中心点
				UAuraAbilitySystemLibrary::GetRadialDamageInnerRadius(EffectContextHandle),// 内半径（100%伤害）
				UAuraAbilitySystemLibrary::GetRadialDamageOuterRadius(EffectContextHandle),// 外半径（0%伤害）
				1.f,// 衰减指数
				UDamageType::StaticClass(),
				TArray<AActor*>(), 
				SourceAvatar, // 伤害来源Actor
				nullptr);// 忽略InstigatorController
		}

		// 将该类型伤害加入总伤害
		Damage += DamageTypeValue;
	}

	// 捕获目标的格挡几率（0-100）
	float TargetBlockChance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().BlockChanceDef, EvaluateParams,
	                                                           TargetBlockChance);
	TargetBlockChance = FMath::Max<float>(0.f, TargetBlockChance);
	// 随机判定是否格挡（格挡成功则伤害减半）
	const bool bBlocked = FMath::RandRange(1, 100) < TargetBlockChance;
	if (bBlocked)
	{
		Damage /= 2;
	}
	// 将格挡结果写入Context，供后续UI和特效读取
	UAuraAbilitySystemLibrary::SetBlockedHit(EffectContextHandle, bBlocked);

	// FGameplayEffectContext* Context = EffectContextHandle.Get();
	// FAuraGameplayEffectContext* AuraContext = static_cast<FAuraGameplayEffectContext*>(Context);
	// AuraContext->SetBlockedHit(bBlocked);
	// AuraContext->SetCriticalHit();

	// 捕获目标护甲值
	float TargetArmor = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorDef, EvaluateParams, TargetArmor);
	TargetArmor = FMath::Max<float>(0.f, TargetArmor);

	//SourceArmorPenetrationDef???
	// 捕获来源护甲穿透值
	float SourceArmorPenetrationDef = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorPenetrationDef, EvaluateParams,SourceArmorPenetrationDef);
	SourceArmorPenetrationDef = FMath::Max<float>(0.f, SourceArmorPenetrationDef);

	// 从曲线表获取护甲穿透系数（根据等级）
	UCharacterClassInfo* CharacterClassInfo = UAuraAbilitySystemLibrary::GetCharacterClassInfo(SourceAvatar);
	const FRealCurve* ArmorPenetrationRealCurve = CharacterClassInfo->DamageCalculationCoefficients->FindCurve(FName("ArmorPenetration"), FString());
	float ArmorPenetrationCoefficient = ArmorPenetrationRealCurve->Eval(SourcePlayerLevel);

	// 计算实际护甲（护甲穿透减少有效护甲）
	// 公式: 有效护甲 = 目标护甲 * (100 - 穿透值 * 穿透系数) / 100
	const float EffectiveArmor = TargetArmor *= (100 - SourceArmorPenetrationDef * ArmorPenetrationCoefficient) / 100.f;
	// 从曲线表获取护甲减免系数（根据目标等级）
	const FRealCurve* EffectiveArmorCurve = CharacterClassInfo->DamageCalculationCoefficients->FindCurve(FName("EfferctiveArmor"), FString());
	float EffectiveArmorCurveCoefficient = EffectiveArmorCurve->Eval(TargetPlayerLevel);
	// 护甲最终减伤
	// 公式: 伤害 = 伤害 * (100 - 有效护甲 * 护甲系数) / 100
	Damage *= (100 - EffectiveArmor * EffectiveArmorCurveCoefficient) / 100.f;


	// 获取暴击抵抗系数（目标等级越高，抵抗越强）
	const FRealCurve* CriticalHitResistanceCurve = CharacterClassInfo->DamageCalculationCoefficients->FindCurve(
		FName("CriticalHitResistance"), FString());
	float CriticalHitResistanceCoefficient = CriticalHitResistanceCurve->Eval(TargetPlayerLevel);

	// 捕获来源暴击几率（通常来自敏捷或装备）
	float SourceCriticalHitChance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitChanceDef, EvaluateParams,
	                                                           SourceCriticalHitChance);
	SourceCriticalHitChance = FMath::Max<float>(0.f, SourceCriticalHitChance);

	// 捕获目标暴击抵抗
	float TargetCriticalHitResistance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitResistanceDef, EvaluateParams, TargetCriticalHitResistance);
	TargetCriticalHitResistance = FMath::Max<float>(0.f, TargetCriticalHitResistance);

	// 捕获来源暴击伤害加成（额外伤害）
	float SourceCriticalHitDamage = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitDamageDef, EvaluateParams, SourceCriticalHitDamage);
	SourceCriticalHitDamage = FMath::Max<float>(0.f, SourceCriticalHitDamage);

	// 计算实际暴击几率（暴击抵抗降低实际几率）
	const float EffectiveCriticalHitChance = SourceCriticalHitChance - TargetCriticalHitResistance * CriticalHitResistanceCoefficient;
	const bool bCriticalHit = FMath::RandRange(1, 100) < EffectiveCriticalHitChance;

	// 将暴击结果写入Context
	UAuraAbilitySystemLibrary::SetCriticalHit(EffectContextHandle, bCriticalHit);

	// 暴击伤害公式: 基础伤害 * 2 + 暴击伤害加成
	Damage = bCriticalHit ? 2.f * Damage + SourceCriticalHitDamage : Damage;
	// Damage = 50;

	// UE_LOG(LogTemp, Warning, TEXT("======Damage: %f"), Damage);

	// 创建最终的GameplayModifier，指定修改的属性（IncomingDamage）和操作方式（Additive）
	// 触发 PostGameplayEffectExecute 进入IncomingDamage分支逻辑
	const FGameplayModifierEvaluatedData EffectData(UAureAttributeSet::GetIncomingDamageAttribute(), EGameplayModOp::Additive, Damage);
	OutExecutionOutput.AddOutputModifier(EffectData);
}
