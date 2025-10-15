// daHuang


#include "AbilitySystem/AbilityTask/TargetDataUnderMouse.h"
#include "AbilitySystemComponent.h"

/**
 * 工厂函数，蓝图节点“TargetDataUnderMouse”的入口
 * 返回一个 Task 实例，生命周期由 Ability 自动管理
 */
UTargetDataUnderMouse* UTargetDataUnderMouse::CreatTargetDataUnderMouse(UGameplayAbility* OwningAbility)
{
	UTargetDataUnderMouse* MyObj = NewAbilityTask<UTargetDataUnderMouse>(OwningAbility);
	if (MyObj != nullptr)
	{
		UE_LOG(LogTemp, Display, TEXT("CreatTargetDataUnderMouse"));
	}
	return MyObj;
}

/**
 * AbilityTask 的入口，区分本地与远程调用
 * 本地控制：直接采集鼠标位置并发送给服务器
 * 远程（服务器）控制：等待客户端同步过来的 TargetData
 */
void UTargetDataUnderMouse::Activate()
{
	const bool bIsLocallyControlled = Ability->GetCurrentActorInfo()->IsLocallyControlled();

	if (bIsLocallyControlled)
	{
		SendMouseCursorData();
	}
	else
	{
		// 服务器 / 监听主机：等待客户端同步
		const FGameplayAbilitySpecHandle SpecHandle = GetAbilitySpecHandle();
		const FPredictionKey ActivationPredictionKey = GetActivationPredictionKey();
		
		// 注册回调：当客户端 TargetData 到达后触发
		AbilitySystemComponent.Get()->AbilityTargetDataSetDelegate(
			GetAbilitySpecHandle(), GetActivationPredictionKey()).AddUObject(
			this, &UTargetDataUnderMouse::OnTargetDataReplicatedCallback);

		// 如果客户端数据已经提前到达，则立即触发；否则 SetWaitingOnRemotePlayerData
		const bool bCalledDelegate = AbilitySystemComponent.Get()->CallReplicatedTargetDataDelegatesIfSet(SpecHandle,ActivationPredictionKey);
		if (!bCalledDelegate)
		{
			SetWaitingOnRemotePlayerData(); // 暂停 Ability 执行，等待网络
		}
	}
	// Super::Activate();
}

/**
 * 本地玩家采集鼠标位置 → 构造 TargetData → 发送给服务器
 * 使用 ScopedPredictionWindow 做输入预测
 */
void UTargetDataUnderMouse::SendMouseCursorData()
{
	// 1. 检查预测键（ScopedPredictionWindow）是否有效，用于预测和网络同步， 打开预测窗口，确保本地预测与服务器一致
	FScopedPredictionWindow ScopedPrediction(AbilitySystemComponent.Get());
	// 2. 获取玩家控制器
	APlayerController* PC = Ability->GetCurrentActorInfo()->PlayerController.Get();
	// 3. 获取鼠标光标下的命中结果
	FHitResult CursorHit;
	PC->GetHitResultUnderCursor(ECC_Visibility, true, CursorHit);

	
	// 4. 构造 GAS 所需的 TargetData 结构
	FGameplayAbilityTargetDataHandle DataHandle;
	FGameplayAbilityTargetData_SingleTargetHit* Data = new FGameplayAbilityTargetData_SingleTargetHit();
	Data->HitResult = CursorHit;
	DataHandle.Add(Data);

	// ValidData.Broadcast(CursorHit.Location);
	
	// 5. 如果存在网络连接，将 TargetData 作为“复制参数”发送给服务器
	AbilitySystemComponent->ServerSetReplicatedTargetData(
		GetAbilitySpecHandle(),
		GetActivationPredictionKey(),
		DataHandle,
		FGameplayTag(),// 无额外标签
		AbilitySystemComponent->ScopedPredictionKey);

	// 6. 如果该任务Task 仍然有效，广播 Blueprint 节点 
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		ValidData.Broadcast(DataHandle);
	}
}

/**
 * 服务器收到客户端同步的 TargetData 后回调
 * 消费数据并继续 Ability 执行
 */
void UTargetDataUnderMouse::OnTargetDataReplicatedCallback(const FGameplayAbilityTargetDataHandle& DataHandle,
                                                           FGameplayTag ActivationTag)
{
	 // 消费掉该份数据，防止重复触发
	AbilitySystemComponent->ConsumeClientReplicatedTargetData(GetAbilitySpecHandle(),GetActivationPredictionKey());
	// 继续 Ability 执行，把数据抛给蓝图
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		ValidData.Broadcast(DataHandle);
	}
}
