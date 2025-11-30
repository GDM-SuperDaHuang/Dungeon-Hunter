// daHuang

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"
#include "AurePlayerState.generated.h"

class ULevelUpInfo;
class UAbilitySystemComponent;
class UAttributeSet;

/**
 * 1,ACharacter / Pawn
 * 由 Level 场景 或 运行时 SpawnActor 生成 → 属于 World 上下文，换地图、死亡、卸载流关卡时会被 Destroy。
 * 
 * 2,APlayerState
 * 由 GameMode::Login() 主动 SpawnActor 生成 → 不与任何 ULevel 绑定，独立于地图存在。
 * 销毁权在 GameMode
 * 
 * 对象				|生命周期管理者		|地图切换	|死亡/卸载	|玩家重连
 * Character/Pawn	|World			  |销毁		  |销毁	 	 |重新生成
 * PlayerController	|GameMode		  |迁移		  |保留	 	 |重新关联
 * PlayerState		|GameMode		  |迁移		  |保留	 	 |原对象仍在
 * 
 */

DECLARE_MULTICAST_DELEGATE_OneParam(FOnplayStateChaned, int32)

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


	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<ULevelUpInfo> LevelUpInfo;

	FOnplayStateChaned OnXpChangedDelegate;
	FOnplayStateChaned OnLevelChangedDelegate;

	int32 GetPlayerLevel() const { return Level; }

	int32 GetXP() const { return XP; }

	void AddToXp(int32 InXP);
	void AddTolevel(int32 InLevel);

	void SetXP(int32 InXp);
	void SetLevel(int32 InLevel);

protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;

private:
	//如果只是使用了 DOREPLIFETIME而没有使用 ReplicatedUsing，那么当属性复制到客户端时，客户端会更新该属性的值，但不会触发任何回调函数。你只能通过直接访问该属性来获取更新后的值。
	UPROPERTY(VisibleAnywhere, ReplicatedUsing=OnRep_Level)
	int32 Level = 1;

	UPROPERTY(VisibleAnywhere, ReplicatedUsing=OnRep_XP)
	int32 XP = 1;

	UFUNCTION()
	void OnRep_Level(int32 OldValue);

	UFUNCTION()
	void OnRep_XP(int32 OldValue);
};
