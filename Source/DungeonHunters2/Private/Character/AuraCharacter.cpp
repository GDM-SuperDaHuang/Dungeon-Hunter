// daHuang


#include "Character/AuraCharacter.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/AureAbilitySystemComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Palyer/AuraPlayerController.h"
#include "Palyer/AurePlayerState.h"
#include "UI/HUD/AuraHUD.h"

AAuraCharacter::AAuraCharacter()
{
	// === 移动组件调参（Diablo-Like 平面移动）==========================
	GetCharacterMovement()->bOrientRotationToMovement = true; // 朝向速度方向
	GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true; // 锁 XY 平面
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	// 不让控制器控制旋转，由移动组件全权负责
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
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

/* ========== 等级接口 ========== */
int32 AAuraCharacter::GetPlayerLevel()
{
	// 玩家等级存于 PlayerState，方便复制
	AAurePlayerState* AurePlayerState = GetPlayerState<AAurePlayerState>();
	check(AurePlayerState);
	return AurePlayerState->GetPlayerLevel();
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
	UAureAbilitySystemComponent* AureASC = Cast<UAureAbilitySystemComponent>(AurePlayerState->GetAbilitySystemComponent());
	// check(AureASC); // 验证转换结果，确保非空
	AureASC->AbilityActorInfoSet();
	AbilitySystemComponent = AurePlayerState->GetAbilitySystemComponent();
	AttributeSet = AurePlayerState->GetAttributeSet();
	
	// 5. 初始化 HUD（仅本地玩家）
	if (AAuraPlayerController* AuraPlayerController = Cast<AAuraPlayerController>(GetController()))
	{
		if (AAuraHUD* AuraHUD = Cast<AAuraHUD>(AuraPlayerController->GetHUD()))
		{
			AuraHUD->InitOverlay(AuraPlayerController,AurePlayerState,AbilitySystemComponent,AttributeSet);
		}
		
	}

	// 6. 刷默认属性 GE（服务器才执行）
	InitializeDefaultAbilities();
}
