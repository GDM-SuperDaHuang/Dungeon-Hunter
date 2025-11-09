// daHuang


#include "AI/BTService_FindNearestPlayer.h"

#include "AIController.h"
#include "BehaviorTree/BTFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"

void UBTService_FindNearestPlayer::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	GEngine->AddOnScreenDebugMessage(1, 1.f, FColor::Red, *AIOwner.GetFName().ToString());
	GEngine->AddOnScreenDebugMessage(1, 1.f, FColor::Red, *ActorOwner.GetFName().ToString());

	APawn* OwnerPawn = AIOwner->GetPawn();

	const FName TargetTag = OwnerPawn->ActorHasTag(FName("Player")) ? FName("Enemy") : FName("Player");

	TArray<AActor*> ActorsWithTag;
	UGameplayStatics::GetAllActorsWithTag(OwnerPawn, TargetTag, ActorsWithTag);


	float ClosestDistance = TNumericLimits<float>::Max();
	AActor* ClosestActor = nullptr;

	for (AActor* Actor : ActorsWithTag)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Magenta, *Actor->GetFName().ToString());
		if (IsValid(Actor) && IsValid(OwnerPawn))
		{
			float Distance = OwnerPawn->GetDistanceTo(Actor);
			if (Distance < ClosestDistance)
			{
				ClosestDistance = Distance;
				ClosestActor = Actor;
			}
		}
	}
	UBTFunctionLibrary::SetBlackboardValueAsObject(this, TargetToFollowSelector, ClosestActor);
	UBTFunctionLibrary::SetBlackboardValueAsFloat(this, DistanceToFollowSelector, ClosestDistance);

}
