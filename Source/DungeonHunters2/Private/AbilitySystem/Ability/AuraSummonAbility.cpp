// daHuang


#include "AbilitySystem/Ability/AuraSummonAbility.h"

#include "Kismet/KismetSystemLibrary.h"

TArray<FVector> UAuraSummonAbility::GetSpawnLocation()
{
	const FVector Forward = GetAvatarActorFromActorInfo()->GetActorForwardVector();
	const FVector Location = GetAvatarActorFromActorInfo()->GetActorLocation();
	float DeltaSpread = SpawnSpread / NumMinion;
	UE::Math::TVector<double> LeftOfSpread = Forward.RotateAngleAxis(SpawnSpread / 2.f, FVector::UpVector);
	UE::Math::TVector<double> RightOfSpread = Forward.RotateAngleAxis(SpawnSpread / 2.f, FVector::UpVector);

	TArray<FVector> SpawnLocations;
	for (int i = 0; i < NumMinion; i++)
	{
		UE::Math::TVector<double> Direction = RightOfSpread.RotateAngleAxis(DeltaSpread * i, FVector::UpVector);
		UE::Math::TVector<double> ChosenSpawnLocation = Location + Direction * FMath::FRandRange(
			MinSpawnDistance, MaxSpawnDistance);

		FHitResult Hit;
		GetWorld()->LineTraceSingleByChannel(
			Hit,
			ChosenSpawnLocation + FVector(0, 0, 400.f),
			ChosenSpawnLocation - FVector(0, 0, 400.f),
			ECC_Visibility);

		if (Hit.bBlockingHit)
		{
			ChosenSpawnLocation = Hit.ImpactPoint;
		}
		SpawnLocations.Add(ChosenSpawnLocation);

		// UKismetSystemLibrary::DrawDebugArrow(GetAvatarActorFromActorInfo(),
		//                                      Location,
		//                                      Location + Direction * MaxSpawnDistance,
		//                                      4,
		//                                      FLinearColor::Green,
		//                                      3.f);
	}


	// UKismetSystemLibrary::DrawDebugArrow(GetAvatarActorFromActorInfo(),
	//                                      Location,
	//                                      Location + LeftOfSpread * MaxSpawnDistance,
	//                                      4,
	//                                      FLinearColor::Green,
	//                                      3.f);
	//
	// DrawDebugSphere(GetWorld(), Location + LeftOfSpread * MaxSpawnDistance, 15.f, 12, FColor::Green, false, 3.f);
	//
	// UKismetSystemLibrary::DrawDebugArrow(GetAvatarActorFromActorInfo(),
	//                                      Location,
	//                                      Location + RightOfSpread * MaxSpawnDistance,
	//                                      4,
	//                                      FLinearColor::Red,
	//                                      3.f);
	// DrawDebugSphere(GetWorld(), Location + RightOfSpread * MaxSpawnDistance, 15.f, 12, FColor::Red, false, 3.f);

	return SpawnLocations;
}

TSubclassOf<APawn> UAuraSummonAbility::GetRandomMinionClass()
{
	const int32 Selection = FMath::RandRange(0, MinionClass.Num() - 1);
	return MinionClass[Selection];
}
