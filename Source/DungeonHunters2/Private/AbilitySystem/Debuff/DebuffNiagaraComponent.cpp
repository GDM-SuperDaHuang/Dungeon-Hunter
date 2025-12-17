// daHuang


#include "AbilitySystem/Debuff/DebuffNiagaraComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Interaction/CombatInterface.h"

UDebuffNiagaraComponent::UDebuffNiagaraComponent()
{
	bAutoActivate = false;
	// Activate();
	// Deactivate();
}

void UDebuffNiagaraComponent::BeginPlay()
{
	Super::BeginPlay();
	// 1. 尝试直接拿 Owner 的 ASC
	ICombatInterface* CombatInterface = Cast<ICombatInterface>(GetOwner());
	UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwner());
	if (ASC)
	{
		//EGameplayTagEventType::NewOrRemoved:无论标签被添加还是移除都触发回调（即数量从 0→1 或 1→0 都会通知）。
		ASC->RegisterGameplayTagEvent(DebuffTag, EGameplayTagEventType::NewOrRemoved).AddUObject(
			this, &UDebuffNiagaraComponent::DebuffTagChanged);
	}
	else if (CombatInterface) // 2. 如果 ASC 还没就绪（ BeginPlay 时可能尚未初始化），则通过接口委托等待
	{
		CombatInterface->GetOnASCRegisteredDelegate().AddWeakLambda(this, [this](UAbilitySystemComponent* InASC)
		{
			InASC->RegisterGameplayTagEvent(DebuffTag, EGameplayTagEventType::NewOrRemoved).AddUObject(
				this, &UDebuffNiagaraComponent::DebuffTagChanged);
		});
	}
	if (CombatInterface)
	{
		CombatInterface->GetOnDeathDelegate().AddDynamic(this,&UDebuffNiagaraComponent::OnOwnerDeath);
	}
	
}

void UDebuffNiagaraComponent::DebuffTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	bool bIsValid = IsValid(GetOwner());
	bool bOwnerAlive = GetOwner()->Implements<UCombatInterface>()&&ICombatInterface::Execute_IsDead(GetOwner());
	
	if (NewCount > 0 && bIsValid && bOwnerAlive)//防止重复激活
	{
		Activate();// 播放，
	}
	else
	{
		Deactivate();
	}
}

void UDebuffNiagaraComponent::OnOwnerDeath(AActor* DeadActor)
{
	Deactivate();
}
