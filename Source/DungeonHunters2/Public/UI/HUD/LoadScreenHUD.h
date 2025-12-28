// daHuang

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "LoadScreenHUD.generated.h"

class UMVVM_LoadSreen;
class ULoadScreenWidget;
/**
 * 
 */
UCLASS()
class DUNGEONHUNTERS2_API ALoadScreenHUD : public AHUD
{
	GENERATED_BODY()
public:

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> LoadScreenWidgetClass;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<ULoadScreenWidget> LoadScreenWidget;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UMVVM_LoadSreen> LoadScreenViewModelClass;
	
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UMVVM_LoadSreen> LoadScreenViewModel;
protected:
	virtual void BeginPlay() override;
};
