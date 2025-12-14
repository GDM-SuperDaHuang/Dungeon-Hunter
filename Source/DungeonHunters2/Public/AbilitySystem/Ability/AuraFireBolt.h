// daHuang

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability/AuraProjectileSpell.h"
#include "AuraFireBolt.generated.h"

/**
 * 
 */
UCLASS()
class DUNGEONHUNTERS2_API UAuraFireBolt : public UAuraProjectileSpell
{
	GENERATED_BODY()

public:
	virtual FString GetDescription(int32 Level) override;
	virtual FString GetNextLevelDescription(int32 Level) override;

	UFUNCTION(BlueprintCallable, Category = "Projectile")
	void SpawnProjectiles(const FVector& ProjectileTargetLocation, const FGameplayTag& SocketTag,bool bOverridePitch,float PitchOverride,AActor* HomingTarget);
protected:

	UPROPERTY(EditDefaultsOnly, Category = "FireBolt")
	float ProjectilesSpread = 90.f;

	UPROPERTY(EditDefaultsOnly, Category = "FireBolt")
	int32 MaxNumProjectiles = 5;

	UPROPERTY(EditDefaultsOnly, Category = "FireBolt")
	float HomingAccelerationMin = 1600;

	UPROPERTY(EditDefaultsOnly, Category = "FireBolt")
	float HomingAccelerationMax = 3200;

	UPROPERTY(EditDefaultsOnly, Category = "FireBolt")
	bool bLaunchHomingProjectiles = true;
};
