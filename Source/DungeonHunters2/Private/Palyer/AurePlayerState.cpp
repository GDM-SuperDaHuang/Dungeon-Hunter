// daHuang


#include "Palyer/AurePlayerState.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/AureAbilitySystemComponent.h"
#include "AbilitySystem/AureAttributeSet.h"
#include "Net/UnrealNetwork.h"

AAurePlayerState::AAurePlayerState()
{
	//每秒更新100次
	NetUpdateFrequency=100.0f;
	// todo 
	AbilitySystemComponent=CreateDefaultSubobject<UAureAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);	//复制
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
	
	
	AttributeSet=CreateDefaultSubobject<UAureAttributeSet>("AttributeSet");
}

void AAurePlayerState::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(AAurePlayerState,Level);
}

UAbilitySystemComponent* AAurePlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AAurePlayerState::OnRep_Level()
{
	
}
