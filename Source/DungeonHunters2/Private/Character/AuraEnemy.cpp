// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/AuraEnemy.h"

#include "AbilitySystemComponent.h"
#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "AbilitySystem/AureAbilitySystemComponent.h"
#include "AbilitySystem/AureAttributeSet.h"
#include "AI/AuraAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DungeonHunters2/DungeonHunters2.h"
#include "GameFramework/CharacterMovementComponent.h"

AAuraEnemy::AAuraEnemy()
{
	/**
	 * ASC与AttributeSet自动绑定的触发时机，CreateDefaultSubobject
	 * ASC 作为Actor(AAuraEnemy) 的子对象,UAureAttributeSet 作为 Actor(AAuraEnemy) 的子对象
	 * 引擎调用 UAbilitySystemComponent::InitializeComponent() 时会：
	 * 		--遍历 Owner 的所有子对象,发现是 UAttributeSet 派生类 → 执行
	 */
	// todo  缺失？ bReplicates = true;

	// 让网格体阻挡 Visibility 通道，便于鼠标检测
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);

	// ===== ASC 创建与复制 ==========================================
	AbilitySystemComponent = CreateDefaultSubobject<UAureAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true); //服务器复制

	// Minimal 模式：仅复制 ActiveGE 列表与属性，不复制 GameplayCue
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;

	// 属性集
	AttributeSet = CreateDefaultSubobject<UAureAttributeSet>("AttributeSet");
	// 头顶血条组件
	HealthBar = CreateDefaultSubobject<UWidgetComponent>("HealthBar");
	HealthBar->SetupAttachment(GetRootComponent());
}

void AAuraEnemy::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (!HasAuthority()) return;
	AuraAIController = Cast<AAuraAIController>(NewController);
	AuraAIController->GetBlackboardComponent()->InitializeBlackboard(*BehaviorTree->BlackboardAsset);
	AuraAIController->RunBehaviorTree(BehaviorTree);
	AuraAIController->GetBlackboardComponent()->SetValueAsBool(FName("HitReacting"), false);
	// 远程攻击类型
	AuraAIController->GetBlackboardComponent()->SetValueAsBool(FName("RangedAttacker"),
	                                                           CharacterClass != ECharacterClass::Warrior);
}

/* -------------------- 接口实现 -------------------- */
void AAuraEnemy::HighlightActor()
{
	// bHighlighted=true;
	//自定义深度显示 / 自定义深度描边高亮（红色）
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

int32 AAuraEnemy::GetPlayerLevel_Implementation()
{
	return Level;
}

void AAuraEnemy::Die()
{
	SetLifeSpan(LifeSpan);
	if (AuraAIController)
	{
		AuraAIController->GetBlackboardComponent()->SetValueAsBool(FName("Dead"), true);
	}
	Super::Die();
}

void AAuraEnemy::SetCombatTarget_Implementation(AActor* InCombatTarget)
{
	CombatTarget = InCombatTarget;
}

AActor* AAuraEnemy::GetCombatTarget_Implementation() const
{
	return CombatTarget;
}


void AAuraEnemy::BeginPlay()
{
	Super::BeginPlay();
	GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed;
	InitAbilityActorInfo();
	if (HasAuthority())
	{
		UAuraAbilitySystemLibrary::GiveStartupAbilities(this, AbilitySystemComponent, CharacterClass);
	}


	// 把血条控件控制器指向自己，方便属性绑定
	if (UAuraUserWidget* AuraUserWidget = Cast<UAuraUserWidget>(HealthBar->GetUserWidgetObject()))
	{
		AuraUserWidget->SetWidgetController(this);
	}


	// 监听 Health/MaxHealth 变化并广播到蓝图（UI 订阅）
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
		// 初始值也广播一次，避免 UI 留 0
		OnHealthChanged.Broadcast(AureAS->GetHealth());
		OnMaxHealthChanged.Broadcast(AureAS->GetMaxHealth());
	};
}

void AAuraEnemy::HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	bHitReacting = NewCount > 0;
	//击中后，停止移动
	GetCharacterMovement()->MaxWalkSpeed = bHitReacting ? 0.0f : BaseWalkSpeed;

	if (AuraAIController && AuraAIController->GetBlackboardComponent())
	{
		AuraAIController->GetBlackboardComponent()->SetValueAsBool(FName("HitReacting"), bHitReacting);
	}

	
}

/* -------------------- ASC 初始化 -------------------- */
void AAuraEnemy::InitAbilityActorInfo()
{
	// Super::InitAbilityActorInfo();
	// Owner 与 Avatar 都设成自己，敌人没有 PlayerState
	AbilitySystemComponent->InitAbilityActorInfo(this, this);

	// 通知 ASC 内部完成后续注册
	Cast<UAureAbilitySystemComponent>(AbilitySystemComponent)->AbilityActorInfoSet();
	if (HasAuthority())
	{
		InitializeDefaultAbilities();
	}
}

void AAuraEnemy::InitializeDefaultAbilities() const
{
	UAuraAbilitySystemLibrary::InitializeDefaultAttributes(this, CharacterClass, Level, AbilitySystemComponent);
}
