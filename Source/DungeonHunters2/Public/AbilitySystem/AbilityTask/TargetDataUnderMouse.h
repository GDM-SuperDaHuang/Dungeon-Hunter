// daHuang

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "TargetDataUnderMouse.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMouseTargetDataSignature, const FGameplayAbilityTargetDataHandle&, DataHandle);

/**
 * 
 */
UCLASS()
class DUNGEONHUNTERS2_API UTargetDataUnderMouse : public UAbilityTask
{
	GENERATED_BODY()

public:
	//mata=( HidePin = "OwningAbility",DefaultToSelf = "OwningAbility",Blu)
	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks",
		meta=(DisplayName = "TargetDataUnderMouse", HidePin = "OwningAbility", DefaultToSelf = "OwningAbility",
			BlueprintInternalUseOnly = "true"))
	static UTargetDataUnderMouse* CreatTargetDataUnderMouse(UGameplayAbility* OwningAbility);


	UPROPERTY(BlueprintAssignable)
	FMouseTargetDataSignature ValidData;

private:

	virtual void Activate() override;

	void SendMouseCursorData();
};
