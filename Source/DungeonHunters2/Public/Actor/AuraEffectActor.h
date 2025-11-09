// daHuang

#pragma once

#include "CoreMinimal.h"
#include "ActiveGameplayEffectHandle.h"
#include "GameFramework/Actor.h"
#include "AuraEffectActor.generated.h"

class UGameplayEffect;
class UAbilitySystemComponent;

UENUM(BlueprintType)
enum class EEffectApplicationPolicy : uint8
{
	ApplyOnOverlap,// 当目标与当前Actor重叠时应用
	ApplyOnEndOverlap,// 当目标与当前Actor结束重叠时应用
	DoNotApply,// 不应用
};

// 效果移除策略：定义何时移除已应用的效果（主要针对无限期效果）
UENUM(BlueprintType)
enum class EEffectRemovalPolicy : uint8
{
	RemoveOnEndOverlap,// 当目标结束重叠时移除
	DoNotRemoval,// 不移除（效果持续到自身结束条件）
};

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

	/**
	 * 向目标Actor应用GameplayEffect
	 * @param TargetActor 目标Actor（需拥有AbilitySystemComponent）
	 * @param GameplayEffectClass 要应用的效果模板（如"治疗效果""中毒效果"）
	 */
	UFUNCTION(BlueprintCallable)
	void ApplyEffectToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect> GameplayEffectClass);

	/**
	 * 当目标与当前Actor开始重叠时调用
	 * 根据应用策略触发对应效果的应用
	 * @param TargetActor 重叠的目标Actor
	 */
	UFUNCTION(BlueprintCallable)
	void OnOverlap(AActor* TargetActor);

	/**
	 * 当目标与当前Actor结束重叠时调用
	 * 根据应用策略触发对应效果的应用，或根据移除策略移除无限期效果
	 * @param TargetActor 结束重叠的目标Actor
	 */
	UFUNCTION(BlueprintCallable)
	void EndOverlap(AActor* TargetActor);

	/** 当效果被移除时，是否销毁当前Actor（如一次性消耗品） */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Applied Effects")
	bool bDestroyOnEffectApplication = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Applied Effects")
	bool bApplyEffectEnemies = false;

	
	// 三种效果的应用策略配置（在蓝图/编辑器中可设置）
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Applied Effects")
	EEffectApplicationPolicy InstantEffectApplicationPolicy = EEffectApplicationPolicy::DoNotApply;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Applied Effects")
	EEffectApplicationPolicy DurationEffectApplicationPolicy = EEffectApplicationPolicy::DoNotApply;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Applied Effects")
	EEffectApplicationPolicy InfiniteEffectApplicationPolicy = EEffectApplicationPolicy::DoNotApply;
	/** 无限期效果的移除策略（默认重叠结束时移除） */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Applied Effects")
	EEffectRemovalPolicy InfiniteEEffectRemovalPolicy = EEffectRemovalPolicy::RemoveOnEndOverlap;

	
	// 三种效果的模板类（在蓝图/编辑器中指定具体效果，如"BP_HealInstant""BP_PoisonDuration"）
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Applied Effects")
	TSubclassOf<UGameplayEffect> InstantGameplayEffectClass;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Applied Effects")
	TSubclassOf<UGameplayEffect> DurationGameplayEffectClass;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Applied Effects")
	TSubclassOf<UGameplayEffect> InfiniteGameplayEffectClass;
	
	/** 
	 * 跟踪已激活的无限期效果：键为效果句柄（用于后续移除），值为目标的ASC
	 * 作用：当目标离开重叠区域时，通过句柄精准移除对应的无限期效果
	 */
	TMap<FActiveGameplayEffectHandle, UAbilitySystemComponent*> ActiveEffectHandles;
	
	/** 效果等级（影响效果强度，如等级2的治疗量是等级1的2倍） */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Applied Effects")
	float ActorLevel = 1.0f;
private:
	// UPROPERTY(VisibleAnywhere)
	// TObjectPtr<USphereComponent> Sphere;
	//
	// UPROPERTY(VisibleAnywhere)
	// TObjectPtr<UStaticMeshComponent> Mesh;
};
