// daHuang


#include "AbilitySystem/AuraAbilitySystemLibrary.h"

#include "Game/AuraGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Palyer/AurePlayerState.h"
#include "UI/HUD/AuraHUD.h"
#include "UI/WidgetController/AuraWidgetController.h"

UOverlayWidgetController* UAuraAbilitySystemLibrary::GetOverlayWidgetController(const UObject* WorldContextObject)
{
	/**
	 * 1. 拿到本地 PlayerController（索引 0 就是本地玩家）
	 * 编辑器 PIE 窗口数 > 1
	 * 索引 0 永远是第一个 PIE 窗口，如果你开 2 个 Client 想分别拿自己 HUD，需要把索引作为参数暴露或走 UWorld::GetFirstPlayerController 循环。
	 */
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		// 2. 把我们的 AuraHUD 薅出来
		if (AAuraHUD* AuraHUD = Cast<AAuraHUD>(PC->GetHUD()))
		{
			// 3. 下面几个指针 HUD 自己都有，但 UI 蓝图没有，所以一次性打包成 FWidgetControllerParams
			AAurePlayerState* PS = PC->GetPlayerState<AAurePlayerState>();
			UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
			UAttributeSet* AS = PS->GetAttributeSet();
			// 4. 构造参数包，扔进 HUD 的工厂函数 → 拿到控制器
			const FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);
			UOverlayWidgetController* OverlayWidgetController = AuraHUD->GetOverlayWidgetController(
				WidgetControllerParams);
			return OverlayWidgetController; // 可能为空，调用方自己判断
		}
	}
	// 失败场景：编辑器 Standalone 窗口没 Play、HUD 没初始化、PlayerState 没复制完……
	return nullptr;
}

UAttributeMenuWidgetController* UAuraAbilitySystemLibrary::GetAttributeMenuWidgetController(
	const UObject* WorldContextObject)
{
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		if (AAuraHUD* AuraHUD = Cast<AAuraHUD>(PC->GetHUD()))
		{
			AAurePlayerState* PS = PC->GetPlayerState<AAurePlayerState>();
			UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
			UAttributeSet* AS = PS->GetAttributeSet();
			const FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);
			UAttributeMenuWidgetController* OverlayWidgetController = AuraHUD->GetAttributeMenuWidgetController(
				WidgetControllerParams);
			return OverlayWidgetController;
		}
	}
	return nullptr;
}

void UAuraAbilitySystemLibrary::InitializeDefaultAttributes(const UObject* WorldContextObject,
                                                            ECharacterClass CharacterClass, float Level,
                                                            UAbilitySystemComponent* ASC)
{
	AAuraGameModeBase* AuraGameMode = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (AuraGameMode == nullptr) return;

	AActor* AvatarActor = ASC->GetAvatarActor();
	
	TObjectPtr<UCharacterClassInfo> CharacterClassInfo = AuraGameMode->CharacterClassInfo;
	FCharacterClassDefaultInfo ClassDefaultInfo = CharacterClassInfo->GetClassDefaultInfo(CharacterClass);

	FGameplayEffectContextHandle PrimaryAttributesContextHandle = ASC->MakeEffectContext();
	PrimaryAttributesContextHandle.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle PrimaryAttributesSpecHandle = ASC->MakeOutgoingSpec(ClassDefaultInfo.PrimaryAttributes,Level,PrimaryAttributesContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*PrimaryAttributesSpecHandle.Data.Get());

	FGameplayEffectContextHandle SecondaryAttributesContextHandle = ASC->MakeEffectContext();
	SecondaryAttributesContextHandle.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle SecondaryAttributesSpecHandle = ASC->MakeOutgoingSpec(CharacterClassInfo->SecondaryAttributes,Level,SecondaryAttributesContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*SecondaryAttributesSpecHandle.Data.Get());

	FGameplayEffectContextHandle VitalAttributesContextHandle = ASC->MakeEffectContext();
	VitalAttributesContextHandle.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle VitalAttributesSpecHandle = ASC->MakeOutgoingSpec(CharacterClassInfo->VitalAttributes,Level,VitalAttributesContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*VitalAttributesSpecHandle.Data.Get());
	
}

void UAuraAbilitySystemLibrary::GiveStartupAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* ASC)
{
	AAuraGameModeBase* AuraGameMode = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (AuraGameMode == nullptr) return;

	TObjectPtr<UCharacterClassInfo> CharacterClassInfo = AuraGameMode->CharacterClassInfo;
	for (TSubclassOf<UGameplayAbility> AbilityClass : CharacterClassInfo->CommonAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
		ASC->GiveAbility(AbilitySpec);
	}

}
