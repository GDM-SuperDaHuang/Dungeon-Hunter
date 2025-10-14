// daHuang


#include "AbilitySystem/AbilityTask/TargetDataUnderMouse.h"
#include "AbilitySystemComponent.h"

UTargetDataUnderMouse* UTargetDataUnderMouse::CreatTargetDataUnderMouse(UGameplayAbility* OwningAbility)
{
	UTargetDataUnderMouse* MyObj = NewAbilityTask<UTargetDataUnderMouse>(OwningAbility);
	if (MyObj != nullptr)
	{
		UE_LOG(LogTemp, Display, TEXT("CreatTargetDataUnderMouse"));
	}
	return MyObj;
}

void UTargetDataUnderMouse::Activate()
{
	const bool bIsLocallyControlled = Ability->GetCurrentActorInfo()->IsLocallyControlled();

	if (bIsLocallyControlled)
	{
		SendMouseCursorData();
	}
	else
	{
		const FGameplayAbilitySpecHandle SpecHandle = GetAbilitySpecHandle();
		const FPredictionKey ActivationPredictionKey = GetActivationPredictionKey();
		
		//todo server
		AbilitySystemComponent.Get()->AbilityTargetDataSetDelegate(
			GetAbilitySpecHandle(), GetActivationPredictionKey()).AddUObject(
			this, &UTargetDataUnderMouse::OnTargetDataReplicatedCallback);

		const bool bCalledDelegate = AbilitySystemComponent.Get()->CallReplicatedTargetDataDelegatesIfSet(SpecHandle,ActivationPredictionKey);
		if (!bCalledDelegate)
		{
			SetWaitingOnRemotePlayerData();
		}
	}
	// Super::Activate();
}

void UTargetDataUnderMouse::SendMouseCursorData()
{
	// 1. 检查预测键（ScopedPredictionWindow）是否有效，用于预测和网络同步
	FScopedPredictionWindow ScopedPrediction(AbilitySystemComponent.Get());
	// 2. 获取玩家控制器
	APlayerController* PC = Ability->GetCurrentActorInfo()->PlayerController.Get();
	// 3. 获取鼠标光标下的命中结果
	FHitResult CursorHit;
	PC->GetHitResultUnderCursor(ECC_Visibility, true, CursorHit);

	
	// 4. 构建目标数据
	FGameplayAbilityTargetDataHandle DataHandle;
	FGameplayAbilityTargetData_SingleTargetHit* Data = new FGameplayAbilityTargetData_SingleTargetHit();
	Data->HitResult = CursorHit;
	DataHandle.Add(Data);

	// ValidData.Broadcast(CursorHit.Location);
	
	// 5. 将目标数据发送到服务器（如果存在网络连接）
	AbilitySystemComponent->ServerSetReplicatedTargetData(
		GetAbilitySpecHandle(),
		GetActivationPredictionKey(),
		DataHandle,
		FGameplayTag(),
		AbilitySystemComponent->ScopedPredictionKey);

	// 6. 如果该任务仍然有效，广播目标数据
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		ValidData.Broadcast(DataHandle);
	}
}

void UTargetDataUnderMouse::OnTargetDataReplicatedCallback(const FGameplayAbilityTargetDataHandle& DataHandle,
                                                           FGameplayTag ActivationTag)
{
	AbilitySystemComponent->ConsumeClientReplicatedTargetData(GetAbilitySpecHandle(),GetActivationPredictionKey());
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		ValidData.Broadcast(DataHandle);
	}
}
