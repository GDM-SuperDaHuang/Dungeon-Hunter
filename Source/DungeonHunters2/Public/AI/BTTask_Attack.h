// daHuang

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlueprintBase.h"
#include "BTTask_Attack.generated.h"

/**
 * 
 */
UCLASS()
class DUNGEONHUNTERS2_API UBTTask_Attack : public UBTTask_BlueprintBase
{
	GENERATED_BODY()
public:

	/**
	 * 注意：Finish Execute需要调用，不然会无限执行这个任务，最终卡在这个任务里，行为树的其他节点无法被执行。
	 */
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
