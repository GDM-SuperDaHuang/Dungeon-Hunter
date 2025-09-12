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

private:
	virtual void InitAbilityActorInfo() override;
};
