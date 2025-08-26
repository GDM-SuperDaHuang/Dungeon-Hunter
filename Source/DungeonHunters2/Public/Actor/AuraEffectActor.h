// daHuang

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AuraEffectActor.generated.h"
class UGameplayEffect;

UCLASS()
class DUNGEONHUNTERS2_API AAuraEffectActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AAuraEffectActor();
	// UFUNCTION()
	// virtual void OnOverlap(UPrimitiveComponent* UOverlappedComponent,AActor* OtherActor,UPrimitiveComponent* OtherComp,int32 OtherBodyIndex,bool bFromSweep,const FHitResult& SweepResult) ;
	//
	// UFUNCTION()
	// virtual void EndOverlap(UPrimitiveComponent* UOverlappedComponent,AActor* OtherActor,UPrimitiveComponent* OtherComp,int32 OtherBodyIndex);

protected:
	virtual void BeginPlay() override;


	void ApplyEffectToTarget(AActor* Target, TSubclassOf<UGameplayEffect> GameplayEffectClass);

	UPROPERTY(EditAnywhere, Category="Applied Effects")
	TSubclassOf<UGameplayEffect> InstantGameplayEffectClass;

private:
	// UPROPERTY(VisibleAnywhere)
	// TObjectPtr<USphereComponent> Sphere;
	//
	// UPROPERTY(VisibleAnywhere)
	// TObjectPtr<UStaticMeshComponent> Mesh;
};
