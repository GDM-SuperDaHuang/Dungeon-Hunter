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

/**
 * 发射，无实时追踪目标，只开头确定目标位置后发射。
 */
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
		// 生成一个Actor ,并且延迟初始化，不进行其他操作	
		AAuraProjectile* Projectile = GetWorld()->SpawnActorDeferred<AAuraProjectile>(
				ProjectileClass,
				SpawnTransform,// 初始 Transform
				GetOwningActorFromActorInfo(),// Owner（通常是施法角色）
				Cast<APawn>(GetOwningActorFromActorInfo()),// Instigator（负责计入伤害统计）
				ESpawnActorCollisionHandlingMethod::AlwaysSpawn);// 碰撞策略：强行生成，不管是否卡墙
		Projectile->DamageEffectParams = MakeDamageEffectParamsFromClassDefaults();

		if (HomingTarget && HomingTarget->Implements<UCombatInterface>())
		{
			Projectile->ProjectileMovement->HomingTargetComponent = HomingTarget->GetRootComponent();
		}
		else
		{	
			//注意指针！！！
			//这里的Projectile->ProjectileMovement->HomingTargetComponent是弱引用，如果目标的被销毁，那么该位置消失,Projectile->ProjectileMovement->HomingTargetComponent指向null
			// Projectile->ProjectileMovement->HomingTargetComponent = NewObject<USceneComponent>(USceneComponent::StaticClass());
		
			//创建一个锚点，先缓存起来
			Projectile->HomingTargetSceneComponent = NewObject<USceneComponent>(USceneComponent::StaticClass());
			Projectile->HomingTargetSceneComponent->SetWorldLocation(ProjectileTargetLocation);
			//设置目标地点
			Projectile->ProjectileMovement->HomingTargetComponent  =Projectile->HomingTargetSceneComponent;
		}
		Projectile->ProjectileMovement->HomingAccelerationMagnitude = FMath::FRandRange(HomingAccelerationMin, HomingAccelerationMax);
		Projectile->ProjectileMovement->bIsHomingProjectile = bLaunchHomingProjectiles;

		// 让导弹真正“启动” ，SpawnActorDeferred需要FinishSpawning， 完成投射物生成（应用之前的设置，触发AAuraProjectile的BeginPlay）
		/**
		 * FinishSpawning 会触发：
		 * AActor::PostActorCreated()
		 * UActorComponent::Activate()（含 ProjectileMovement）
		 * AActor::BeginPlay()
		 */
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
