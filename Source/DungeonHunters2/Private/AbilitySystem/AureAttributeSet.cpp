// daHuang


#include "AbilitySystem/AureAttributeSet.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayEffectExtension.h"
#include "GameFramework/Character.h"
#include "Net/UnrealNetwork.h"

UAureAttributeSet::UAureAttributeSet()
{
	//属性宏产生
	InitHealth(100.f);
	InitMaxHealth(150.f);
	InitMana(100.f);
	InitMaxMana(150.f);
}

void UAureAttributeSet::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UAureAttributeSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAureAttributeSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAureAttributeSet, Mana, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAureAttributeSet, MaxMana, COND_None, REPNOTIFY_Always);
}
//执行时机是属性值即将被修改但尚未实际更新到 FGameplayAttributeData 之前
void UAureAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);
	if (Attribute == GetHealthAttribute())
	{
		// 限制Health在0到MaxHealth之间
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxHealth());
		// UE_LOG(LogTemp, Warning, TEXT("Health=%f"), NewValue);
	}
	if (Attribute == GetMaxHealthAttribute())
	{
		// UE_LOG(LogTemp, Warning, TEXT("MaxHealth=%f"), NewValue);
	}
	if (Attribute == GetManaAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMana());
		// UE_LOG(LogTemp, Warning, TEXT("Mana=%f"), NewValue);
	}
	if (Attribute == GetMaxManaAttribute())
	{
		// UE_LOG(LogTemp, Warning, TEXT("MaxMana=%f"), NewValue);
	}
}

void UAureAttributeSet::SetEffectProperties(const struct FGameplayEffectModCallbackData& Data, FEffectProperties& props) const
{
	// source = 
	props.EffectContextHandle = Data.EffectSpec.GetContext();
	props.SourceASC = props.EffectContextHandle.GetOriginalInstigatorAbilitySystemComponent();

	if (IsValid(props.SourceASC) && props.SourceASC->AbilityActorInfo.IsValid() && props.SourceASC->AbilityActorInfo->
		AvatarActor.
		IsValid())
	{
		props.SourceAvatarActor = props.SourceASC->AbilityActorInfo->AvatarActor.Get();
		props.SourceController = props.SourceASC->AbilityActorInfo->PlayerController.Get();
		if (props.SourceController == nullptr && props.SourceAvatarActor != nullptr)
		{
			if (const APawn* Pawn = Cast<APawn>(props.SourceAvatarActor))
			{
				props.SourceController = Pawn->GetController();
			}
		}
		if (props.SourceController != nullptr)
		{
			props.SourceCharacter = Cast<ACharacter>(props.SourceController->GetPawn());
		}
	}

	if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
	{
		props.TargetAvatarActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
		props.TargetController = Data.Target.AbilityActorInfo->PlayerController.Get();
		props.TargetCharacter = Cast<ACharacter>(props.TargetAvatarActor);
		props.TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(props.TargetAvatarActor);
	}
}

void UAureAttributeSet::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
	FEffectProperties Props;
	SetEffectProperties(Data, Props);// 解析效果的源和目标信息
	                                                                                                                                                                                                                                                                                                                                                                                                                                                         

	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		UE_LOG(LogTemp, Warning, TEXT("Health=%f"), GetMaxHealth());
		UE_LOG(LogTemp, Warning, TEXT("Magnitude=%f"), Data.EvaluatedData.Magnitude);
	}
}

void UAureAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAureAttributeSet, Health, OldHealth);
}

void UAureAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAureAttributeSet, MaxHealth, OldMaxHealth);
}

void UAureAttributeSet::OnRep_Mana(const FGameplayAttributeData& OldMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAureAttributeSet, Mana, OldMana);
}

void UAureAttributeSet::OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAureAttributeSet, MaxMana, OldMaxMana);
}
