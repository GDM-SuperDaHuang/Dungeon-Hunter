// daHuang

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "AuraGameplayAbility.generated.h"

/**
 * 项目中所有技能的基类，扩展自UGameplayAbility，主要用于关联输入与技能
 * 核心功能：通过GameplayTag实现技能与输入的解耦，方便灵活绑定不同输入（如鼠标、键盘）
 * GiveAbility(Spec);--> DynamicAbilityTags 会被 复制到客户端影子 ASC。
 */
UCLASS()
class DUNGEONHUNTERS2_API UAuraGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
public:
	/**
	 * 技能关联的输入标签（如"InputTag.LMB"对应左键，"InputTag.1"对应数字键1）
	 * 在编辑器中配置，用于在技能添加时绑定到输入事件
	 * 与UAureAbilitySystemComponent中的输入处理逻辑联动
	 */
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	FGameplayTag StartupInputTag;


};
