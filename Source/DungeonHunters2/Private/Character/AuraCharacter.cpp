// daHuang


#include "Character/AuraCharacter.h"

#include "AbilitySystemComponent.h"
#include "AuraGameplayTags.h"
#include "NiagaraComponent.h"
#include "AbilitySystem/AureAbilitySystemComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Palyer/AuraPlayerController.h"
#include "AbilitySystem/Data/LevelUpInfo.h"
#include "AbilitySystem/Debuff/DebuffNiagaraComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Palyer/AurePlayerState.h"
#include "UI/HUD/AuraHUD.h"

AAuraCharacter::AAuraCharacter()
{
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->SetUsingAbsoluteRotation(true);
	CameraBoom->bDoCollisionTest = false;

	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>("TopDownCameraComponent");
	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false;

	LevelNiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>("LevelNiagaraComponent");
	LevelNiagaraComponent->SetupAttachment(GetRootComponent());
	LevelNiagaraComponent->bAutoActivate = false;

	// === 移动组件调参（Diablo-Like 平面移动）==========================
	GetCharacterMovement()->bOrientRotationToMovement = true; // 朝向速度方向
	GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true; // 锁 XY 平面
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	// 不让控制器控制旋转，由移动组件全权负责
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	CharacterClass = ECharacterClass::Elementalist;
}

void AAuraCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// 服务器端初始化 ASC 与属性
	InitAbilityActorInfo();
	AddCharacterAbilities(); // 给予默认技能
}

void AAuraCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	//初始化 到客户端
	InitAbilityActorInfo();
}

void AAuraCharacter::AddToXP_Implementation(int32 InXP)
{
	// 玩家等级存于 PlayerState，方便复制
	AAurePlayerState* AurePlayerState = GetPlayerState<AAurePlayerState>();
	check(AurePlayerState);
	AurePlayerState->AddToXp(InXP);
}

void AAuraCharacter::LevelUp_Implementation()
{
	MulticastLevelUpParticles_Implementation();
}

void AAuraCharacter::MulticastLevelUpParticles_Implementation() const
{
	if (IsValid(LevelNiagaraComponent))
	{
		const FVector CameraLocation = TopDownCameraComponent->GetComponentLocation();
		const FVector NiagaraLocation = LevelNiagaraComponent->GetComponentLocation();
		UE::Math::TRotator<double> ToCameraRotation = (CameraLocation - NiagaraLocation).Rotation();
		LevelNiagaraComponent->SetWorldRotation(ToCameraRotation);
		LevelNiagaraComponent->Activate(true);
	}
}


int32 AAuraCharacter::GetXP_Implementation() const
{
	// 玩家等级存于 PlayerState，方便复制
	AAurePlayerState* AurePlayerState = GetPlayerState<AAurePlayerState>();
	check(AurePlayerState);
	return AurePlayerState->GetXP();
}

int32 AAuraCharacter::FindLevelForXP_Implementation(int32 InXP) const
{
	AAurePlayerState* AurePlayerState = GetPlayerState<AAurePlayerState>();
	check(AurePlayerState);
	return AurePlayerState->LevelUpInfo->FindLevelForXP(InXP);
}

int32 AAuraCharacter::GetAttributePointsReward_Implementation(int32 Level) const
{
	AAurePlayerState* AurePlayerState = GetPlayerState<AAurePlayerState>();
	check(AurePlayerState);
	return AurePlayerState->LevelUpInfo->LevelUpInformation[Level].AttributePointAward;
}

int32 AAuraCharacter::GetSpellPointsReward_Implementation(int32 Level) const
{
	AAurePlayerState* AurePlayerState = GetPlayerState<AAurePlayerState>();
	check(AurePlayerState);
	return AurePlayerState->LevelUpInfo->LevelUpInformation[Level].SpellPointAward;
}

void AAuraCharacter::AddToPlayerLevel_Implementation(int32 InPlayLevel)
{
	AAurePlayerState* AurePlayerState = GetPlayerState<AAurePlayerState>();
	check(AurePlayerState);
	AurePlayerState->AddTolevel(InPlayLevel);

	if (UAureAbilitySystemComponent* AureASC = Cast<UAureAbilitySystemComponent>(GetAbilitySystemComponent()))
	{
		AureASC->UpdateAbilityStatuses(AurePlayerState->GetPlayerLevel());
	}
}

void AAuraCharacter::AddToAttributePoints_Implementation(int32 InAttributePoints)
{
	AAurePlayerState* AurePlayerState = GetPlayerState<AAurePlayerState>();
	check(AurePlayerState);
	AurePlayerState->AddToAttributePoints(InAttributePoints);
}

void AAuraCharacter::AddToSpellPoints_Implementation(int32 InSpellPoints)
{
	AAurePlayerState* AurePlayerState = GetPlayerState<AAurePlayerState>();
	check(AurePlayerState);
	AurePlayerState->AddToSpellPoints(InSpellPoints);
}

int32 AAuraCharacter::GetAttributePoints_Implementation() const
{
	AAurePlayerState* AurePlayerState = GetPlayerState<AAurePlayerState>();
	check(AurePlayerState);
	return AurePlayerState->GetAttributePoints();
}

int32 AAuraCharacter::GetSpellPoints_Implementation() const
{
	AAurePlayerState* AurePlayerState = GetPlayerState<AAurePlayerState>();
	check(AurePlayerState);
	return AurePlayerState->GetSpellPoints();
}

void AAuraCharacter::ShowMagicCircle_Implementation(UMaterialInterface* DecalMaterial)
{
	if (AAuraPlayerController* AuraPlayerController = Cast<AAuraPlayerController>(GetController()))
	{
		AuraPlayerController->ShowMagicCircle(DecalMaterial);
		AuraPlayerController->bShowMouseCursor = false;
	}
}

void AAuraCharacter::HideMagicCircle_Implementation()
{
	if (AAuraPlayerController* AuraPlayerController = Cast<AAuraPlayerController>(GetController()))
	{
		AuraPlayerController->HideMagicCircle();
		AuraPlayerController->bShowMouseCursor = true;
	}
}

/* ========== 等级接口 ========== */
int32 AAuraCharacter::GetPlayerLevel_Implementation()
{
	// 玩家等级存于 PlayerState，方便复制
	AAurePlayerState* AurePlayerState = GetPlayerState<AAurePlayerState>();
	check(AurePlayerState);
	return AurePlayerState->GetPlayerLevel();
}

void AAuraCharacter::OnRep_Stunned()
{
	if (UAureAbilitySystemComponent* AuraASC = Cast<UAureAbilitySystemComponent>(AbilitySystemComponent))
	{
		const FAuraGameplayTags& GameplayTags = FAuraGameplayTags::Get();
		FGameplayTagContainer BlockedTags;
		BlockedTags.AddTag(GameplayTags.Play_Block_InputPressed);
		BlockedTags.AddTag(GameplayTags.Play_Block_InputHeld);
		BlockedTags.AddTag(GameplayTags.Play_Block_InputReleased);
		BlockedTags.AddTag(GameplayTags.Play_Block_CursorTrace);
		if (bIsStunned)
		{
			AuraASC->AddLooseGameplayTags(BlockedTags);
			StunDebuffComponent->Activate();
		}
		else
		{
			AuraASC->RemoveLooseGameplayTags(BlockedTags);
			StunDebuffComponent->Deactivate();
		}
	}
}

void AAuraCharacter::OnRep_Burned()
{
	if (bIsBurned)
	{
		BurnDebuffComponent->Activate();
	}
	else
	{
		BurnDebuffComponent->Deactivate();
	}
}

/* ========== ASC 初始化 ========== */
void AAuraCharacter::InitAbilityActorInfo()
{
	// 1. 拿到 PlayerState 里的 ASC & AttributeSet
	AAurePlayerState* AurePlayerState = GetPlayerState<AAurePlayerState>();
	check(AurePlayerState);

	/***
	 * 2. 绑定 Owner = PS，Avatar = 自身 Character
	 * 		Owner = PlayerState（持久存在）
	 *	 	Avatar = Character（可能死亡/换图）
	 * 1，将 AurePlayerState和this 中的AttributeSet 和 AbilitySystemComponent 进行绑定
	 *  传入了PlayerState（作为OwnerActor）
	 *  当前角色this（作为AvatarActor）
	 * 2,这时使用GAS修改属性会触发回调 PreAttributeChange，PostGameplayEffectExecute。
	 */
	AurePlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(AurePlayerState, this);

	// 4. 通知 ASC 内部完成初始化（注册回调、广播 AssetTag）
	UAureAbilitySystemComponent* AureASC = Cast<UAureAbilitySystemComponent>(
		AurePlayerState->GetAbilitySystemComponent());
	// check(AureASC); // 验证转换结果，确保非空
	AureASC->AbilityActorInfoSet();
	AbilitySystemComponent = AurePlayerState->GetAbilitySystemComponent();
	AttributeSet = AurePlayerState->GetAttributeSet();
	OnASCRegistered.Broadcast(AbilitySystemComponent);
	AbilitySystemComponent->RegisterGameplayTagEvent(FAuraGameplayTags::Get().Debuff_Stun, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &AAuraCharacter::StunTagChanged);

	// 5. 初始化 HUD（仅本地玩家）
	if (AAuraPlayerController* AuraPlayerController = Cast<AAuraPlayerController>(GetController()))
	{
		if (AAuraHUD* AuraHUD = Cast<AAuraHUD>(AuraPlayerController->GetHUD()))
		{
			AuraHUD->InitOverlay(AuraPlayerController, AurePlayerState, AbilitySystemComponent, AttributeSet);
		}
	}

	// 6. 刷默认属性 GE（服务器才执行）
	InitializeDefaultAbilities();
}
