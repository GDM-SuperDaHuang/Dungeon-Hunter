// daHuang

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"
#include "AurePlayerState.generated.h"

class UAbilitySystemComponent;
class UAttributeSet;

/**
 * 
 */
UCLASS()
class DUNGEONHUNTERS2_API AAurePlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AAurePlayerState();
	// virtual void GetLifetimeReplicatedProps(float DeltaTime) override;
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	/*** IAbilitySystemInterface */
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	/*** IAbilitySystemInterface end*/
	
	FORCEINLINE UAttributeSet* GetAttributeSet() const { return AttributeSet; }

	int32 GetPlayerLevel() const { return Level; }
protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;

private:

	//如果只是使用了 DOREPLIFETIME而没有使用 ReplicatedUsing，那么当属性复制到客户端时，客户端会更新该属性的值，但不会触发任何回调函数。你只能通过直接访问该属性来获取更新后的值。
	UPROPERTY(VisibleAnywhere, ReplicatedUsing=OnRep_Level)
	int32 Level = 1;

	UFUNCTION()
	void OnRep_Level();
};
