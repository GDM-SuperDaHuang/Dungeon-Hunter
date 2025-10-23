// daHuang

#pragma once

#include "CoreMinimal.h"
#include "Data/CharacterClassInfo.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AuraAbilitySystemLibrary.generated.h"

class UAbilitySystemComponent;
class UOverlayWidgetController;
class UAttributeMenuWidgetController;
/*
 * 工具库惯例：只放静态函数，无成员变量，任何地方都能调用。
 * 本文件专门给“蓝图/UMG 拿不到但又常用”的 GAS 对象提供一站式获取接口。
 * 例如：
 *   - UI 想拿 OverlayWidgetController，但 HUD 指针在 C++ 里，蓝图摸不到 → 用本库函数。
 *   - 未来想拿 AttributeMenuWidgetController、Effect 历史、GameplayCue 管理器……都往这里堆。
 */
UCLASS()
class DUNGEONHUNTERS2_API UAuraAbilitySystemLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	
	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|WidgetController")
	static UOverlayWidgetController* GetOverlayWidgetController(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|WidgetController")
	static UAttributeMenuWidgetController* GetAttributeMenuWidgetController(const UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|CharacterDefaultAttributes")
	static void InitializeDefaultAttributes(const UObject* WorldContextObject,ECharacterClass CharacterClass,float Level,UAbilitySystemComponent* ASC);
};
