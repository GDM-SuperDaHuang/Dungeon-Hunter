// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "Interaction/EnenmyInterface"
#include "AuraEnemy.generated.h"

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

protected:
	virtual void BeginPlay() override;
	// //
	// UPROPERTY(BlueprintReadOnly)
	// bool bHighlighted = false;
	virtual void InitAbilityActorInfo() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Character Class Defaults")
	int32 Level = 1;

private:
};
