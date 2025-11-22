// daHuang

#pragma once

#include "CoreMinimal.h"
#include "AuraDamageGameplayAbility.h"
#include "AuraProjectileSpell.generated.h"

class AAuraProjectile;
/**
 * 投射物技能的基类，继承自UAuraGameplayAbility
 * 专门用于处理需要发射实体投射物（如火球、冰箭）的技能
 */
UCLASS()
class DUNGEONHUNTERS2_API UAuraProjectileSpell : public UAuraDamageGameplayAbility
{
	GENERATED_BODY()

public:

protected:
	/**
	 * 重写技能激活函数，是技能逻辑的入口
	 * 当技能被触发（如按下对应输入键）时调用
	 * @param Handle：技能实例的句柄，用于标识当前激活的技能
	 * @param ActorInfo：包含技能所属的Actor信息（如施法者、控制器）
	 * @param ActivationInfo：激活相关的上下文信息（如预测键、激活次数）
	 * @param TriggerEventData：触发技能的事件数据（如点击位置、目标信息）
	 */
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	                             const FGameplayAbilityActivationInfo ActivationInfo,
	                             const FGameplayEventData* TriggerEventData) override;

	/**
	 * 生成并发射投射物的函数（蓝图可调用）
	 * 负责计算发射位置、生成投射物实例、设置投射物属性（如伤害、速度）
	 */
	UFUNCTION(BlueprintCallable, Category = "Projectile")
	void SpawnProjectile(const FVector& ProjectileTargetLocation, const FGameplayTag& SocketTag);

	/**
	 * 投射物的类模板（在编辑器中指定，如AFireballProjectile、IColdArrowProjectile）
	 * 技能激活时会根据此模板生成具体的投射物Actor
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AAuraProjectile> ProjectileClass;
};
