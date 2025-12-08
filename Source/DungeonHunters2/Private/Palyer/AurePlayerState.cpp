// daHuang


#include "Palyer/AurePlayerState.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/AureAbilitySystemComponent.h"
#include "AbilitySystem/AureAttributeSet.h"
#include "Net/UnrealNetwork.h"


AAurePlayerState::AAurePlayerState()
{
	//每秒更新100次
	NetUpdateFrequency = 100.0f;
	// 这里ASC最大扫描和UAureAttributeSet进行绑定
	AbilitySystemComponent = CreateDefaultSubobject<UAureAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true); //复制
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);


	AttributeSet = CreateDefaultSubobject<UAureAttributeSet>("AttributeSet");
}

void AAurePlayerState::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	/** 
	 * 第一个参数 AAurePlayerState是声明属性的类名。
	 * 第二个参数 Level是要复制的变量名。 
	 * 使用getLevel就能获取到最新的Level
	 */
	DOREPLIFETIME(AAurePlayerState, Level);
	DOREPLIFETIME(AAurePlayerState, XP);
	DOREPLIFETIME(AAurePlayerState, AttributePoints);
	DOREPLIFETIME(AAurePlayerState, SpellPoints);
}

UAbilitySystemComponent* AAurePlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AAurePlayerState::AddToXp(int32 InXP)
{
	XP += InXP;
	OnXpChangedDelegate.Broadcast(XP);
}

void AAurePlayerState::AddTolevel(int32 InLevel)
{
	Level += InLevel;
	OnLevelChangedDelegate.Broadcast(Level);
}

void AAurePlayerState::SetXP(int32 InXp)
{
	XP = InXp;
	OnXpChangedDelegate.Broadcast(XP);
}

void AAurePlayerState::SetLevel(int32 InLevel)
{
	Level = InLevel;
	OnLevelChangedDelegate.Broadcast(Level);
}

void AAurePlayerState::OnRep_Level(int32 OldValue)
{
	OnLevelChangedDelegate.Broadcast(Level);
}

void AAurePlayerState::OnRep_XP(int32 OldValue)
{
	OnXpChangedDelegate.Broadcast(XP);
}

void AAurePlayerState::OnRep_AttributePoints(int32 OldValue)
{
	OnAttributePointsChangedDelegate.Broadcast(OldValue);

}

void AAurePlayerState::OnRep_SpellPoints(int32 OldValue)
{
	OnSpellPointsChangedDelegate.Broadcast(OldValue);

}

void AAurePlayerState::AddToAttributePoints(int32 InAttributePoints)
{
	AttributePoints += InAttributePoints;
	OnAttributePointsChangedDelegate.Broadcast(InAttributePoints);
}

void AAurePlayerState::AddToSpellPoints(int32 InSpellPoints)
{
	SpellPoints += InSpellPoints;
	OnSpellPointsChangedDelegate.Broadcast(InSpellPoints);
}
