// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/AuraEnemy.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "AbilitySystem/AureAbilitySystemComponent.h"
#include "AbilitySystem/AureAttributeSet.h"
#include "DungeonHunters2/DungeonHunters2.h"

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

	// 属性集
	AttributeSet = CreateDefaultSubobject<UAureAttributeSet>("AttributeSet");
	// 头顶血条组件
	HealthBar = CreateDefaultSubobject<UWidgetComponent>("HealthBar");
	HealthBar->SetupAttachment(GetRootComponent());
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

int32 AAuraEnemy::GetPlayerLevel()
{
	return Level;
}

void AAuraEnemy::BeginPlay()
{
	Super::BeginPlay();
	InitAbilityActorInfo();

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
		// 初始值也广播一次，避免 UI 留 0
		OnHealthChanged.Broadcast(AureAS->GetHealth());
		OnMaxHealthChanged.Broadcast(AureAS->GetMaxHealth());
	};
}

/* -------------------- ASC 初始化 -------------------- */
void AAuraEnemy::InitAbilityActorInfo()
{
	// Super::InitAbilityActorInfo();
	// Owner 与 Avatar 都设成自己，敌人没有 PlayerState
	AbilitySystemComponent->InitAbilityActorInfo(this, this);

	// 通知 ASC 内部完成后续注册
	Cast<UAureAbilitySystemComponent>(AbilitySystemComponent)->AbilityActorInfoSet();

	InitializeDefaultAbilities();
}

void AAuraEnemy::InitializeDefaultAbilities() const
{
	UAuraAbilitySystemLibrary::InitializeDefaultAttributes(this,CharacterClass,Level,AbilitySystemComponent);
	
}
