// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/AuraCharacterBase.h"
#include "AbilitySystemComponent.h"
#include "AuraGameplayTags.h"
#include "AbilitySystem/AureAbilitySystemComponent.h"
#include "Components/CapsuleComponent.h"
#include "DungeonHunters2/DungeonHunters2.h"
#include "Kismet/GameplayStatics.h"

AAuraCharacterBase::AAuraCharacterBase()
{
	// 不让角色 Tick，性能优化（子类需要再开）
	PrimaryActorTick.bCanEverTick = false;

	// === 碰撞设置 ==========================================================
	// 胶囊体对摄像机忽略，避免穿墙遮挡
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	//这会把角色胶囊体的 GenerateOverlapEvents 标记关掉，从此胶囊他不再触发重叠事件：
	//关闭胶囊体重叠事件，统一用网格体接收投射物
	GetCapsuleComponent()->SetGenerateOverlapEvents(false);
	// 网格体对摄像机忽略，对投射物只触发重叠
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Projectile, ECR_Overlap);
	GetMesh()->SetGenerateOverlapEvents(true);


	// === 武器组件 ===========================================================
	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");
	// 挂到右手插槽，可在子类改 Socket 名，注意名称 WeaponHandSocket 一定要一致
	Weapon->SetupAttachment(GetMesh(), FName(TEXT("WeaponHandSocket")));
	// 武器本身不产生物理碰撞
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

UAbilitySystemComponent* AAuraCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UAnimMontage* AAuraCharacterBase::GetHitReactMontage_Implementation()
{
	return HitReactMontage;
}

void AAuraCharacterBase::Die()
{
	Weapon->DetachFromComponent(FDetachmentTransformRules(EDetachmentRule::KeepWorld, true));
	MulticastHandleDeath_Implementation();
}

void AAuraCharacterBase::MulticastHandleDeath_Implementation()
{
	UGameplayStatics::PlaySoundAtLocation(this, DeathSound, GetActorLocation(), GetActorRotation());
	Weapon->SetSimulatePhysics(true);
	Weapon->SetEnableGravity(true);
	Weapon->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);

	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetEnableGravity(true);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	GetMesh()->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Dissolve();
	bDead = true;
}

void AAuraCharacterBase::BeginPlay()
{
	Super::BeginPlay();
}

// 供 GA 或 Task 调用的便捷函数：返回武器尖端位置
FVector AAuraCharacterBase::GetCombatSocketLocation_Implementation(const FGameplayTag& MontageTag)
{
	FAuraGameplayTags GameplayTags = FAuraGameplayTags::Get();
	if (MontageTag.MatchesTagExact(GameplayTags.CombatSocket_Weapon) && IsValid(Weapon))
	{
		return Weapon->GetSocketLocation(WeaponTipSocketName);
	}

	if (MontageTag.MatchesTagExact(GameplayTags.CombatSocket_RightHand))
	{
		return GetMesh()->GetSocketLocation(RightHandSocketName);
	}

	if (MontageTag.MatchesTagExact(GameplayTags.CombatSocket_LeftHand))
	{
		return Weapon->GetSocketLocation(LeftHandSocketName);
	}

	if (MontageTag.MatchesTagExact(GameplayTags.CombatSocket_Tail))
	{
		return Weapon->GetSocketLocation(TailSocketName);
	}

	return FVector::ZeroVector;
}

bool AAuraCharacterBase::IsDead_Implementation() const
{
	return bDead;
}

AActor* AAuraCharacterBase::GetAvatar_Implementation()
{
	return this;
}

TArray<FTaggedMontage> AAuraCharacterBase::GetAttackMontages_Implementation()
{
	return AttackMontages;
}

UNiagaraSystem* AAuraCharacterBase::GetBloodEffect_Implementation()
{
	return BloodEffect;
}

FTaggedMontage AAuraCharacterBase::GetTaggedMontageByTag_Implementation(const FGameplayTag& MontageTag)
{
	for (FTaggedMontage AttackMontage : AttackMontages)
	{
		if (AttackMontage.EventMontage == MontageTag)
		{
			return AttackMontage;
		}
	}
	return FTaggedMontage();
}

int32 AAuraCharacterBase::GetMinionCount_Implementation()
{
	return MinionCount;
}

void AAuraCharacterBase::IncrementMinionCount_Implementation(int32 Amount)
{
	MinionCount += Amount;
}


void AAuraCharacterBase::InitAbilityActorInfo()
{
}

/* 通用给自己上 GE 的工具；Level 参数留给将来成长 */
void AAuraCharacterBase::ApplyEffectToSelf(TSubclassOf<UGameplayEffect> GameplayEffectClass, float Leve) const
{
	check(IsValid(GetAbilitySystemComponent()));
	check(GameplayEffectClass);
	FGameplayEffectContextHandle ContextHandle = GetAbilitySystemComponent()->MakeEffectContext();
	ContextHandle.AddSourceObject(this);
	const FGameplayEffectSpecHandle SpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(
		GameplayEffectClass, Leve, ContextHandle);
	GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), GetAbilitySystemComponent());
}


/* 把默认三大 GE 刷到自己身上 —— 只在服务器执行 */
void AAuraCharacterBase::InitializeDefaultAbilities() const
{
	// todo 这里代码隔离优化，    if (!HasAuthority()) return;                     // ❌ 客户端直接返回
	ApplyEffectToSelf(DefaultPrimaryAbilityEffect, 1.0f);
	ApplyEffectToSelf(DefaultSecondaryAbilityEffect, 1.0f);
	ApplyEffectToSelf(DefaultVitalAbilityEffect, 1.0f);
}

/* 服务器给角色赋予初始技能（通常由 GameMode 在 Possess 后调用）*/
void AAuraCharacterBase::AddCharacterAbilities()
{
	// todo 可以有代码隔离优化 ， if (!HasAuthority()) return;                     // ❌ 防客户端误调
	UAureAbilitySystemComponent* AureASC = CastChecked<UAureAbilitySystemComponent>(AbilitySystemComponent);
	if (!HasAuthority()) return;
	AureASC->AddCharacterAbilities(StartUpAbility);
}

void AAuraCharacterBase::Dissolve()
{
	if (IsValid(DissolveMaterialInstance))
	{
		UMaterialInstanceDynamic* DynamicMatInst = UMaterialInstanceDynamic::Create(DissolveMaterialInstance, this);
		GetMesh()->SetMaterial(0, DynamicMatInst);
		StartDissolveTimeline(DynamicMatInst);
	}

	if (IsValid(WeaponDissolveMaterialInstance))
	{
		UMaterialInstanceDynamic* DynamicMatInst = UMaterialInstanceDynamic::Create(
			WeaponDissolveMaterialInstance, this);
		Weapon->SetMaterial(0, DynamicMatInst);
		StartWeaponDissolveTimeline(DynamicMatInst);
	}
}
