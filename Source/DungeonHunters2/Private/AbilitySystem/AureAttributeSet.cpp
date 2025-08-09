// daHuang


#include "AbilitySystem/AureAttributeSet.h"
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
	
	DOREPLIFETIME_CONDITION_NOTIFY(UAureAttributeSet,Health,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAureAttributeSet,MaxHealth,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAureAttributeSet,Mana,COND_None,REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UAureAttributeSet,MaxMana,COND_None,REPNOTIFY_Always);

}

void UAureAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAureAttributeSet,Health,OldHealth);
}

void UAureAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAureAttributeSet,MaxHealth,OldMaxHealth);
}

void UAureAttributeSet::OnRep_Mana(const FGameplayAttributeData& OldMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAureAttributeSet,Mana,OldMana);
}

void UAureAttributeSet::OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UAureAttributeSet,MaxMana,OldMaxMana);

}

