// daHuang


#include "AbilitySystem/ModMagCalc/MMC_MaxHealth.h"
#include "AbilitySystem/AureAttributeSet.h"
#include "Interaction/CombatInterface.h"

UMMC_MaxHealth::UMMC_MaxHealth()
{
	// 设置捕获属性为活力(Vigor)
	VigorDef.AttributeToCapture = UAureAttributeSet::GetVigorAttribute();
	// 指定从目标（接收效果的对象）获取属性值
	VigorDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	// 不创建属性快照（实时获取最新值）
	VigorDef.bSnapshot = false;
	// 注册要捕获的属性到系统，当属性变化时，会触发会调用CalculateBaseMagnitude_Implementation。
	RelevantAttributesToCapture.Add(VigorDef);
}

float UMMC_MaxHealth::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	// gather tags from source and target  步骤1: 获取效果相关的标签数据
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();
	// 步骤2: 设置评估参数（包含源/目标标签）
	FAggregatorEvaluateParameters EvaluateParams;
	EvaluateParams.SourceTags = SourceTags;
	EvaluateParams.TargetTags = TargetTags;
	// 步骤3: 获取目标的活力值
	float Vigor = 0.0f;
	GetCapturedAttributeMagnitude(VigorDef, Spec, EvaluateParams, Vigor);
	Vigor = FMath::Max(Vigor, 0.0f);
	// 步骤4: 获取来源对象的角色等级
	/**
	 * 两种方式设置源对象 
	 * 1，ApplyEffectToTarget 是设置的源对象，Spec.GetContext().GetSourceObject()
	 * 2，    
	 * AurePlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(
	 *  AurePlayerState, // OwnerActor = PlayerState
	 *  this             // AvatarActor = Character自身
    );
	*/
	// ICombatInterface* CombatInterface = Cast<ICombatInterface>(Spec.GetContext().GetSourceObject());
	UObject* SourceObject = Spec.GetContext().GetSourceObject();
	
	ICombatInterface* CombatInterface = Cast<ICombatInterface>(SourceObject);
	const int32 PlayerLevel = CombatInterface->GetPlayerLevel();
	// 步骤5: 计算最终最大生命值
	return PlayerLevel * 10 + Vigor * 2 + 100;


	// UObject* SourceObject = Spec.GetContext().GetSourceObject();
	// ICombatInterface* CombatInterface = Cast<ICombatInterface>(SourceObject);
	// const int32 PlayerLevel = CombatInterface->GetPlayerLevel();
	// return PlayerLevel * 10 + Vigor * 2 + 100;
} 
