// daHuang

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "EnenmyInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UEnenmyInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class DUNGEONHUNTERS2_API IEnenmyInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	//敌人选中后高光
	virtual void HighlightActor() = 0;
	//取消敌人选中后高光
	virtual void UnHighlightActor() = 0;

};
