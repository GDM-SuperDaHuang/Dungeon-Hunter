// daHuang


#include "UI/HUD/AuraHUD.h"

#include "Blueprint/UserWidget.h"
#include "UI/Widget/AuraUserWidget.h"
#include "UI/WidgetController/AttributeMenuWidgetController.h"
#include "UI/WidgetController/OverlayWidgetController.h"

/* ===================================================================
 *  工厂函数：第一次调用时创建控制器并绑定依赖，后续直接返回单例
 * =================================================================== */
UOverlayWidgetController* AAuraHUD::GetOverlayWidgetController(const FWidgetControllerParams& WCParams)
{
	if (OverlayWidgetController == nullptr)
	{

		// 根据蓝图/CPP 子类 new 出控制器实例，Outer 设为 HUD，生命周期跟随 HUD
		// 极其不推荐的写法：硬编码蓝图路径
		// UClass* BlueprintClass = LoadClass<UOverlayWidgetController>(nullptr, TEXT("/Game/Blueprints/BP_CustomOverlayController.BP_CustomOverlayController_C"));
		// OverlayWidgetController = NewObject<UOverlayWidgetController>(this, BlueprintClass);
		// 极其不推荐的写法：硬编码蓝图路径
		OverlayWidgetController = NewObject<UOverlayWidgetController>(this, OverlayWidgetControllerClass);

		// 把 PC/PS/ASC/AS 一次性塞进控制器
		OverlayWidgetController->SetWidgetControllerParams(WCParams);

		// 立即绑定 ASC 属性委托 → UI 事件，避免漏掉后续属性变化
		OverlayWidgetController->BindCallbacksToDependencies();
	}
	return OverlayWidgetController;
}

/* ===================================================================
 *  属性面板控制器工厂，逻辑与 Overlay 完全一致
 * =================================================================== */
UAttributeMenuWidgetController* AAuraHUD::GetAttributeMenuWidgetController(const FWidgetControllerParams& WCParams)
{
	if (AttributeMenuWidgetController == nullptr)
	{
		AttributeMenuWidgetController = NewObject<UAttributeMenuWidgetController>(this, UAttributeMenuWidgetControllerClass);
		AttributeMenuWidgetController->SetWidgetControllerParams(WCParams);
		AttributeMenuWidgetController->BindCallbacksToDependencies();
	}
	return AttributeMenuWidgetController;
}

/* ===================================================================
 *  真正把 UI 塞进屏幕的入口
 *  由 AAuraCharacter::InitAbilityActorInfo() （客户端）手动调用
 * =================================================================== */
void AAuraHUD::InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS)
{
	// 防御性检查：蓝图中必须拖好 Widget 类
	checkf(OverlayWidgetClass, TEXT("OverlayWidgetClass unintialized"));
	checkf(OverlayWidgetControllerClass, TEXT("OverlayWidgetControllerClass unintialized"));

	/* ---------- 1. 创建原始 UUserWidget ---------- */
	UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), OverlayWidgetClass);
	OverlayWidget = Cast<UAuraUserWidget>(Widget);

	/* ---------- 2. 打包参数，拿到控制器单例 ---------- */
	const FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);
	UOverlayWidgetController* WidgetController = GetOverlayWidgetController(WidgetControllerParams);

	/* ---------- 3. 把控制器回写给 Widget，蓝图可立即使用 ---------- */
	OverlayWidget->SetWidgetController(WidgetController);
	/* ---------- 4. 把当前属性值一次性推给 UI，解决“打开 UI 时属性还没同步”问题 ---------- */
	WidgetController->BroadcastInitialValues();


	/* ---------- 5. 真正显示到屏幕 ---------- */
	Widget->AddToViewport();
}
