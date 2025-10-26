// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/AuraEnemy.h"

#include "AbilitySystemComponent.h"
#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "AbilitySystem/AureAbilitySystemComponent.h"
#include "AbilitySystem/AureAttributeSet.h"
#include "DungeonHunters2/DungeonHunters2.h"
#include "GameFramework/CharacterMovementComponent.h"

AAuraEnemy::AAuraEnemy()
{
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	AbilitySystemComponent = CreateDefaultSubobject<UAureAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true); //服务器复制
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	AttributeSet = CreateDefaultSubobject<UAureAttributeSet>("AttributeSet");
	HealthBar = CreateDefaultSubobject<UWidgetComponent>("HealthBar");
	HealthBar->SetupAttachment(GetRootComponent());
}

void AAuraEnemy::HighlightActor()
{
	// bHighlighted=true;
	//自定义深度显示
	GetMesh()->SetRenderCustomDepth(true);
	GetMesh()->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);

	Weapon->SetRenderCustomDepth(true);
	Weapon->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
}

void AAuraEnemy::UnHighlightActor()
{
	// bHighlighted=false;
	GetMesh()->SetRenderCustomDepth(false);
	Weapon->SetRenderCustomDepth(false);
}

int32 AAuraEnemy::GetPlayerLevel()
{
	return Level;
}


void AAuraEnemy::BeginPlay()
{
	Super::BeginPlay();
	GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed;
	InitAbilityActorInfo();
	UAuraAbilitySystemLibrary::GiveStartupAbilities(this,AbilitySystemComponent);

	if (UAuraUserWidget* AuraUserWidget = Cast<UAuraUserWidget>(HealthBar->GetUserWidgetObject()))
	{
		AuraUserWidget->SetWidgetController(this);
	}

	if (const UAureAttributeSet* AureAS = Cast<UAureAttributeSet>(AttributeSet))
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AureAS->GetHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnHealthChanged.Broadcast(Data.NewValue);
			}
		);

		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AureAS->GetMaxHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnMaxHealthChanged.Broadcast(Data.NewValue);
			}
		);

		AbilitySystemComponent->RegisterGameplayTagEvent(FAuraGameplayTags::Get().Effects_HitReact,
		                                                 EGameplayTagEventType::NewOrRemoved).AddUObject(
			this,
			&AAuraEnemy::HitReactTagChanged
		);
		OnHealthChanged.Broadcast(AureAS->GetHealth());
		OnMaxHealthChanged.Broadcast(AureAS->GetMaxHealth());
	};
}

void AAuraEnemy::HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	bHitReacting = NewCount > 0;
	//击中后，停止移动
	GetCharacterMovement()->MaxWalkSpeed = bHitReacting ? 0.0f : BaseWalkSpeed;
	if (NewCount > 0)
	{
	}
}

void AAuraEnemy::InitAbilityActorInfo()
{
	// Super::InitAbilityActorInfo();
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	Cast<UAureAbilitySystemComponent>(AbilitySystemComponent)->AbilityActorInfoSet();

	InitializeDefaultAbilities();
}

void AAuraEnemy::InitializeDefaultAbilities() const
{
	UAuraAbilitySystemLibrary::InitializeDefaultAttributes(this, CharacterClass, Level, AbilitySystemComponent);
}
