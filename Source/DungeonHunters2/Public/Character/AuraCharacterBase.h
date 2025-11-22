// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystem/AureAttributeSet.h"
#include "GameFramework/Character.h"
#include "Interaction/CombatInterface.h"
#include "AuraCharacterBase.generated.h"

class UNiagaraSystem;
class UAbilitySystemComponent;
class UAttributeSet;
class UGameplayAbility;
class UGameplayEffect;

UCLASS(Abstract)
class DUNGEONHUNTERS2_API AAuraCharacterBase : public ACharacter, public IAbilitySystemInterface,
                                               public ICombatInterface
{
	GENERATED_BODY()

public:
	AAuraCharacterBase();
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAttributeSet* GetAttributeSet() const { return AttributeSet; }

	/**战斗接口*/
	virtual UAnimMontage* GetHitReactMontage_Implementation() override;
	virtual void Die() override;
	virtual FVector GetCombatSocketLocation_Implementation(const FGameplayTag& MontageTag) override;
	virtual bool IsDead_Implementation() const override;
	virtual AActor* GetAvatar_Implementation() override;
	virtual TArray<FTaggedMontage> GetAttackMontages_Implementation() override;
	virtual UNiagaraSystem* GetBloodEffect_Implementation() override;
	virtual FTaggedMontage GetTaggedMontageByTag_Implementation(const FGameplayTag& MontageTag) override;
	virtual int32 GetMinionCount_Implementation() override;
	virtual void IncrementMinionCount_Implementation(int32 Amount) override;
	
	/**战斗接口 end*/


	UFUNCTION(NetMulticast, Reliable)
	virtual void MulticastHandleDeath();

	UPROPERTY(EditAnywhere, Category="Combat")
	TArray<FTaggedMontage> AttackMontages;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Combat")
	TObjectPtr<USkeletalMeshComponent> Weapon;

	UPROPERTY(EditAnywhere, Category="Combat")
	FName WeaponTipSocketName;

	UPROPERTY(EditAnywhere, Category="Combat")
	FName LeftHandSocketName;

	UPROPERTY(EditAnywhere, Category="Combat")
	FName RightHandSocketName;

	UPROPERTY(EditAnywhere, Category="Combat")
	FName TailSocketName;


	bool bDead = false;

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;

	virtual void InitAbilityActorInfo();

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Abilities")
	TSubclassOf<UGameplayEffect> DefaultPrimaryAbilityEffect;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Abilities")
	TSubclassOf<UGameplayEffect> DefaultSecondaryAbilityEffect;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Abilities")
	TSubclassOf<UGameplayEffect> DefaultVitalAbilityEffect;

	// UPROPERTY(BlueprintReadOnly, EditAnywhere,Category="Abilities")
	// TSubclassOf<UGameplayEffect> DefaultAbilityEffect;

	void ApplyEffectToSelf(TSubclassOf<UGameplayEffect> GameplayEffectClass, float Leve) const;
	virtual void InitializeDefaultAbilities() const;

	void AddCharacterAbilities();

	/* Dissolve Effects*/

	void Dissolve();

	UFUNCTION(BlueprintImplementableEvent)
	void StartDissolveTimeline(UMaterialInstanceDynamic* MaterialInstanceDynamic);

	UFUNCTION(BlueprintImplementableEvent)
	void StartWeaponDissolveTimeline(UMaterialInstanceDynamic* MaterialInstanceDynamic);


	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UMaterialInstance> DissolveMaterialInstance;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UMaterialInstance> WeaponDissolveMaterialInstance;

	// UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Combat")
	UNiagaraSystem* BloodEffect;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Combat")
	USoundBase* DeathSound;


	/** Minion */
	int32 MinionCount = 0;

private:
	//技能列表
	UPROPERTY(EditAnywhere, Category="Abilities")
	TArray<TSubclassOf<UGameplayAbility>> StartUpAbility;

	UPROPERTY(EditAnywhere, Category="Combat")
	TObjectPtr<UAnimMontage> HitReactMontage;
};
