// daHuang

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "AuraCharacter.generated.h"

/**
 * 
 */
UCLASS()
class DUNGEONHUNTERS2_API AAuraCharacter : public AAuraCharacterBase
{
	GENERATED_BODY()

public:
	AAuraCharacter();
	// 服务器
	virtual void PossessedBy(AController* NewController) override;
	// 客户端
	virtual void OnRep_PlayerState() override;

	/**combat Interface*/
	virtual int32 GetPlayerLevel() override;

private:
	virtual void InitAbilityActorInfo() override;
};
