// daHuang


#include "Palyer/AurePlayerState.h"

#include "AbilitySystemComponent.h"
#include "AttributeSet.h"

AAurePlayerState::AAurePlayerState()
{
	//每秒更新100次
	NetUpdateFrequency=100.0f;

	AbilitySystemComponent=CreateDefaultSubobject<UAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);	//复制
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
	
	
	AttributeSet=CreateDefaultSubobject<UAttributeSet>("AttributeSet");
}

UAbilitySystemComponent* AAurePlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}
