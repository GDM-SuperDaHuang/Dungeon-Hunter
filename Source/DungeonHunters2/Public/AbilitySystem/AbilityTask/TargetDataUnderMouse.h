// daHuang

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "TargetDataUnderMouse.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMouseTargetDataSignature, const FGameplayAbilityTargetDataHandle&,
                                            DataHandle);
/**
 * 
 */
UCLASS()
class DUNGEONHUNTERS2_API UTargetDataUnderMouse : public UAbilityTask
{
	GENERATED_BODY()

public:
	/**
	 * DisplayName = "TargetDataUnderMouse": 在蓝图中显示的函数名称。
	 * HidePin = "OwningAbility": 在蓝图中调用此函数时，将隐藏OwningAbility引脚。因为我们已经设置了DefaultToSelf，所以这个引脚不需要显示。
	 * DefaultToSelf = "OwningAbility": 将调用此函数的对象（通常是当前的GameplayAbility）默认作为OwningAbility参数传入。这意味着在蓝图中，这个参数会自动填充为当前的Ability，不需要手动连接。
	 * BlueprintInternalUseOnly = "true": 这个函数仅限蓝图内部使用，不会出现在节点的搜索列表中，只能通过特定方式（比如AbilityTask节点）调用。
	 */
	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks",
		meta=(DisplayName = "TargetDataUnderMouse", HidePin = "OwningAbility", DefaultToSelf = "OwningAbility",
			BlueprintInternalUseOnly = "true"))
	static UTargetDataUnderMouse* CreatTargetDataUnderMouse(UGameplayAbility* OwningAbility);
	
	/**
	 * 因为蓝图节点帮你提前把 FGameplayAbilityTargetDataHandle 拆包成 FHitResult，
	 * 再把 FHitResult 拆成 Vector 的 Location，所以看起来像是“直接输出 Vector”，
	 * 但实际上委托签名永远是 FGameplayAbilityTargetDataHandle —— 只是 Epic 给你包了一层“糖衣”。
	 */
	UPROPERTY(BlueprintAssignable)
	FMouseTargetDataSignature ValidData;

private:
	virtual void Activate() override;

	void SendMouseCursorData();

	void OnTargetDataReplicatedCallback(const FGameplayAbilityTargetDataHandle& DataHandle, FGameplayTag ActivationTag);
};
