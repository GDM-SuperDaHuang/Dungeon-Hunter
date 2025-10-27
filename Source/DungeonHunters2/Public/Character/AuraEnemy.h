// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "Character/AuraCharacterBase.h"
#include "Components/WidgetComponent.h"
#include "Interaction/EnenmyInterface.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "AuraEnemy.generated.h"

enum class ECharacterClass : uint8;
/**
 * 
 */
UCLASS()
class DUNGEONHUNTERS2_API AAuraEnemy : public AAuraCharacterBase, public IEnenmyInterface
{
	GENERATED_BODY()

	//public IEnenmyInterface 必须被重写，不然报错
public:
	AAuraEnemy();

	/** Begin IEnenmyInterface */
	//敌人选中后高光
	virtual void HighlightActor() override;
	//取消敌人选中后高光
	virtual void UnHighlightActor() override;
	/** End IEnenmyInterface */

	/**combat Interface*/
	virtual int32 GetPlayerLevel() override;
	virtual void Die() override;
	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnHealthChanged;

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnMaxHealthChanged;
	void HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount);

	UPROPERTY(BlueprintReadOnly,Category="Combat")
	bool bHitReacting = false;

	UPROPERTY(BlueprintReadOnly,Category="Combat")
	float BaseWalkSpeed = 250.f;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Combat")
	float LifeSpan = 5.f;
	
protected:
	virtual void BeginPlay() override;
	// //
	// UPROPERTY(BlueprintReadOnly)
	// bool bHighlighted = false;
	virtual void InitAbilityActorInfo() override;
	virtual void InitializeDefaultAbilities() const override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Character Class Defaults")
	int32 Level = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Character Class Defaults")
	ECharacterClass CharacterClass = ECharacterClass::Warrior;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UWidgetComponent> HealthBar;

private:
};
