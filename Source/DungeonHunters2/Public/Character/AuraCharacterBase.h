// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystem/AureAttributeSet.h"
#include "GameFramework/Character.h"
#include "Interaction/CombatInterface.h"
#include "AuraCharacterBase.generated.h"

class UAbilitySystemComponent;
class UAttributeSet;
// class UGameplayEffect;

UCLASS(Abstract)
class DUNGEONHUNTERS2_API AAuraCharacterBase : public ACharacter, public IAbilitySystemInterface, public ICombatInterface
{
	GENERATED_BODY()

public:
	AAuraCharacterBase();
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAttributeSet* GetAttributeSet() const { return AttributeSet; }

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category="Combat")
	TObjectPtr<USkeletalMeshComponent> Weapon;

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
	void InitializeDefaultAbilities() const;
};
