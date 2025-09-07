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

	//primary Attributes
	DOREPLIFETIME_CONDITION_NOTIFY(UAureAttributeSet, Strength, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAureAttributeSet, Intelligence, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAureAttributeSet, Resilience, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAureAttributeSet, Vigor, COND_None, REPNOTIFY_Always);
	
	//secondary Attributes
	DOREPLIFETIME_CONDITION_NOTIFY(UAureAttributeSet, Armor, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAureAttributeSet, ArmorPenetration, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAureAttributeSet, BlockChance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAureAttributeSet, CriticalHitChance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAureAttributeSet, CriticalHitDamage, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAureAttributeSet, CriticalHitResistance, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAureAttributeSet, HealthRegeneration, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAureAttributeSet, ManaRegeneration, COND_None, REPNOTIFY_Always);

	//vital Attributes
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
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxMana());
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
		// 限制Health在0到MaxHealth之间
		SetHealth(FMath::Clamp(GetHealth(), 0.0f, GetMaxHealth()));
	}

	if (Data.EvaluatedData.Attribute == GetManaAttribute())
	{
		SetMana(FMath::Clamp(GetMana(), 0.0f, GetMaxMana()));
	}

	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		UE_LOG(LogTemp, Warning, TEXT("MaxHealth=%f"), GetMaxHealth());
		UE_LOG(LogTemp, Warning, TEXT("Health=%f"), GetHealth());
		UE_LOG(LogTemp, Warning, TEXT("Magnitude=%f"), Data.EvaluatedData.Magnitude);
	}
	if (Data.EvaluatedData.Attribute == GetManaAttribute())
	{
		UE_LOG(LogTemp, Warning, TEXT("MaxMana=%f"), GetMaxMana());
		UE_LOG(LogTemp, Warning, TEXT("Mana=%f"), GetMana());
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

void UAureAttributeSet::OnRep_Strength(const FGameplayAttributeData& OldStrength) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAureAttributeSet, Strength, OldStrength);
}

void UAureAttributeSet::OnRep_Intelligence(const FGameplayAttributeData& OldIntelligence) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAureAttributeSet, Intelligence, OldIntelligence);
}

void UAureAttributeSet::OnRep_Resilience(const FGameplayAttributeData& OldResilience) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAureAttributeSet, Resilience, OldResilience);
}

void UAureAttributeSet::OnRep_Vigor(const FGameplayAttributeData& OldVigor) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAureAttributeSet, Vigor, OldVigor);
}

void UAureAttributeSet::OnRep_Armor(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAureAttributeSet, Armor, OldValue);
}

void UAureAttributeSet::OnRep_ArmorPenetration(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAureAttributeSet, ArmorPenetration, OldValue);
}

void UAureAttributeSet::OnRep_BlockChance(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAureAttributeSet, BlockChance, OldValue);
}

void UAureAttributeSet::OnRep_CriticalHitChance(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAureAttributeSet, CriticalHitChance, OldValue);
}

void UAureAttributeSet::OnRep_CriticalHitDamage(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAureAttributeSet, CriticalHitDamage, OldValue);
}

void UAureAttributeSet::OnRep_CriticalHitResistance(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAureAttributeSet, CriticalHitResistance, OldValue);
}

void UAureAttributeSet::OnRep_HealthRegeneration(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAureAttributeSet, HealthRegeneration, OldValue);
}

void UAureAttributeSet::OnRep_ManaRegeneration(const FGameplayAttributeData& OldValue) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAureAttributeSet, ManaRegeneration, OldValue);
}
