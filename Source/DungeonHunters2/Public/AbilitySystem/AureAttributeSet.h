// daHuang

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "AureAttributeSet.generated.h"

//生成getter、setter、Init 函数
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

DECLARE_DELEGATE_RetVal(FGameplayAttribute, FAttributeSignature);

USTRUCT()
struct FEffectProperties
{
	GENERATED_BODY()

	FEffectProperties()
	{
	}

	FGameplayEffectContextHandle EffectContextHandle; // 效果上下文句柄，存储效果元数据

	// 源（施加效果的对象）相关信息
	UPROPERTY()
	UAbilitySystemComponent* SourceASC; // 来源的能力系统组件
	UPROPERTY()
	AActor* SourceAvatarActor; // 来源的Avatar角色
	UPROPERTY()
	AController* SourceController; // 来源的控制器
	UPROPERTY()
	ACharacter* SourceCharacter; // 来源的角色

	// 目标（接收效果的对象）相关信息
	UPROPERTY()
	UAbilitySystemComponent* TargetASC; // 目标的能力系统组件
	UPROPERTY()
	AActor* TargetAvatarActor; // 目标的Avatar角色
	UPROPERTY()
	AController* TargetController; // 目标的控制器
	UPROPERTY()
	ACharacter* TargetCharacter; // 目标的角色
};

//方式一
//	TMap<FGameplayTag, FGameplayAttribute(*)()> TagsToAttributes;


//方式一
// typedef TBaseStaticDelegateInstance<FGameplayAttribute(), FDefaultDelegateUserPolicy>::FFuncPtr FAttributeFuncPtr;
//TMap<FGameplayTag, FAttributeFuncPtr> TagsToAttributes;

//方式三
template <class T>
using FAttributeFuncPtr = typename TBaseStaticDelegateInstance<T, FDefaultDelegateUserPolicy>::FFuncPtr;

/**
 * 
 */
UCLASS()
class DUNGEONHUNTERS2_API UAureAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UAureAttributeSet();

	// 网络复制支持：定义哪些属性需要复制到客户端
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;;

	// 执行时机是属性值即将被修改但尚未实际更新到 FGameplayAttributeData 之前
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	// 效果执行后的回调（后处理阶段）
	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;

	//方式4
	// TMap<FGameplayTag, TBaseStaticDelegateInstance<FGameplayAttribute(), FDefaultDelegateUserPolicy>::FFuncPtr> TagsToAttributes;
	TMap<FGameplayTag, FAttributeFuncPtr<FGameplayAttribute()>> TagsToAttributes;

	/*
	 * primary Attributes
	 */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Strength, Category="primary Attributes")
	FGameplayAttributeData Strength;
	ATTRIBUTE_ACCESSORS(UAureAttributeSet, Strength); //生成getter、setter函数

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Intelligence, Category="primary Attributes")
	FGameplayAttributeData Intelligence;
	ATTRIBUTE_ACCESSORS(UAureAttributeSet, Intelligence); //生成getter、setter函数

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Resilience, Category="primary Attributes")
	FGameplayAttributeData Resilience;
	ATTRIBUTE_ACCESSORS(UAureAttributeSet, Resilience); //生成getter、setter函数

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Vigor, Category="primary Attributes")
	FGameplayAttributeData Vigor;
	ATTRIBUTE_ACCESSORS(UAureAttributeSet, Vigor); //生成getter、setter函数

	/*
	 *  Attributes
	 */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Armor, Category="Secondary Attributes")
	FGameplayAttributeData Armor;
	ATTRIBUTE_ACCESSORS(UAureAttributeSet, Armor);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ArmorPenetration, Category="Secondary Attributes")
	FGameplayAttributeData ArmorPenetration;
	ATTRIBUTE_ACCESSORS(UAureAttributeSet, ArmorPenetration);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_BlockChance, Category="Secondary Attributes")
	FGameplayAttributeData BlockChance;
	ATTRIBUTE_ACCESSORS(UAureAttributeSet, BlockChance);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CriticalHitChance, Category="Secondary Attributes")
	FGameplayAttributeData CriticalHitChance;
	ATTRIBUTE_ACCESSORS(UAureAttributeSet, CriticalHitChance);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CriticalHitDamage, Category="Secondary Attributes")
	FGameplayAttributeData CriticalHitDamage;
	ATTRIBUTE_ACCESSORS(UAureAttributeSet, CriticalHitDamage);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CriticalHitResistance, Category="Secondary Attributes")
	FGameplayAttributeData CriticalHitResistance;
	ATTRIBUTE_ACCESSORS(UAureAttributeSet, CriticalHitResistance);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_HealthRegeneration, Category="Secondary Attributes")
	FGameplayAttributeData HealthRegeneration;
	ATTRIBUTE_ACCESSORS(UAureAttributeSet, HealthRegeneration);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ManaRegeneration, Category="Secondary Attributes")
	FGameplayAttributeData ManaRegeneration;
	ATTRIBUTE_ACCESSORS(UAureAttributeSet, ManaRegeneration);

	/*
	 *vital Attributes
	 */
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Health, Category="Vital Attributes")
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UAureAttributeSet, Health); //生成getter、setter函数

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxHealth, Category="Vital Attributes")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UAureAttributeSet, MaxHealth);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Mana, Category="Vital Attributes")
	FGameplayAttributeData Mana;
	ATTRIBUTE_ACCESSORS(UAureAttributeSet, Mana);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxMana, Category="Vital Attributes")
	FGameplayAttributeData MaxMana;
	ATTRIBUTE_ACCESSORS(UAureAttributeSet, MaxMana);

	/**
	 * meta Attributes
	 */
	UPROPERTY(BlueprintReadOnly, Category="Vital Attributes")
	FGameplayAttributeData IncomingDamage;
	ATTRIBUTE_ACCESSORS(UAureAttributeSet, IncomingDamage);

	/**
	 * call black
	 */
	/* ========== 网络复制回调 ========== */
	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldHealth) const;

	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const;

	UFUNCTION()
	void OnRep_Mana(const FGameplayAttributeData& OldMana) const;

	UFUNCTION()
	void OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const;

	UFUNCTION()
	void OnRep_Strength(const FGameplayAttributeData& OldStrength) const;

	UFUNCTION()
	void OnRep_Intelligence(const FGameplayAttributeData& OldIntelligence) const;

	UFUNCTION()
	void OnRep_Resilience(const FGameplayAttributeData& OldResilience) const;

	UFUNCTION()
	void OnRep_Vigor(const FGameplayAttributeData& OldVigor) const;

	UFUNCTION()
	void OnRep_Armor(const FGameplayAttributeData& OldValue) const;
	UFUNCTION()
	void OnRep_ArmorPenetration(const FGameplayAttributeData& OldValue) const;
	UFUNCTION()
	void OnRep_BlockChance(const FGameplayAttributeData& OldValue) const;
	UFUNCTION()
	void OnRep_CriticalHitChance(const FGameplayAttributeData& OldValue) const;
	UFUNCTION()
	void OnRep_CriticalHitDamage(const FGameplayAttributeData& OldValue) const;
	UFUNCTION()
	void OnRep_CriticalHitResistance(const FGameplayAttributeData& OldValue) const;
	UFUNCTION()
	void OnRep_HealthRegeneration(const FGameplayAttributeData& OldValue) const;
	UFUNCTION()
	void OnRep_ManaRegeneration(const FGameplayAttributeData& OldValue) const;

private:
	// 设置效果属性 - 提取效果相关的上下文信息
	void SetEffectProperties(const struct FGameplayEffectModCallbackData& Data, FEffectProperties& props) const;
};
