// daHuang

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability/AuraBeamSpell.h"
#include "Electrocute.generated.h"

/**
 * 
 */
UCLASS()
class DUNGEONHUNTERS2_API UElectrocute : public UAuraBeamSpell
{
	GENERATED_BODY()
public:
	virtual FString GetDescription(int32 Level) override;
	virtual FString GetNextLevelDescription(int32 Level) override;

};
