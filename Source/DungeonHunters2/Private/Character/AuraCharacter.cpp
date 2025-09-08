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
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
}

void AAuraCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	//初始化 到服务器
	InitAbilityActorInfo();
}

void AAuraCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	//初始化 到客户端
	InitAbilityActorInfo();
}

int32 AAuraCharacter::GetPlayerLevel()
{
	AAurePlayerState* AurePlayerState = GetPlayerState<AAurePlayerState>();
	check(AurePlayerState);
	return AurePlayerState->GetPlayerLevel();
}

void AAuraCharacter::InitAbilityActorInfo()
{
	AAurePlayerState* AurePlayerState = GetPlayerState<AAurePlayerState>();
	check(AurePlayerState);

	/***
	 * 1，将 AurePlayerState和this 中的AttributeSet 和 AbilitySystemComponent 进行绑定
	 * 传入了PlayerState（作为OwnerActor）
	 * 当前角色this（作为AvatarActor）
	 * 2,这时使用GAS修改属性会触发回调 PreAttributeChange，PostGameplayEffectExecute。
	 */

	AurePlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(AurePlayerState, this);
	UAureAbilitySystemComponent* AureASC = Cast<UAureAbilitySystemComponent>(AurePlayerState->GetAbilitySystemComponent());
	// check(AureASC); // 验证转换结果，确保非空
	AureASC->AbilityActorInfoSet();
	AbilitySystemComponent = AurePlayerState->GetAbilitySystemComponent();
	AttributeSet = AurePlayerState->GetAttributeSet();
	
	if (AAuraPlayerController* AuraPlayerController = Cast<AAuraPlayerController>(GetController()))
	{
		if (AAuraHUD* AuraHUD = Cast<AAuraHUD>(AuraPlayerController->GetHUD()))
		{
			AuraHUD->InitOverlay(AuraPlayerController,AurePlayerState,AbilitySystemComponent,AttributeSet);
		}
		
	}
	InitializeDefaultAbilities();
}
