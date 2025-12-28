// daHuang

#pragma once

#include "CoreMinimal.h"
#include "AuraAbilityTypes.h"
#include "../../../../../../Softstore/Epic Games/UE_5.4/Engine/Plugins/FX/Niagara/Source/Niagara/Classes/NiagaraSystem.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "AuraProjectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;

UCLASS()
class DUNGEONHUNTERS2_API AAuraProjectile : public AActor
{
	GENERATED_BODY()

public:
	AAuraProjectile();
	 
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UProjectileMovementComponent> ProjectileMovement;

	// UPROPERTY(BlueprintReadWrite,meta=(ExposeOnSpawn = true))
	// FGameplayEffectSpecHandle DamageEffectHandle;

	UPROPERTY(BlueprintReadWrite,meta=(ExposeOnSpawn = true))
	FDamageEffectParams DamageEffectParams;

	UPROPERTY()
	TObjectPtr<USceneComponent> HomingTargetSceneComponent;
protected:
	virtual void BeginPlay() override;
	
	UFUNCTION(BlueprintCallable)
	virtual void OnHit();
	virtual void Destroyed() override;
	UFUNCTION()
	virtual void OnSphereOverlap(UPrimitiveComponent*  OverlappedComponent,AActor* OtherActor,UPrimitiveComponent* OtherComp,int32 OtherBodyIndex,bool bFromSweep,const FHitResult& SweepResult);

	bool IsValidOverlap(AActor* OtherActor);
	bool bHit = false;
	
	UPROPERTY()
	TObjectPtr<UAudioComponent> LoopingSoundComponent;
private:

	UPROPERTY(EditDefaultsOnly)
	float LiftSpan = 10.f;
	
	
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> Sphere;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UNiagaraSystem> ImpactEffect;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundBase> ImpactSound;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundBase> LoopingSound;


};
