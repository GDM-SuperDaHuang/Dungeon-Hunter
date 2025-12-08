// daHuang

#pragma once
#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "GameFramework/HUD.h"
#include "AuraHUD.generated.h"

class USpellMenuWidgetController;
class UAttributeMenuWidgetController;
class UOverlayWidgetController;
class UAuraUserWidget;
struct FWidgetControllerParams;
/**
 * 
 */
UCLASS()
class DUNGEONHUNTERS2_API AAuraHUD : public AHUD
{
	GENERATED_BODY()

public:
	// UPROPERTY()
	// TObjectPtr<UAuraUserWidget> OverlayWidget;

	// 第一次调用时 NewObject 创建 Overlay 控制器并绑定依赖；后续直接返回同一实例
	UOverlayWidgetController* GetOverlayWidgetController(const FWidgetControllerParams& WCParams);

	// 属性面板控制器的工厂函数，逻辑同上
	UAttributeMenuWidgetController* GetAttributeMenuWidgetController(const FWidgetControllerParams& WCParams);
	// 第一次调用时 NewObject 创建 Overlay 控制器并绑定依赖；后续直接返回同一实例
	USpellMenuWidgetController* GetSpellMenuWidgetController(const FWidgetControllerParams& WCParams);

	
	
	/* -------------------- 真正把 UI 塞进屏幕 -------------------- */
    // 由 AuraCharacter 在客户端初始化完毕后手动调用
    // 负责：1. CreateWidget  2. 绑定控制器  3. 广播初始值  4. AddToViewport
	void  InitOverlay(APlayerController* PC,APlayerState* PS,UAbilitySystemComponent* ASC,UAttributeSet* AS);
protected:

private:

 	/* -------------------- 原始 UI 实例 -------------------- */
    // 蓝图里拖的 WBP_Overlay 实例（血条/蓝条/等级等）
	UPROPERTY()
	TObjectPtr<UAuraUserWidget> OverlayWidget;
	
	/* -------------------- 蓝图类指定 -------------------- */
    // 指定用哪个蓝图/CPP 子类来创建 OverlayWidget
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> OverlayWidgetClass;


	/* -------------------- 控制器实例 -------------------- */
    // Overlay 控制器实例（单例，HUD 级生命周期）
	UPROPERTY()
	TObjectPtr<UOverlayWidgetController> OverlayWidgetController;

	//只能存储 UOverlayWidgetController 类或其蓝图子类的类型信息，而不是实例。
	//增加灵活性可以处理不同子类的角色类型创建不同的 UOverlayWidgetController 子类
	UPROPERTY(EditAnywhere)
	TSubclassOf<UOverlayWidgetController> OverlayWidgetControllerClass;

	/* -------------------- 属性面板控制器 -------------------- */
	UPROPERTY()
	TObjectPtr<UAttributeMenuWidgetController> AttributeMenuWidgetController;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UAttributeMenuWidgetController> UAttributeMenuWidgetControllerClass;

	/* -------------------- 属性面板控制器 -------------------- */
	UPROPERTY()
	TObjectPtr<USpellMenuWidgetController> SpellMenuWidgetController;

	UPROPERTY(EditAnywhere)
	TSubclassOf<USpellMenuWidgetController> SpellMenuWidgetControllerClass;

};
