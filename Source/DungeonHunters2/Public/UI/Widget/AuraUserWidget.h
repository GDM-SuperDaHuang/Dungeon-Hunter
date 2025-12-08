// daHuang

#pragma once


#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AuraUserWidget.generated.h"                                                  

/**
 * 
 */
UCLASS()
class DUNGEONHUNTERS2_API UAuraUserWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	void SetWidgetController(UObject* InWidgetController);
	
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UObject> WidgetController;

protected:

	//蓝图实现 对应蓝图的 Event Widget Controller Set
	// 触发蓝图 Event Widget Controller Set 的回调
	UFUNCTION(BlueprintImplementableEvent)
	void WidgetControllerSet();
};
