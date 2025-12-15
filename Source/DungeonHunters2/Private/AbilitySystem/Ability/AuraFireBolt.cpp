// daHuang


#include "AbilitySystem/Ability/AuraFireBolt.h"

#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "AbilitySystem/AureAbilitySystemComponent.h"
#include "Actor/AuraProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"


FString UAuraFireBolt::GetDescription(int32 Level)
{
	const float ManaCost = GetManaCost(Level);
	float Cooldown = GetCooldown(Level);
	if (Level == 1)
	{
		return FString::Printf(TEXT("%d,%f,%f"), NumProjectiles, ManaCost, Cooldown);
	}
	else
	{
		return FString::Printf(TEXT(""));
	}
}

FString UAuraFireBolt::GetNextLevelDescription(int32 Level)
{
	if (Level == 1)
	{
		return FString::Printf(TEXT(""));
	}
	else
	{
		return FString::Printf(TEXT(""));
	}
}

void UAuraFireBolt::SpawnProjectiles(
	const FVector& ProjectileTargetLocation,
	const FGameplayTag& SocketTag,
	bool bOverridePitch,
	float PitchOverride,
	AActor* HomingTarget)
{
	bool bHasAuthority = GetAvatarActorFromActorInfo()->HasAuthority();
	if (!bHasAuthority) return;
	
	const FVector SocketLocation = ICombatInterface::Execute_GetCombatSocketLocation(
		GetAvatarActorFromActorInfo(), SocketTag);
	// 从武器的特定socket（如"WeaponTip"）获取发射位置
	// const FVector CombatSocketLocation = CombatInterface->GetCombatSocketLocation();
	FRotator Rotation = (ProjectileTargetLocation - SocketLocation).Rotation();
	if (bOverridePitch)
	{
		Rotation.Pitch = PitchOverride;
	}

	const FVector Forward = Rotation.Vector();
	const int32 EffectiveNumProjectiles = FMath::Min(NumProjectiles, GetAbilityLevel());
	TArray<FRotator> FRotators = UAuraAbilitySystemLibrary::EvenlySpacedRotators(Forward, FVector::UpVector, ProjectilesSpread, EffectiveNumProjectiles);
	for (FRotator& Rot : FRotators)
	{
		//射击 ,并且在武器处产生
		FTransform SpawnTransform;
		SpawnTransform.SetLocation(SocketLocation);
		SpawnTransform.SetRotation(Rotation.Quaternion());
		AAuraProjectile* Projectile = GetWorld()->SpawnActorDeferred<AAuraProjectile>(
				ProjectileClass,
				SpawnTransform,
				GetOwningActorFromActorInfo(),
				Cast<APawn>(GetOwningActorFromActorInfo()),
				ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
		Projectile->DamageEffectParams = MakeDamageEffectParamsFromClassDefaults();

		if (HomingTarget && HomingTarget->Implements<UCombatInterface>())
		{
			Projectile->ProjectileMovement->HomingTargetComponent = HomingTarget->GetRootComponent();
		}
		else
		{//注意指针
			// Projectile->ProjectileMovement->HomingTargetComponent = NewObject<USceneComponent>(USceneComponent::StaticClass());
		
			Projectile->HomingTargetSceneComponent = NewObject<USceneComponent>(USceneComponent::StaticClass());
			Projectile->HomingTargetSceneComponent->SetWorldLocation(ProjectileTargetLocation);
			Projectile->ProjectileMovement->HomingTargetComponent  =Projectile->HomingTargetSceneComponent;
		}
		Projectile->ProjectileMovement->HomingAccelerationMagnitude = FMath::FRandRange(HomingAccelerationMin, HomingAccelerationMax);
		Projectile->ProjectileMovement->bIsHomingProjectile = bLaunchHomingProjectiles;

		// SpawnActorDeferred需要FinishSpawning， 完成投射物生成（应用之前的设置，触发AAuraProjectile的BeginPlay）
		Projectile->FinishSpawning(SpawnTransform);
	
	}

	// TArray<FVector> Directions = UAuraAbilitySystemLibrary::EvenlySpacedVector(Forward, FVector::UpVector, ProjectilesSpread, NumProjectiles);
	// for (FVector& Direction : Directions)
	// {
	// 	UKismetSystemLibrary::DrawDebugArrow(
	// 		GetAvatarActorFromActorInfo(),
	// 		SocketLocation,
	// 		SocketLocation+Direction*75.f,
	// 		5,FLinearColor::Red,
	// 		120,
	// 		5);
	// 	
	// }
}
