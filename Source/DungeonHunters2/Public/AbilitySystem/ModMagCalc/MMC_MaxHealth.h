// daHuang

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "MMC_MaxHealth.generated.h"

/**
 * 
 */
UCLASS()
class DUNGEONHUNTERS2_API UMMC_MaxHealth : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()
public:
	UMMC_MaxHealth();
	// 核心计算函数：实现最大生命值的基础计算逻辑
	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;
private:
	 // 定义要捕获的属性：活力(Vigor)
	FGameplayEffectAttributeCaptureDefinition VigorDef;
};
