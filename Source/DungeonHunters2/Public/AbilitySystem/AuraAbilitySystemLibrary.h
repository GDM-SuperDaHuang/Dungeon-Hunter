// daHuang

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "Data/CharacterClassInfo.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AuraAbilitySystemLibrary.generated.h"

struct FDamageEffectParams;
class UAbilityInfo;
class AAuraHUD;
struct FWidgetControllerParams;
class USpellMenuWidgetController;
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
	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|WidgetController",
		meta=(DefaultToSelf = "WorldContextObject"))
	static bool MakeWidgetControllerParams(const UObject* WorldContextObject,
	                                       FWidgetControllerParams& OutWCParams, AAuraHUD*& OutAuraHud);


	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|WidgetController",
		meta=(DefaultToSelf = "WorldContextObject"))
	static UOverlayWidgetController* GetOverlayWidgetController(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|WidgetController",
		meta=(DefaultToSelf = "WorldContextObject"))
	static UAttributeMenuWidgetController* GetAttributeMenuWidgetController(const UObject* WorldContextObject);


	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|WidgetController",
		meta=(DefaultToSelf = "WorldContextObject"))
	static USpellMenuWidgetController* GetSpellMenuWidgetController(const UObject* WorldContextObject);


	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|CharacterDefaultAttributes")
	static void InitializeDefaultAttributes(const UObject* WorldContextObject, ECharacterClass CharacterClass,
	                                        float Level, UAbilitySystemComponent* ASC);

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|CharacterDefaultAttributes")
	static void GiveStartupAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* ASC,
	                                 ECharacterClass CharacterClass);


	/**
	 * 
	 */
	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|CharacterDefaultAttributes")
	static UCharacterClassInfo* GetCharacterClassInfo(const UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|CharacterDefaultAttributes")
	static UAbilityInfo* GetAbilityInfo(const UObject* WorldContextObject);


	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|GameplayEffects")
	static bool IsBlockedHit(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|GameplayEffects")
	static bool IsSuccessfulDebuff(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|GameplayEffects")
	static float GetDebuffDamage(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|GameplayEffects")
	static float GetDebuffDuration(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|GameplayEffects")
	static float GetDebuffFrequency(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|GameplayEffects")
	static FGameplayTag GetDamageType(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|GameplayEffects")
	static FVector GetDeathImpulse(UPARAM(ref)
		const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|GameplayEffects")
	static FVector GetKnockbackForce(UPARAM(ref)
		const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|GameplayEffects")
	static bool IsCriticalHit(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|GameplayEffects")
	static bool IsRadialDamage(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|GameplayEffects")
	static float GetRadialDamageInnerRadius(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|GameplayEffects")
	static float GetRadialDamageOuterRadius(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|GameplayEffects")
	static FVector GetRadialDamageOrigin(const FGameplayEffectContextHandle& EffectContextHandle);


	/*
	* Effect Context Setters
	*/


	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|GameplayEffects")
	static void SetBlockedHit(UPARAM(ref)
	                          FGameplayEffectContextHandle& EffectContextHandle, bool bInIsBlockedHit);

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|GameplayEffects")
	static void SetCriticalHit(UPARAM(ref)
	                           FGameplayEffectContextHandle& EffectContextHandle, bool bInIsCriticalHit);


	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|GameplayEffects")
	static void SetIsSuccessfulDebuff(UPARAM(ref)
	                                  FGameplayEffectContextHandle& EffectContextHandle, bool bInIsSuccessfulDebuff);

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|GameplayEffects")
	static void SetDebuffDamage(UPARAM(ref)
	                            FGameplayEffectContextHandle& EffectContextHandle, float InDebuffDamage);

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|GameplayEffects")
	static void SetDebuffDuration(UPARAM(ref)
	                              FGameplayEffectContextHandle& EffectContextHandle, float InDebuffDuration);

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|GameplayEffects")
	static void SetDebuffFrequency(UPARAM(ref)
	                               FGameplayEffectContextHandle& EffectContextHandle, float InDebuffFrequency);

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|GameplayEffects")
	static void SetDamageType(UPARAM(ref)
	                          FGameplayEffectContextHandle& EffectContextHandle, const FGameplayTag& InDamageType);


	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|GameplayEffects")
	static void SetDeathImpulse(UPARAM(ref)
	                            FGameplayEffectContextHandle& EffectContextHandle, const FVector& InDeathImpulse);


	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|GameplayEffects")
	static void SetKnockbackForce(UPARAM(ref)
	                              FGameplayEffectContextHandle& EffectContextHandle, const FVector& InKnockbackForce);


	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|GameplayEffects")
	static void SetIsRadialDamage(UPARAM(ref)
	                              FGameplayEffectContextHandle& EffectContextHandle, bool bInIsRadialDamage);

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|GameplayEffects")
	static void SetRadialDamageInnerRadius(UPARAM(ref)
	                                       FGameplayEffectContextHandle& EffectContextHandle, float InInnerRadius);

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|GameplayEffects")
	static void SetRadialDamageOuterRadius(UPARAM(ref)
	                                       FGameplayEffectContextHandle& EffectContextHandle, float InOuterRadius);

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|GameplayEffects")
	static void SetRadialDamageOrigin(UPARAM(ref)
	                                  FGameplayEffectContextHandle& EffectContextHandle, const FVector& InOrigin);


	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|GameplayMechanics")
	static void GetLivePlayersWithinRadius(const UObject* WorldContextObject,
	                                       TArray<AActor*>& OutOverlappingActors,
	                                       const TArray<AActor*>& ActorsToIgnore,
	                                       float Radius,
	                                       const FVector& SphereOrigin);

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|GameplayMechanics")
	static void GetClosestTargets(int32 MaxTargets, const TArray<AActor*>& Actors, TArray<AActor*>& OutClosestTargets,
	                              const FVector& Origin);


	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|GameplayMechanics")
	static bool IsNotFriend(AActor* FirstActor, AActor* SecondActor);


	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|DamageEffect")
	static FGameplayEffectContextHandle ApplyDamageEffect(const FDamageEffectParams& DamageEffectParams);

	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|GameplayMechanics")
	static TArray<FRotator> EvenlySpacedRotators(const FVector& Forward, const FVector& Axis, float Spread,
	                                             int32 NumRotators);

	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|GameplayMechanics")
	static TArray<FVector> EvenlySpacedVector(const FVector& Forward, const FVector& Axis, float Spread,
	                                          int32 NumVectors);


	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|GameplayMechanics")
	static int32 GetXPRewardForClassAndLevel(
		const UObject* WorldContextObject,
		ECharacterClass CharacterClass,
		int32 CharacterLevel);


	/*
	 * Damage Effect Params
	 */

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|DamageEffect")
	static void SetIsRadialDamageEffectParam(UPARAM(ref) FDamageEffectParams& DamageEffectParams, bool bIsRadial, float InnerRadius, float OuterRadius, FVector Origin);

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|DamageEffect")
	static void SetKnockbackDirection(UPARAM(ref) FDamageEffectParams& DamageEffectParams, FVector KnockbackDirection, float Magnitude = 0.f);
	
	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|DamageEffect")
	static void SetDeathImpulseDirection(UPARAM(ref) FDamageEffectParams& DamageEffectParams, FVector ImpulseDirection, float Magnitude = 0.f);

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|DamageEffect")
	static void SetTargetEffectParamsASC(UPARAM(ref) FDamageEffectParams& DamageEffectParams, UAbilitySystemComponent* InASC);
};
